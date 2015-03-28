// include the library code:
#include <Wire.h>
#include <Adafruit_MCP23017.h>

#include "KeyScanner.h"

#define LED_PIN0 8
#define LED_PIN1 9

const byte KEY_ROWS0 = 8;
const byte KEY_COLS0 = 6;

const uint8_t key_row0[KEY_ROWS0] = { 8, 9, 10, 11, 12, 13, 14, 15 };
const uint8_t key_col0[KEY_COLS0] = { 2, 3, 4, 5, 6, 7 };

const char key_values0[KEY_ROWS0 * KEY_COLS0] = {
  /* left hand side */
  KEY_ESC, '1', '2', '3', '4', '5',
  KEY_TAB, 'q', 'w', 'e', 'r', 't',
  KEY_CAPS_LOCK, 'a', 's', 'd', 'f', 'g',
  KEY_RIGHT_SHIFT, 'z', 'x', 'c', 'v', 'b',
  /* right hand side */
  '6', '7', '8', '9', '0', KEY_BACKSPACE,
  'y', 'u', 'i', 'o', 'p', '\\',
  'h', 'j', 'k', 'l', ';', KEY_RETURN,
  'n', 'm', ',', '.', '/', '\'' 
};

KeyScanner scanner0(&key_row0[0], KEY_ROWS0, &key_col0[0], KEY_COLS0, &key_values0[0]);

const byte KEY_ROWS1 = 6;
const byte KEY_COLS1 = 7;

const uint8_t key_row1[KEY_ROWS1] = { 8, 9, 10, 11, 12, 13 };
const uint8_t key_col1[KEY_COLS1] = { 4, 6, 7, 5, 3, 2, 1 };


const char key_values1[KEY_ROWS1 * KEY_COLS1] = {
  0, 0, KEY_F1, KEY_F2, KEY_HOME, KEY_UP_ARROW, KEY_PAGE_UP,
  '-', '=', KEY_F3, KEY_F4, KEY_LEFT_ARROW, 0, KEY_RIGHT_ARROW,
  '[', ']', KEY_F5, KEY_F6, KEY_END, KEY_DOWN_ARROW, KEY_PAGE_DOWN,
  0, 0, 0, KEY_ESC, KEY_INSERT, KEY_RIGHT_GUI, KEY_DELETE,
  0, KEY_LEFT_SHIFT, KEY_LEFT_CTRL, KEY_LEFT_ALT, KEY_RIGHT_ALT, KEY_RIGHT_CTRL, KEY_RIGHT_SHIFT,
  0, 0, ' ', 0, 0, ' ', 0
};


/* Use this for testing since there are so many
non-character keys on the front panel */
/*
const char key_values1[KEY_ROWS1 * KEY_COLS1] = {
  '0', '1', '2', '3', '4', '5', '6',
  '7', '8', '9', 'a', 'b', 'c', 'd',
  'e', 'f', 'g', 'h', 'i', 'j', 'k',
  'l', 'm', 'n', 'o', 'p', 'q', 'r', 
  's', 't', 'u', 'v', 'w', 'x', 'y',
  'z', '`', ',', '.', '/', ';', '\'',
};
*/

KeyScanner scanner1(&key_row1[0], KEY_ROWS1, &key_col1[0], KEY_COLS1, &key_values1[0]);

void setup()
{
  Keyboard.begin();
  pinMode(LED_PIN0, OUTPUT);
  digitalWrite(LED_PIN0, LOW);
  pinMode(LED_PIN1, OUTPUT);
  digitalWrite(LED_PIN1, LOW);
  
  scanner0.begin(0);
  scanner1.begin(1);
}

void loop()    
{
    digitalWrite(LED_PIN0, scanner0.scanKeys() ? HIGH : LOW);
    digitalWrite(LED_PIN1, scanner1.scanKeys() ? HIGH : LOW);
} 


