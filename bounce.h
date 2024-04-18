#include "framedraw.h"

#ifndef BOUNCE_H
#define BOUNCE_H

#define DEF_X_VEL 3
#define DEF_Y_VEL 4

typedef struct bounce_object {
	fb_dev *dev;
	cairo_t *fbcr;
	cairo_surface_t *img_surface;
	int x_pos;
	int y_pos;
	int x_vel;
	int y_vel;
	int w;
	int h;
	int x_scale;
	int y_scale;
} bounce_obj;

void run_bounce(fb_dev *dev, cairo_t *fbcr, const char *img_path);
void move_bounce(bounce_obj *b);
void draw_bounce(bounce_obj *b);


#endif /* BOUNCE_H */


