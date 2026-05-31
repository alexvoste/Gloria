#ifndef VIDEO_H
#define VIDEO_H

#include "limine.h"
#include <stdint.h>

typedef uint32_t color_t;

#define COLOR_BLACK 0x00000000
#define COLOR_WHITE 0xffffffff
#define COLOR_RED 0xffff0000
#define COLOR_GREEN 0xff00ff00
#define COLOR_BLUE 0xff0000ff

void video_init(struct limine_framebuffer *fb);
void video_put_pixel(int x, int y, uint32_t color);
void video_clear(uint32_t color);
void video_draw_rect(int x, int y, int width, int height, uint32_t color);
void video_draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void video_putchar(int x, int y, char c, uint32_t color);
void video_print(int x, int y, const char *str, uint32_t color);

#endif
