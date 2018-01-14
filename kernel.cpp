#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
	#error "You must be using a cross-compiler!"
#elif !defined(__i386__)
	#error "Kernel must be compiled with a ix86-elf compiler!"
#endif

// VGA textmode buffer
volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

// Initialize cursor position to top left corner
int term_col = 0;
int term_row = 0;
uint8_t term_color = 0x0F;

// Initialize terminal by filling it with space characters
void term_init(void)
{
	for (int col = 0; col < VGA_COLS; col++)
	{
		for (int row = 0; row < VGA_ROWS; row++)
		{
			const size_t index = (VGA_COLS * row) + col;
			vga_buffer[index] = ((uint16_t)term_color << 8) | ' ';
		}
	}
}

// Places a single character on the screen
void term_putc(char c)
{
	switch(c)
	{
		case '\n':	// newline character
			term_col = 0;
			term_row++;
			break;
		default:	// Any other characters
			const size_t index = (VGA_COLS * term_row) + term_col;
			vga_buffer[index] = ((uint16_t)term_color << 8) | c;
			term_col++;
			break;
	}

	if (term_col >= VGA_COLS)
	{
		term_col = 0;
		term_row++;
	}

	if (term_row >= VGA_ROWS)
	{
		// Handle scrolling here
		// For now, reset to top of screen
		term_col = 0;
		term_row = 0;
	}
}

// Print a null terminated string
void term_print(const char* str)
{
	for (size_t i = 0; str[i] != '\0'; i++)
		term_putc(str[i]);
}

// Kernel main function
extern "C" /* Use C linkage */
void kernel_main(void)
{
	// Initialize terminal
	term_init();

	// Display test string
	term_print("Hello, World!\n");
	term_print("Kernel loaded.\n");
}