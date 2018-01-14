#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
	#error "You must be using a cross-compiler!"
#elif !defined(__i386__)
	#error "Kernel must be compiled with a ix86-elf compiler!"
#endif

/* VGA color constants */
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

/* VGA textmode buffer */
volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

/* Build color vga entry in 8 bit format */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

/* Build vga entry */
static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

// Initialize cursor position to top left corner
int term_col = 0;
int term_row = 0;
uint8_t term_color = 0x0F;

/* Initialize terminal by filling it with space characters */
void term_init(void)
{
	for (int row = 0; row < VGA_ROWS; row++)
	{
		for (int col = 0; col < VGA_COLS; col++)
		{
			const size_t index = (VGA_COLS * row) + col;
			vga_buffer[index] = vga_entry(' ', 0x0F); /* Default to white on black */
		}
	}
}

/* Places a single character on the screen */
void term_putc(unsigned char c)
{
	// TODO implement more support for ANSI escape codes
	switch(c)
	{
		case '\n':	// newline character
			term_col = 0;
			term_row++;
			break;
		default:	// Any other characters
			const size_t index = (VGA_COLS * term_row) + term_col;
			vga_buffer[index] = vga_entry(c, term_color);
			term_col++;
			break;
	}

	/* End of line */
	if (term_col >= VGA_COLS)
	{
		term_col = 0;
		term_row++;
	}

	/* Bottom of terminal */
	if (term_row >= VGA_ROWS)
	{
		for (int row = 0; row < VGA_ROWS; row++)
		{
			for (int col = 0; col < VGA_COLS; col++)
			{
				const size_t dest_index = (VGA_COLS * row) + col;
				if (row == VGA_ROWS - 1)
				{
					vga_buffer[dest_index] = vga_entry(' ', 0x0F);
				}
				else
				{
					const size_t src_index = (VGA_COLS * (row + 1)) + col;
					vga_buffer[dest_index] = vga_buffer[src_index];
				}
			}
		}

		term_col = 0;
		term_row = VGA_ROWS - 1;
	}
}

/* Print a null terminated string */
void term_print(const char* str)
{
	for (size_t i = 0; str[i] != '\0'; i++)
		term_putc(str[i]);
}

/* Kernel main function */
extern "C" /* Use C linkage */
void kernel_main(void)
{
	// Initialize terminal
	term_init();

	// Display test string
	term_print("Hello, World!\n");
	term_print("Kernel loaded.\n");
}