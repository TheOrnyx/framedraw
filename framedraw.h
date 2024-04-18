#include <cairo/cairo.h>
#include <linux/fb.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef FRAMEDRAW_H
#define FRAMEDRAW_H

#define FORMAT CAIRO_FORMAT_ARGB32

#define ERR(exit_msg, perror_src, exit_code)		\
		fprintf(stderr, exit_msg);									\
		perror(perror_src);													\
		exit(exit_code);

union fb_color {
		struct __attribute__((__packed__)) {
				uint8_t b;
				uint8_t g;
				uint8_t r;
				uint8_t a;
		} rgba;
		uint32_t data;
};

typedef struct fb_device {
		int fd;
		struct fb_fix_screeninfo fsi;
		struct fb_var_screeninfo vsi;
		size_t mem_size;
		uint32_t *fb_data;
		union fb_color *fbc;
		union fb_color bg_col;
} fb_dev;

// open the framebuffer file
int get_fd(void);
// Get the fixed screeninfo from fd
void get_fix_screeninfo(int fd, struct fb_fix_screeninfo *fsi);
// Get the 
void get_var_screeninfo(int fd, struct fb_var_screeninfo *vsi);
// Print the info about a var screeninfo struct (Stolen from soda, thanks)
void print_var_screeninfo(const struct fb_var_screeninfo *vsi);
// Print the info about a fix screeninfo struct (Also stolen from soda, thanks)
void print_fix_screeninfo(const struct fb_fix_screeninfo *fsi);
// Get the background color for the framebuffer (once again partly stolen from sofa, thanks ^^)
union fb_color get_bg_color(const union fb_color *fbc, uint32_t width, uint32_t height);
cairo_surface_t *make_surface_from_fb(fb_dev *dev);
void draw_with_cairo(fb_dev *dev, cairo_t *fbcr, const char *img_path);

#endif /* FRAMEDRAW_H */
