#include "../include/terminal.h"
#include "../include/video.h"

extern void *memmove(void *dest, const void *src, size_t n);
extern void *memset(void *s, int c, size_t n);

static struct limine_framebuffer *g_fb = NULL;
static uint32_t g_fg_color = 0xFFFFFFFF;
static uint32_t g_bg_color = 0x00000000;

static int term_cursor_x = 0;
static int term_cursor_y = 0;
static int term_cols = 0;
static int term_rows = 0;

#define CHARACTER_WIDTH 8
#define CHARACTER_HEIGHT 8

void term_init(struct limine_framebuffer *fb, uint32_t fg_color,
               uint32_t bg_color) {
  g_fb = fb;
  g_fg_color = fg_color;
  g_bg_color = bg_color;

  term_cursor_x = 0;
  term_cursor_y = 0;

  term_cols = fb->width / CHARACTER_WIDTH;
  term_rows = fb->height / CHARACTER_HEIGHT;
}

static void term_scroll(void) {
  size_t pitch = g_fb->pitch;
  size_t row_bytes = pitch * CHARACTER_HEIGHT;

  void *dest = (void *)g_fb->address;
  const void *src = (const void *)((uintptr_t)g_fb->address + row_bytes);
  size_t bytes_to_copy = pitch * (g_fb->height - CHARACTER_HEIGHT);

  memmove(dest, src, bytes_to_copy);

  void *bottom_row_start = (void *)((uintptr_t)g_fb->address +
                                    pitch * (g_fb->height - CHARACTER_HEIGHT));

  uint32_t *pixel_ptr = (uint32_t *)bottom_row_start;
  size_t total_pixels = (g_fb->width) * CHARACTER_HEIGHT;
  for (size_t i = 0; i < total_pixels; i++) {
    pixel_ptr[i] = g_bg_color;
  }

  term_cursor_y = term_rows - 1;
}

void term_putchar(char c) {
  if (!g_fb)
    return;

  if (c == '\n') {
    term_cursor_x = 0;
    term_cursor_y++;
  } else if (c == '\r') {
    term_cursor_x = 0;
  } else if (c == '\t') {
    term_cursor_x = (term_cursor_x + 8) & ~7;
  } else {
    video_putchar(term_cursor_x * CHARACTER_WIDTH,
                  term_cursor_y * CHARACTER_HEIGHT, c, g_fg_color);
    term_cursor_x++;
  }

  if (term_cursor_x >= term_cols) {
    term_cursor_x = 0;
    term_cursor_y++;
  }

  if (term_cursor_y >= term_rows) {
    term_scroll();
  }
}

void term_print(const char *str) {
  for (size_t i = 0; str[i] != '\0'; i++) {
    term_putchar(str[i]);
  }
}

void term_clear(void) {
  if (!g_fb)
    return;

  uint32_t *pixel_ptr = (uint32_t *)g_fb->address;
  size_t total_pixels = (g_fb->pitch / 4) * g_fb->height;
  for (size_t i = 0; i < total_pixels; i++) {
    pixel_ptr[i] = g_bg_color;
  }

  term_cursor_x = 0;
  term_cursor_y = 0;
}
