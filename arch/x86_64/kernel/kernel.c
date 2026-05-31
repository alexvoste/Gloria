#include "../include/limine.h"
#include "../include/video.h"

typedef unsigned long long u64;
typedef unsigned int u32;

#define NULL ((void *)0)

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_framebuffer_request
    framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

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

  /*
video_draw_rect(50, 50, 200, 100, COLOR_RED);
video_draw_rect(100, 200, 150, 75, COLOR_GREEN);
video_draw_line(50, 50, 300, 250, COLOR_BLUE);
video_draw_line(250, 50, 50, 300, COLOR_WHITE);
video_draw_line(350, 100, 500, 400, COLOR_RED);
  */

  video_print(60, 80, "GloriaOS Kernel booted successfully!", COLOR_WHITE);
  video_print(60, 100, "Graphics driver active", COLOR_GREEN);
  video_print(60, 120, "Bresenham lines enabled", COLOR_BLUE);

  while (1) {
    __asm__ volatile("hlt");
  }
}
