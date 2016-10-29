__asm__(".code16gcc\n");
__asm__ ("jmpl  $0, $main\n");

typedef signed char int8_t;
typedef signed int int16_t;
typedef signed long int int32_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long int uint32_t;

#define SCREEN_ADDR 0xb800
#define SCREEN_COLS 80
#define SCREEN_ROWS 25

#define BLACK 0x00
#define BLUE 0x01
#define GREEN 0x02
#define CYAN 0x03
#define RED 0x04
#define MAGENTA 0x05
#define BROWN 0x06
#define LIGHT_GREY 0x07
#define DARK_GREY 0x08
#define LIGHT_BLUE 0x09
#define LIGHT_GREEN 0x0A
#define LIGHT_CYAN 0x0B
#define LIGHT_RED 0x0C
#define LIGHT_MAGENTA 0x0D
#define LIGHT_BROWN 0X0E
#define WHITE 0X0F

static uint16_t *screen = (uint16_t*)(SCREEN_ADDR << 4);

/*
  Absolute Memory Location = (Segment value * 16) + Offset value
 */
static inline uint32_t get_addr(uint16_t segment, uint16_t offset)
{
  return (segment << 4) + offset;
}

static inline uint8_t get_text_attribute(uint8_t fg, uint8_t bg){
  return (bg << 4) | (fg & 0x0F);
}

static inline void clear_screen()
{
  uint16_t data = (get_text_attribute(WHITE, BLACK) << 8) | ' ';

  int i, j;
  for (i = 0; i < SCREEN_ROWS; ++i) {
    for (j = 0; j < SCREEN_COLS; ++j) {
      screen[i * SCREEN_COLS + j] = data;
    }
  }
}

static inline void screen_print(char c, uint8_t row, uint8_t col)
{
  uint16_t data = (get_text_attribute(WHITE, BLACK) << 8) | c;
  screen[row * SCREEN_COLS + col] = data;
}

static uint8_t row = 0;
static uint8_t col = 0;
void printstr(char* s)
{
  while (*s) {
    if (*s == '\n') {
      col = 0;
      row++;
    }

    if (*s > 31 && *s < 127 ) {
      screen_print(*s, row, col++);
    }

    s++;

    if (col == SCREEN_COLS) {
      col = 0;
      row++;
    }
  }
}

void main()
{
  uint8_t bios_drive = 0;
  __asm__ __volatile__("movb  %%dl, %0" : "=r"(bios_drive));

  clear_screen();

  printstr("Welcome to David OS.\n");
  printstr("Booted from: ");

  while (1);
}