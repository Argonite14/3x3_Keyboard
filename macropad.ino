#include <HID-Project.h>
#include <HID-Settings.h>

// =====================
// MATRIX CONFIG
// =====================
#define ROWS 3
#define COLS 3

int rowPins[ROWS] = {5, 6, 7};
int colPins[COLS] = {2, 3, 4};

// =====================
// STATE TRACKING
// =====================
bool keyState[ROWS][COLS] = {false};
unsigned long pressTime[ROWS][COLS];
unsigned long lastRepeat[ROWS][COLS];

// Hold tuning
const unsigned long HOLD_DELAY  = 300;  // ms before repeat
const unsigned long REPEAT_RATE = 80;   // ms between repeats

// =====================
// FUNCTION DECLARATIONS
// =====================
void handleKeyPress(int row, int col);
void handleKeyHold(int row, int col);

// =====================
// SETUP
// =====================
void setup() {
  for (int r = 0; r < ROWS; r++) {
    pinMode(rowPins[r], OUTPUT);
    digitalWrite(rowPins[r], HIGH);
  }

  for (int c = 0; c < COLS; c++) {
    pinMode(colPins[c], INPUT_PULLUP);
  }

  Keyboard.begin();
  Consumer.begin();
}

// =====================
// MAIN LOOP
// =====================
void loop() {
  for (int r = 0; r < ROWS; r++) {
    digitalWrite(rowPins[r], LOW);

    for (int c = 0; c < COLS; c++) {
      bool pressed = (digitalRead(colPins[c]) == LOW);

      // Key just pressed
      if (pressed && !keyState[r][c]) {
        keyState[r][c] = true;
        pressTime[r][c] = millis();
        lastRepeat[r][c] = millis();
        handleKeyPress(r, c);
      }

      // Key held
      if (pressed && keyState[r][c]) {
        handleKeyHold(r, c);
      }

      // Key released
      if (!pressed && keyState[r][c]) {
        keyState[r][c] = false;
      }
    }

    digitalWrite(rowPins[r], HIGH);
  }

  delay(5); // debounce
}

// =====================
// SINGLE PRESS ACTIONS
// =====================
void handleKeyPress(int row, int col) {

  // ROW 0 – VOLUME
  if (row == 0 && col == 0) {
    Consumer.write(MEDIA_VOLUME_UP);
  }
  if (row == 0 && col == 1) {
    Consumer.write(MEDIA_VOLUME_MUTE);
  }
  if (row == 0 && col == 2) {
    Consumer.write(MEDIA_VOLUME_DOWN);
  }

  // ROW 1 – UTILITIES
  if (row == 1 && col == 0) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press('s');
    delay(50);
    Keyboard.releaseAll();
  }

  if (row == 1 && col == 1) {
    Consumer.write(MEDIA_PLAY_PAUSE);
  }

  if (row == 1 && col == 2) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(50);
    Keyboard.releaseAll();
    delay(200);
    Keyboard.print("calc");
    Keyboard.write(KEY_RETURN);
  }

  // ROW 2 – MEDIA / WINDOW
  if (row == 2 && col == 0) {
    Consumer.write(MEDIA_PREVIOUS);
  }

  if (row == 2 && col == 1) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('d');
    delay(50);
    Keyboard.releaseAll();
  }

  if (row == 2 && col == 2) {
    Consumer.write(MEDIA_NEXT);
  }
}

// =====================
// HOLD ACTIONS
// =====================
void handleKeyHold(int row, int col) {
  unsigned long now = millis();

  // Only volume up/down repeat
  if (row == 0 && (col == 0 || col == 2)) {

    if (now - pressTime[row][col] >= HOLD_DELAY) {

      if (now - lastRepeat[row][col] >= REPEAT_RATE) {
        lastRepeat[row][col] = now;

        if (col == 0) {
          Consumer.write(MEDIA_VOLUME_UP);
        }
        if (col == 2) {
          Consumer.write(MEDIA_VOLUME_DOWN);
        }
      }
    }
  }
}
