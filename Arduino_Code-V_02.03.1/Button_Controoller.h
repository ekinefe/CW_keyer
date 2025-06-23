#ifndef BUTTON_CONTROLLER_H
#define BUTTON_CONTROLLER_H

#include "PinSettings.h"
#include "Globals.h"
#include "Buffer_Cleanner.h"
#include <Arduino.h>
#include "CLI_Debuger.h"

// ----------------------------------------------------------------------------
// Handles the “clear” button press: wipes text and Morse buffers, resets the
// display, outputs debug info, and debounces the input.
// ----------------------------------------------------------------------------

void clearButton(){
    DefaultLocation = "Button_Controoller.h - clearButton";
    DefaultMessage = "'CleanBuffer' function is calling.";
    CLIDebuger();
    CleanBuffer();
    DefaultMessage = "'CleanBuffer' function is called.";
    CLIDebuger();
}

// void modeButton() {

// }

#endif
