#include <Keyboard.h> // Pastikan HID sudah aktif

// Definisi jumlah baris dan kolom
#define ROWS 5
#define COLS 14
#define KEY_PRINT_SCREEN  0xCE
#define KEY_MENU          0xED // "Keyboard Application" in USB standard

// Pin baris dan kolom
byte rowPins[ROWS] = {PA0, PA1, PA2, PA3, PA4};
byte colPins[COLS] = {PA5, PA6, PA7, PB0, PB1, PB3, PB10, PB11, PB12, PB13, PB14, PB15, PC13, PC14};

// Layout keyboard
char keys[ROWS][COLS] = {
  {'`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', KEY_BACKSPACE},
  {KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']','\\'},
  {KEY_CAPS_LOCK, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '\0', KEY_RETURN},
  {KEY_LEFT_SHIFT, 'z','x', 'c', 'v', 'b', 'n', 'm', ',' , '.', '/', '\0', KEY_RIGHT_SHIFT, '\0'},
  {KEY_LEFT_CTRL, KEY_LEFT_GUI, KEY_LEFT_ALT, '\0', '\0', ' ', '\0', '\0', KEY_RIGHT_ALT, '\f', KEY_MENU, '\0', KEY_RIGHT_CTRL, '\0'}
};

// Pemetaan angka ke tombol fungsi F1-F12
const uint8_t functionKeys[12] = {
  KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12
};

// Pemetaan angka ke tombol fungsi kanan, kiri, atas, bawah
const uint8_t functionKeysArrow[4] = {
  KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW, KEY_LEFT_ARROW
};

// pemetaan angka untuk tombol fungsi tombol lain
const uint8_t functionKeysOperate[8] = {
  KEY_ESC, KEY_INSERT, KEY_DELETE, KEY_PAGE_UP, KEY_PAGE_DOWN, KEY_HOME, KEY_END, KEY_PRINT_SCREEN
};

// Untuk menghindari input berulang
bool keyStates[ROWS][COLS] = {0}; // 0 = released, 1 = pressed
bool modifierPressed = false;    // Untuk mencatat status tombol \f

void setup() {
  // Inisialisasi pin baris sebagai OUTPUT
  for (byte i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
  }

  // Inisialisasi pin kolom sebagai INPUT_PULLUP
  for (byte i = 0; i < COLS; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }

  // Mulai komunikasi HID Keyboard
  Keyboard.begin();  // Mulai komunikasi HID Keyboard
}

// Fungsi untuk mengirim tombol ke komputer
void sendKey(char key) {
  if (key >= 0x80) { // Tombol spesial (KEY_*)
    Keyboard.press(key);
    delay(50);
  } else {
    Keyboard.press(key);
    delay(50);
  }
}

// Fungsi untuk melepas tombol
void releaseKey(char key) {
  if (key >= 0x80) { // Tombol spesial (KEY_*)
    Keyboard.release(key);
    delay(50);
  } else {
    Keyboard.release(key);
    delay(50);
  }
}

void loop() {
   // Loop untuk memindai baris dan kolom
  for (byte row = 0; row < ROWS; row++) {
    // Aktifkan baris (turunkan menjadi LOW)
    digitalWrite(rowPins[row], LOW);

    // Baca status setiap kolom
    for (byte col = 0; col < COLS; col++) {
      bool isPressed = !digitalRead(colPins[col]); // LOW berarti tombol ditekan
      bool shiftPressed = false;

      // Cek apakah tombol Shift ditekan
      if (keys[row][col] == KEY_LEFT_SHIFT || keys[row][col] == KEY_RIGHT_SHIFT) {
        shiftPressed = isPressed; // Update status Shift
      }

      // Update status tombol modifier (\f)
      if (keys[row][col] == '\f') {
        modifierPressed = isPressed;
      }

      if (isPressed && !keyStates[row][col]) {
        // Tombol ditekan
        keyStates[row][col] = true;

        if(shiftPressed && keys[row][col] == '=') {
          sendKey('+');
          releaseKey('+');
        } else if (modifierPressed && keys[row][col] >= '1' && keys[row][col] <= '9'){
          uint8_t index = keys[row][col] - '1';
          sendKey(functionKeys[index]);
          releaseKey(functionKeys[index]);
        } else if (modifierPressed && keys[row][col] == '0'){
          sendKey(functionKeys[9]);
          releaseKey(functionKeys[9]);
        } else if ( modifierPressed && keys[row][col] == '-'){
          sendKey(functionKeys[10]);
          releaseKey(functionKeys[10]);
        } else if ( modifierPressed && keys[row][col] == '=') {
          sendKey(functionKeys[11]);
          releaseKey(functionKeys[11]);
        } else if ( modifierPressed && keys[row][col] == 'w') {
          sendKey(functionKeysArrow[0]);
          releaseKey(functionKeysArrow[0]);
        } else if ( modifierPressed && keys[row][col] == 's') {
          sendKey(functionKeysArrow[1]);
          releaseKey(functionKeysArrow[1]);
        } else if ( modifierPressed && keys[row][col] == 'd') {
          sendKey(functionKeysArrow[2]);
          releaseKey(functionKeysArrow[2]);
        } else if ( modifierPressed && keys[row][col] == 'a') {
          sendKey(functionKeysArrow[3]);
          releaseKey(functionKeysArrow[3]);
        } else if ( modifierPressed && keys[row][col] == '`') {
          sendKey(functionKeysOperate[0]);
          releaseKey(functionKeysOperate[0]);
        } else if ( modifierPressed && keys[row][col] == '.') {
          sendKey(functionKeysOperate[1]);
          releaseKey(functionKeysOperate[1]);
        } else if ( modifierPressed && keys[row][col] == '/') {
          sendKey(functionKeysOperate[2]);
          releaseKey(functionKeysOperate[2]);
        } else if ( modifierPressed && keys[row][col] == ';') {
          sendKey(functionKeysOperate[3]);
          releaseKey(functionKeysOperate[3]);
        } else if ( modifierPressed && keys[row][col] == '\'') {
          sendKey(functionKeysOperate[4]);
          releaseKey(functionKeysOperate[4]);
        } else if ( modifierPressed && keys[row][col] == '[') {
          sendKey(functionKeysOperate[5]);
          releaseKey(functionKeysOperate[5]);
        } else if ( modifierPressed && keys[row][col] == ']') {
          sendKey(functionKeysOperate[6]);
          releaseKey(functionKeysOperate[6]);
        } else if ( modifierPressed && keys[row][col] == 'p') {
          sendKey(functionKeysOperate[7]);
          releaseKey(functionKeysOperate[7]);
        } else {
          sendKey(keys[row][col]);  
        }

      } else if (!isPressed && keyStates[row][col]) {
        // Tombol dilepas
        keyStates[row][col] = false;
        releaseKey(keys[row][col]);
      }
    }

    // Nonaktifkan baris
    digitalWrite(rowPins[row], HIGH);
  }
 
}
