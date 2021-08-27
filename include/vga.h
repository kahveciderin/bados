#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "string.h"

#define VGA_WIDTH        80
#define VGA_HEIGHT       25

extern int glob_x;
extern int glob_y;
extern uint8_t glob_vga_color;

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

uint8_t vga_color(uint8_t fg, uint8_t bg);
uint16_t vga_char(unsigned char uc, uint8_t color);
void vga_init();
void print_digits(int n, int base, char *base_digits);
void vga_char_out(int x, int y, char c, uint8_t color);
void vga_text_out(int x, int y, char *text, uint8_t color);
void vga_printf(const char *fmt, ...);
void scroll_terminal(int lines);