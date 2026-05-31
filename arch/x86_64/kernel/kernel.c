#include "../include/limine.h"
#include "../include/terminal.h"
#include "../include/video.h"

typedef unsigned long long u64;
typedef unsigned int u32;

#define NULL ((void *)0)

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

void *memset(void *s, int c, u64 n) {
  uint8_t *p = s;

  for (u64 i = 0; i < n; i++) {
    p[i] = (uint8_t)c;
  }

  return s;
}

void *memmove(void *dest, const void *src, u64 n) {
  uint8_t *pdest = dest;
  const uint8_t *psrc = src;

  if ((uintptr_t)src > (uintptr_t)dest) {
    for (u64 i = 0; i < n; i++) {
      pdest[i] = psrc[i];
    }
  } else if ((uintptr_t)src < (uintptr_t)dest) {
    for (u64 i = n; i > 0; i--) {
      pdest[i - 1] = psrc[i - 1];
    }
  }

  return dest;
}

int memcmp(const void *s1, const void *s2, u64 n) {
  const uint8_t *p1 = s1;
  const uint8_t *p2 = s2;

  for (u64 i = 0; i < n; i++) {
    if (p1[i] != p2[i]) {
      return p1[i] < p2[i] ? -1 : 1;
    }
  }

  return 0;
}

void _start(void) {
  if (framebuffer_request.response == NULL ||
      framebuffer_request.response->framebuffer_count < 1) {
    while (1) {
      __asm__ volatile("cli; hlt");
    }
  }

  struct limine_framebuffer *framebuffer =
      framebuffer_request.response->framebuffers[0];

  video_init(framebuffer);

  video_clear(0x0F0F1F);

  term_init(framebuffer, 0x00FF00FF, 0x0A0A1EFF);

  term_print("GloriaOS Terminal System v1.0 booted successfully!\n");

  for (int i = 1; i <= 2; i++) {
    term_print("this is system log message number: ");
    term_putchar('0' + (i / 10));
    term_putchar('0' + (i % 10));
    term_print("] - Everything is working like a charm!\n");
  }

  term_print("\All tasks completed. Stading by input...\n");

  /*
video_draw_rect(50, 50, 200, 100, COLOR_RED);
video_draw_rect(100, 200, 150, 75, COLOR_GREEN);
video_draw_line(50, 50, 300, 250, COLOR_BLUE);
video_draw_line(250, 50, 50, 300, COLOR_WHITE);
video_draw_line(350, 100, 500, 400, COLOR_RED);
  */

  while (1) {
    __asm__ volatile("hlt");
  }
}
