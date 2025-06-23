#ifndef PINSETTINGS_H
#define PINSETTINGS_H

// Paddle input
#define dotPin A0               // Analog pin for detecting Morse “dot” input (short press)
#define dashPin A1              // Analog pin for detecting Morse “dash” input (long press)

// Potentiometer
#define wpmPotPin A2            // Analog pin connected to the potentiometer for adjusting WPM
#define HzPotPin A3             // Analog pin connected to the potentiometer for adjsuting Hz

// Sound Outputs
#define buzzerPin 8             // Digital pin driving the buzzer for audio output

// LEDs
#define morseLEDPin 10          // Digital pin controlling the LED that blinks for Morse output
#define powerLEDPin 9           // Digital pin for the power-status indicator LED

// Buttons
#define functionButtonPin 7     // Digital pin connected to the function-selector button
#define clearButtonPin 6        // Digital pin connected to the “clear” button (resets buffers)

#endif