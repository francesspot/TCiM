#include <Wire.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1   

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===== DUŻA BITMAPA 64x64 =====
#define LOGO1_HEIGHT 64
#define LOGO1_WIDTH  64

static const unsigned char PROGMEM logo1[] = {
  // (Twoja pierwsza bitmapa – skrócona dla czytelności)
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  // ... (TU WSTAW CAŁĄ BITMAPĘ 64x64 BEZ ZMIAN)
};

// ===== MAŁA BITMAPA 32x32 (DO ANIMACJI) =====
#define LOGO2_HEIGHT 32
#define LOGO2_WIDTH  32

static const unsigned char PROGMEM logo2[] = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  // ... (TU WSTAW DRUGĄ BITMAPĘ 32x32 BEZ ZMIAN)
};

// ===== ANIMACJA =====
#define NUMFLAKES 5
#define XPOS 0
#define YPOS 1
#define DELTAY 2

void setup() {
  Serial.begin(9600);

  if(!display.begin(i2c_Address, true)) {
    Serial.println(F("Nie znaleziono wyswietlacza SH1106"));
    for(;;);
  }

  display.clearDisplay();

  // ===== 1. DUŻA BITMAPA =====
  display.drawBitmap(32, 0, logo1, LOGO1_WIDTH, LOGO1_HEIGHT, SH110X_WHITE);
  display.display();
  delay(4000);

  // ===== 2. TEKST =====
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);

  display.setCursor(15, 5);
  display.println("WESOLYCH");

  display.setCursor(30, 25);
  display.println("SWIAT");

  display.setCursor(15, 45);
  display.println("WIELKANOCY");

  display.display();
  delay(5000);

  // ===== 3. ANIMACJA =====
  display.clearDisplay();
  testdrawbitmap(logo2, LOGO2_WIDTH, LOGO2_HEIGHT);
}

void loop() {
  // nic – animacja działa w funkcji
}

// ===== FUNKCJA ANIMACJI =====
void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];

  // inicjalizacja
  for (uint8_t f = 0; f < NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;
  }

  while (1) {
    // rysuj
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SH110X_WHITE);
    }
    display.display();
    delay(200);

    // kasuj i przesuwaj
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SH110X_BLACK);
      icons[f][YPOS] += icons[f][DELTAY];

      if (icons[f][YPOS] > display.height()) {
        icons[f][XPOS] = random(display.width());
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }
  }
}
