/*
 * QLab remote code.
 *
 * Basically, a USB keyboard with only 4 keys that you can use
 * to control QLab for sound cues.
 *
 * Uses a Teensy LC (https://www.pjrc.com/store/teensylc.html)
 *
 * Full instructions: https://github.com/andyw-lala/qlab-remote
 *
 * You must select Keyboard from the "Tools > USB Type" menu
 *
 * Originally based on Teensy example code.
 */

/*
 * Early POC work done on a Teensy 3.6.
 */

#include <Bounce.h>

/*
 * Pins used.
 * Note the output pins used are the 20mA capable pins, if you
 * are going to keep the LEDs under program control, you should
 * continue to use these pins.
 */
const int go_input = 0;
const int stop_input = 1;
const int fwd_input = 2;
const int rew_input = 3;

const int go_led = 5;
const int stop_led = 16;
const int fwd_led = 17;
const int rew_led = 21;

/*
 * Debounce time used, if the buttons you use are noiser, you may want to
 * increase this value.
 */
#define DEBOUNCE_MS	(10)

/*
 * Bounce objects for each button.
 */
Bounce go_button   = Bounce(go_input, DEBOUNCE_MS);
Bounce stop_button = Bounce(stop_input, DEBOUNCE_MS);
Bounce fwd_button  = Bounce(fwd_input, DEBOUNCE_MS);
Bounce rew_button  = Bounce(rew_input, DEBOUNCE_MS);

void setup() {
  /*
   * Configure the button inputs with pullups, and the LED output
   * pins as outputs.
   *
   * TODO: Configure all other pins as outputs for minimum noise.
   */
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  pinMode(go_led, OUTPUT);
  pinMode(stop_led, OUTPUT);
  pinMode(fwd_led, OUTPUT);
  pinMode(rew_led, OUTPUT);

  pinMode(13, OUTPUT);	/* Debug, on-board LED */
}

void loop() {
  static int led = 0;

  /*
   * TODO: LED blinky pattern code goes here.
   */

  /*
   * Call the update() method for all buttons.
   */
  go_button.update();
  stop_button.update();
  fwd_button.update();
  rew_button.update();
  

  /*
   * Onlr react to falling edges of buttons (they are active low.)
   */
  if (go_button.fallingEdge()) {
//  Keyboard.println("B0 press");
    led = 1;
    digitalWrite(go_led, 1);
  }
  if (stop_button.fallingEdge()) {
//  Keyboard.println("B1 press");
    led = 0;
    digitalWrite(stop_led, 1);
  }
  if (fwd_button.fallingEdge()) {
//  Keyboard.println("B2 press");
    led ^= 1;
    digitalWrite(fwd_led, 1);
  }
  if (rew_button.fallingEdge()) {
//  Keyboard.println("B3 press");
    if (led == 0) {
      led = 1;
    } else {
      led = 0;
    }
    digitalWrite(rew_led, 1);
  }
  digitalWrite(13, led);
}
