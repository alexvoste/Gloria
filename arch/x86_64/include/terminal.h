#ifndef TERMINAL_H
#define TERMINAL_H

#include "limine.h"
#include <stddef.h>
#include <stdint.h>

void term_init(struct limine_framebuffer *fb, uint32_t fg_color,
               uint32_t bg_color);
void term_putchar(char c);
void term_print(const char *str);
void term_clear(void);

#endif
