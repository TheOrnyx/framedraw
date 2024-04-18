#include <stdlib.h>
#include <stdio.h>
#include <sysexits.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/fb.h>

#define ERR(exit_msg, perror_src, exit_code)		\
		 fprintf(stderr, exit_msg);									\
		 perror(perror_src);												\
		 exit(exit_code);

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


int main(int argc, char *argv[]) {
		 if (argc < 2) {
					printf("Please enter a filename");
					return EX_NOINPUT;
		 }
		 
		 const char *file_name = argv[2]; //TODO - replace this later with a command line flag
		 int fd = get_fd();

		 struct fb_fix_screeninfo fsi;
		 get_fix_screeninfo(fd, &fsi);

#ifdef DEBUG
		 print_fix_screeninfo(&fsi);
#endif

		 struct fb_var_screeninfo vsi;
		 get_var_screeninfo(fd, &vsi);

#ifdef DEBUG
		 print_var_screeninfo(&vsi);
#endif
		 
		 return 0;
}

int get_fd(void) {
		 // try to get framebuffer from environment var or fixed fb0 file
		 char *fb_path = getenv("FRAMEBUFFER");
		 if (fb_path == NULL) {
					fb_path = "/dev/fb0";
		 }

		 int fd = open(fb_path, O_RDWR);
		 if (fd < 0) {
					ERR("Failed to open file", "open", EX_NOINPUT);
		 }
		 
		 return fd;
}

void get_fix_screeninfo(int fd, struct fb_fix_screeninfo *fsi) {
		 if (ioctl(fd, FBIOGET_FSCREENINFO, fsi)) {
					ERR("Failed to get screeninfo via FIOGET_FSCREENINFO\n", "ioctl", EXIT_FAILURE);
		 }
}

void get_var_screeninfo(int fd, struct fb_var_screeninfo *vsi) {
		 if (ioctl(fd, FBIOGET_VSCREENINFO, vsi)) {
					ERR("Failed to get vscreeninfo via FBIOGET_VSCREENINFO\n", "ioctl", EXIT_FAILURE);
		 }
}

// Print out a fb_bitfield (once again stolen from soda, thanks)
void print_bitfield(const struct fb_bitfield *bf, const char *name) {
		 printf("    <fb_bitfield> %s {\n", name);
		 printf("        offset = %u\n", bf->offset);
		 printf("        length = %u\n", bf->length);
		 printf("        msb_right = %u\n", bf->msb_right);
		 printf("    };\n");
}

void print_var_screeninfo(const struct fb_var_screeninfo *vsi) {
		 printf("struct fb_var_screeninfo {\n");
		 printf("    xres = %u;\n", vsi->xres);
		 printf("    yres = %u;\n", vsi->yres);
		 printf("    xres_virtual = %u;\n", vsi->xres_virtual);
		 printf("    yres_virtual = %u;\n", vsi->yres_virtual);
		 printf("    xoffset = %u;\n", vsi->xoffset);
		 printf("    yoffset = %u;\n", vsi->yoffset);

		 printf("    bits_per_pixel = %u;\n", vsi->bits_per_pixel);
		 printf("    grayscale = %u;\n", vsi->grayscale);

		 print_bitfield(&vsi->red, "red");
		 print_bitfield(&vsi->green, "green");
		 print_bitfield(&vsi->blue, "blue");
		 print_bitfield(&vsi->transp, "transp");

		 printf("    nonstd = %u;\n", vsi->nonstd);

		 printf("    activate = %u;\n", vsi->activate);

		 printf("    height = %u;\n", vsi->height);
		 printf("    width = %u;\n", vsi->width);

		 printf("    accel_flags = %u;\n", vsi->accel_flags);

		 printf("    pixclock = %u;\n", vsi->pixclock);
		 printf("    left_margin = %u;\n", vsi->left_margin);
		 printf("    right_margin = %u;\n", vsi->right_margin);
		 printf("    upper_margin = %u;\n", vsi->upper_margin);
		 printf("    lower_margin = %u;\n", vsi->lower_margin);
		 printf("    hsync_len = %u;\n", vsi->hsync_len);
		 printf("    vsync_len = %u;\n", vsi->vsync_len);
		 printf("    sync = %u;\n", vsi->sync);
		 printf("    vmode = %u;\n", vsi->vmode);
		 printf("    rotate = %u;\n", vsi->rotate);
		 printf("    colorspace = %u;\n", vsi->colorspace);
		 printf("    reserved[4] = { %u, %u, %u, %u };\n", vsi->reserved[0],
						vsi->reserved[1], vsi->reserved[2], vsi->reserved[3]);
		 printf("}\n");
}

void print_fix_screeninfo(const struct fb_fix_screeninfo *fsi) {
		 printf("struct fb_fix_screeninfo {\n");
		 printf("    id = %.16s;\n", fsi->id);
		 printf("    smem_start = %lu;\n", fsi->smem_start);
		 printf("\n");
		 printf("    smem_len = %u;\n", fsi->smem_len);
		 printf("    type = %u;\n", fsi->type);
		 printf("    type_aux = %u;\n", fsi->type_aux);
		 printf("    visual = %u;\n", fsi->visual);
		 printf("    xpanstep = %hu\n", fsi->xpanstep);
		 printf("    ypanstep = %hu\n", fsi->ypanstep);
		 printf("    ywrapstep = %hu\n", fsi->ywrapstep);
		 printf("    line_length = %u;\n", fsi->line_length);
		 printf("    mmio_start = %lu;\n", fsi->mmio_start);
		 printf("\n");
		 printf("    mmio_len = %u;\n", fsi->mmio_len);
		 printf("    accel = %u;\n", fsi->accel);
		 printf("\n");
		 printf("    capabilities = %hu;\n", fsi->capabilities);
		 printf("    reserved = { %hu, %hu };\n", fsi->reserved[0],
						fsi->reserved[1]);
		 printf("}\n");
}
