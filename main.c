#include "include/delay.h"
#include "include/io.h"
#include "include/keyboard.h"
#include "include/vga.h"
#include "include/string.h"

void init_kernel_modules() {
  for (int i = 0; i < 143; i++) {
    glob_vga_color = vga_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    vga_printf("[");
    glob_vga_color = vga_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    if (i % 10 == 0) {
      glob_vga_color = vga_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
      vga_printf(" FAILED ");
    } else {
      vga_printf(" OK ");
    }
    glob_vga_color = vga_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    vga_printf("] ");
    vga_printf("Initializing module \"#%X\"\n", i);
    _delay_cycles(1000000);
  }
}

void kernel_entry() {
  // Set up the VGA.
  glob_vga_color = vga_color(VGA_COLOR_RED, VGA_COLOR_BLACK);
  vga_init();
  init_kernel_modules();
  glob_vga_color = vga_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  _delay_cycles(400000000);
  vga_init();
  vga_printf("\n\n\n\n\n\n\n\n\n\n\n\n\n                            BadOS "
             "v0.0.1\n                            Loading");
  for (int i = 0; i < 3; i++) {
    vga_printf(".");
    _delay_cycles(100000000);
  }

  vga_init();
  pic_init(0x20, 0x28);

  vga_printf(">");

  char c = 0;
  char command[256];
  int command_index = 0;
  for(int i = 0; i < 256; i++) command[i] = 0;

  do {
    if (inb(0x60) != c) {
      c = inb(0x60);
      if (c > 0) {
        char co = translate_ps2_scancode_to_char(c);
        if (co == '\n') {

          vga_printf("\n");
          if (strcmp(command, "help")) {
            vga_printf("Fuck you there is no help");
          }else if (strcmp(command, "shutdown")) {
            vga_printf("Shutting the computer down. This only works on VMs for now. TODO: Implement ACPI magic.");
            c = 1;
          }else{
            vga_printf("Unknown command: %s", command);
          }

          vga_printf("\n");

          command_index = 0;
          for(int i = 0; i < 256; i++) command[i] = 0;
          vga_printf(">");
        } else if (co == '\b') {
          if(command_index == 0) continue;
          if (glob_x > 0) {
            glob_x--;
            vga_char_out(glob_x, glob_y, ' ', glob_vga_color);
            glob_x--;
          } else {
            glob_y--;
            glob_x = VGA_WIDTH - 1;
            vga_char_out(glob_x, glob_y, ' ', glob_vga_color);
            glob_y--;
            glob_x = VGA_WIDTH - 1;
          }
          command[--command_index] = 0;
        } else {
          vga_char_out(glob_x, glob_y, co,
                       glob_vga_color);
          command[command_index++] = co;
        }
      }
    }

  } while (c != 1);

  vga_init();
  vga_printf("\n\n\n\n\n\n\n\n\n\n\n\n\n                            BadOS "
             "v0.0.1\n                            Shutdown");

  outw(0x604, 0x2000);
  outw(0xB004, 0x2000);
  outw(0x4004, 0x3400); // force shutdown on virtual machines

  vga_init();
  vga_printf("You are a fucking dumbass if you ran this on real hardware. Shutdown not implemented yet. Go fuck your ACPI.");

  while (1)
    ;
}