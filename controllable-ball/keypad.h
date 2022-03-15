#ifndef KEYPAD_DOT_H
#define KEYPAD_DOT_H

#include <stdbool.h>

// Setup the io stream for a keypad. If the agrument bool is false then it uses pin 7-0
void KeypadInit(bool usePin15To8);

// Activates a row for input on the keypad, also updates the out-register. Index starts at 1
void KeypadActivateRow(unsigned int rowIndex);

// Returns column data of pressed keys in the active row
unsigned char KeypadReadCol();

// Returns a keycode for the first button pressed (closest to top right).
// The keycode is comprised of a byte where the first half byte represents the row and second the column
unsigned char KeypadGetKeycode();

#endif /* KEYPAD_DOT_H */