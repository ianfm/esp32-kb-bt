/** BT-KB-OLED
 * @file bt-kb-oled.ino
 * @author Ian McMurray
 * 
 * Initial work outline:
 * Start with a small text display routine
 * Next tap into the keyboard wiring header or cable
 * Get Vcc, Gnd, Clk, and Data pinned out on a breadboard
 * Open PS/2 / AT command interface 
 * Get the right transmission timing and w.e. right
 * Get keypresses to show up on display
 * 
 * Explore BT interface
 * Look through BT HID projects 
 * 
 * @note
 *  The display can write 4 lines of text, 21 characters each
 *  Lines wrap ehn longer than
 * 
 * 
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library but can be overwritten
// with Wire.setPins(SDA, SCL); in loop()
#define SDA 18
#define SCL 19

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };



void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally

  Wire.setPins(SDA, SCL);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  delay(2000);

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
}

void loop() {
  const int frameDuration = 8000;
  testPrintLines(1);
  delay(frameDuration);
  testRowsText(1);
  delay(frameDuration);
  testPrintLineWrap(1);
  delay(frameDuration);
  testPrintBigLines(1);
  delay(frameDuration);
  testPrintExtraBig(1);
  delay(frameDuration);
  testPrintPositions(1);
  delay(frameDuration);
}

/* ******************************************************************** */

/* My display tests */

void testDispPixel(bool clear) {
  if (clear) {
    display.clearDisplay();
  }
  display.setCursor(0, 0);     // Start at top-left corner

  // Draw a pixels in white
  display.drawPixel(10, 10, SSD1306_WHITE);
  display.drawPixel(10, 20, SSD1306_WHITE);
  display.drawPixel(10, 30, SSD1306_WHITE);
  display.display();
}

void testRowsText(bool clear) {
  if (clear) {
    display.clearDisplay();
  }
  display.setTextSize(1);
  display.setCursor(0, 0);     // Start at top-left corner
  display.println(F("aaaaaaaaaa"));
  display.println(F("bbbbbbbbbb"));
  display.println(F("MMMMMMMMMM"));
  display.println(F("iiiiiiiiii"));
  display.println(F("zzzzzzzzzz"));
  display.display();
}

void testPrintLineWrap(bool clear) {
  if (clear) {
    display.clearDisplay();
  }
  display.setTextSize(1);
  display.setCursor(0, 0);     // Start at top-left corner
  display.println(F("0123456789abcdefghij0123456789"));
  display.display();
}

void testPrintLines(bool clear) {
  if (clear) {
    display.clearDisplay();
  }
  display.setTextSize(1);
  display.setCursor(0, 0);     // Start at top-left corner
  display.println(F("lorem ipsum delorias "));
  display.setCursor(0, 14);     // Start at top-left corner
  display.println(F("il rega deus mastochi"));
  display.setCursor(0, 27);     // Start at top-left corner
  display.println(F("aggregate nose handle"));
  display.display();
}

void testPrintPositions(bool clear) {
  if (clear) {
    display.clearDisplay();
  }
  display.setTextSize(1);
  display.setCursor(0, 0);     // Start at top-left corner
  display.print(F("lorem"));
  display.setCursor(12, 8);     // Start at top-left corner
  display.println(F("ipsum"));
  display.setCursor(42, 16);     // Start at top-left corner
  display.println(F("bitches"));
  display.setCursor(84, 24);     // Start at top-left corner
  display.println(F("~~!!!~~"));
  display.display();
}

/**
 * @brief print 
 * */
void testPrintExtraBig(bool clear) {
  if (clear) {
    display.clearDisplay();
  }
  display.setTextSize(3);
  display.setCursor(0, 0);     // Start at top-left corner
  display.println(F("chicken"));
  display.setTextSize(1);
  display.println(F("legs thighs & breasts"));
  display.display();
}


void testPrintBigLines(bool clear) {
  if (clear) {
    display.clearDisplay();
  }
  display.setTextSize(2);
  display.setCursor(0, 0);     // Start at top-left corner
  display.println(F("flugelhorn"));
  display.setTextSize(2);
  display.println(F("_buttress"));
  display.display();
}

/*  Below were imported from example */

void testdrawrect(bool clear) {
  if (clear) {
    display.clearDisplay();
  }

  for(int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}
