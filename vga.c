#include "include/vga.h"

int glob_x = 0;
int glob_y = 0;
uint8_t glob_vga_color;

uint8_t vga_color(uint8_t fg, uint8_t bg) { return fg | bg << 4; }

uint16_t vga_char(unsigned char uc, uint8_t color) {
  return (uint16_t)uc | (uint16_t)color << 8;
}

void vga_init() {
  glob_x = 0;
  glob_y = 0;
  uint16_t *vga_mem = (uint16_t *)0xB8000;
  for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
    vga_mem[i] = vga_char(' ', glob_vga_color);
  }
}

void vga_char_out(int x, int y, char c, uint8_t color) {
  uint16_t *vga_mem = (uint16_t *)0xB8000;
  vga_mem[y * VGA_WIDTH + x] = vga_char(c, color);
  x++;
  if (x >= VGA_WIDTH) {
    x = 0;
    y++;
  }
  if (y >= VGA_HEIGHT) {
    return scroll_terminal(1);
  }
  glob_x = x;
  glob_y = y;
}

void vga_text_out(int x, int y, char *text, uint8_t color) {
  uint16_t *vga_mem = (uint16_t *)0xB8000;
  for (int i = 0; i < strlen(text); i++) {
    vga_mem[y * VGA_WIDTH + x] = vga_char(text[i], color);
    x++;
    if (x >= VGA_WIDTH) {
      x = 0;
      y++;
    }
    if (y >= VGA_HEIGHT) {
      scroll_terminal(1);
      continue;
    }
  }
  glob_x = x;
  glob_y = y;
}

void print_digits(int n, int base, char *base_digits) {
  if(n == 0) return vga_char_out(glob_x, glob_y, '0', glob_vga_color);
  size_t rem = n;

  size_t tmp_rem = rem;
  uint8_t total_length = 0;
  while (tmp_rem > 0) {
    total_length++;
    tmp_rem /= base;
  }

  char buff[total_length + 1];
  for(int i = 0; i < total_length + 1; i++) buff[i] = 0;

  uint8_t tot = 0;
  for (tot = total_length - 1; rem; tot--) {
    buff[tot] = base_digits[(rem % base)];
    rem /= base;
  }

  vga_text_out(glob_x, glob_y, buff, glob_vga_color);
}

void vga_printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int c = *fmt++;
  while (c != '\0') {
    if (c == '%') {
      c = *fmt++;
      switch (c) {
      case 's': {
        char *s = va_arg(args, char *);
        vga_text_out(glob_x, glob_y, s, glob_vga_color);
        break;
      }
      case 'c': {
        char c = va_arg(args, int);
        vga_text_out(glob_x, glob_y, &c, glob_vga_color);
        break;
      }
      case 'd': {
        int d = va_arg(args, int);
        print_digits(d, 10, "0123456789");
        break;
      }
      case 'x': {
        int d = va_arg(args, int);
        print_digits(d, 16, "0123456789abcdef");
        break;
      }
      case 'X': {
        int d = va_arg(args, int);
        print_digits(d, 16, "0123456789ABCDEF");
        break;
      }
      default:
        vga_char_out(glob_x, glob_y, '%', glob_vga_color);
        vga_char_out(glob_x, glob_y, c, glob_vga_color);
        break;
      }
    } else if (c == '\n') {
      glob_x = 0;
      glob_y++;
      if(glob_y >= VGA_HEIGHT) {
        scroll_terminal(1);
      }
    } else if (c == '\r') {
      glob_x = 0;
    } else {
      vga_char_out(glob_x, glob_y, c, glob_vga_color);
    }
    c = *fmt++;
  }
  va_end(args);
}

void scroll_terminal(int lines) {
  uint16_t *vga_mem = (uint16_t *)0xB8000;
  int i = 0;
  for (i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
    vga_mem[i] = i + (VGA_WIDTH * lines) < VGA_WIDTH * VGA_HEIGHT
                     ? vga_mem[i + (VGA_WIDTH * lines)]
                     : vga_char(' ', glob_vga_color);
  }
  glob_x = 0;
  glob_y = (i - (VGA_WIDTH * lines)) / VGA_WIDTH;
}