#include "bounce.h"
#include <cairo/cairo.h>
#include <stdio.h>
#include <sys/param.h>
#include <unistd.h>

void run_bounce(fb_dev *dev, cairo_t *fbcr, const char *img_path) {
	bounce_obj b;
	b.fbcr = fbcr;
	b.dev = dev;
	b.img_surface = cairo_image_surface_create_from_png(img_path);
	b.x_vel = DEF_X_VEL;
	b.y_vel = DEF_Y_VEL;
	b.x_pos = 0;
	b.y_pos = 0;
	b.w = cairo_image_surface_get_width(b.img_surface);
	b.h = cairo_image_surface_get_height(b.img_surface);
	b.x_scale = MIN(b.w, dev->vsi.xres_virtual);
	b.y_scale = MIN(b.h, dev->vsi.yres_virtual);
	setvbuf (stdout, NULL, _IONBF, BUFSIZ);
	
	while (1) {
		move_bounce(&b);
		draw_bounce(&b);
		printf("\r");
		usleep(10000);
	}
}

void draw_bounce(bounce_obj *b) {
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_RGB16_565,
																												b->dev->vsi.xres_virtual,
																												b->dev->vsi.yres_virtual);
	cairo_t *cr = cairo_create(surface); // create new surfaces to avoid flickering
	union fb_color bg_col = b->dev->bg_col;
	cairo_set_source_rgba(cr, bg_col.rgba.r, bg_col.rgba.g, bg_col.rgba.b, bg_col.rgba.a);
	/* cairo_identity_matrix(cr); */
	

	// Clear the screen
	cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
	cairo_paint(cr);
	cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
	
	cairo_scale(cr, b->x_scale/b->w, b->y_scale/b->h);
	cairo_set_source_surface(cr, b->img_surface, b->x_pos, b->y_pos);
	cairo_paint(cr);

	cairo_set_source_surface(b->fbcr, surface, 0, 0);
	cairo_paint(b->fbcr);
	cairo_surface_destroy(surface);
	cairo_destroy(cr);
}

void move_bounce(bounce_obj *b) {
	// update X
	b->x_pos += b->x_vel;
	if (b->x_pos < 0) {
		b->x_pos = 0;
		b->x_vel = -b->x_vel;
	} else if (b->x_pos + b->w > b->dev->vsi.xres_virtual) {
		b->x_pos = b->dev->vsi.xres_virtual - b->w;
		b->x_vel = -b->x_vel;
	}

	// Update y position
	b->y_pos += b->y_vel;
	if (b->y_pos < 0) {
		b->y_pos = 0;
		b->y_vel = -b->y_vel;
	} else if (b->y_pos + b->h > b->dev->vsi.yres_virtual) {
		b->y_pos = b->dev->vsi.yres_virtual - b->h;
		b->y_vel = -b->y_vel;
	}
}
