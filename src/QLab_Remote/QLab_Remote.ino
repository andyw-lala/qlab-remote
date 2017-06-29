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

#include <Bounce.h>

/*
 * Pins used.
 * Note the output pins used are the 20mA capable pins, if you
 * are going to keep the LEDs under program control, you should
 * continue to use these pins.
 */
const int go_input = 6;
const int stop_input = 18;
const int fwd_input = 22;
const int rew_input = 14;

const int go_led = 5;
const int stop_led = 17;
const int fwd_led = 21;
const int rew_led = 16;

/*
 * Debounce time used, if the buttons you use are noiser, you may want to
 * increase this value.
 */
#define DEBOUNCE_MS	(20)

/*
 * Bounce objects for each button.
 */
Bounce go_button   = Bounce(go_input, DEBOUNCE_MS);
Bounce stop_button = Bounce(stop_input, DEBOUNCE_MS);
Bounce fwd_button  = Bounce(fwd_input, DEBOUNCE_MS);
Bounce rew_button  = Bounce(rew_input, DEBOUNCE_MS);

/*
 * We flash the LEDs in a pattern on power up, the behaviour is driven
 * by this variable. See blinky(). Once it is zero, the startup greeting
 * is terminated and we revert to normal behaviour.
 */
int greeting;

void setup() {
  /*
   * Configure the button inputs with pullups, and the LED output
   * pins as outputs.
   */
  pinMode(go_input, INPUT_PULLUP);
  pinMode(stop_input, INPUT_PULLUP);
  pinMode(fwd_input, INPUT_PULLUP);
  pinMode(rew_input, INPUT_PULLUP);

  pinMode(go_led, OUTPUT);
  pinMode(stop_led, OUTPUT);
  pinMode(fwd_led, OUTPUT);
  pinMode(rew_led, OUTPUT);

  /*
   * Kick off the Keyboard HID device.
   */
  Keyboard.begin();

  /*
   * Initialise the variable that drives the startup greeting.
   * This should result in 3 iterations of the pattern.
   */
  greeting = 9;
}

/*
 * Generate the startup greeting by blinking the LEDs based on the
 * value of greeting. Decrement it, and be sure to leave all the LEDs
 * on after the pattern is completed (greeting == 0).
 *
 * Just use simple modulo 3 arithmetic to drive the pattern.
 */
void blinky()
{
  /*
   * Figure out which LED(s) to turn on this pass
   */
  switch (greeting-- % 3) {
    case 0:
      digitalWrite(fwd_led, 1);
      digitalWrite(rew_led, 1);
      break;
    case 1:
      digitalWrite(go_led, 1);
      break;
    case 2:
      digitalWrite(stop_led, 1);
      break;
  }

  /*
   * Delay to show the pattern for 0.25 second
   */
  delay(250);

  /*
   * If we are in the middle of the greeting, turn all the LEDs off and delay
   * briefly before the next iteration.
   * If we are done (greeting == 0) - just turn all the LEDs on, we will not
   * be called again.
   */
  if (greeting) {
    digitalWrite(fwd_led, 0);
    digitalWrite(rew_led, 0);
    digitalWrite(stop_led, 0);
    digitalWrite(go_led, 0);
    delay(20);
  } else {
    digitalWrite(fwd_led, 1);
    digitalWrite(rew_led, 1);
    digitalWrite(stop_led, 1);
    digitalWrite(go_led, 1);
  }
}

void loop() {

  /*
   * Call the update() method for all buttons.
   */
  go_button.update();
  stop_button.update();
  fwd_button.update();
  rew_button.update();
  
  /*
   * Generate the startup greeting pattern until that sequence is complete
   * (indicated by greeting == 0)
   */
  if (greeting) {
    blinky();
    return;
  }

  /*
   * Only react to falling edges of buttons (they are active low.)
   * We need to use Keyboard.press() to generate non-ascii keystrokes
   * (up/down arrow) - so we normalise on that for all actions.
   *
   * This requires the use of Keyboard.release() or .releaseAll()
   * to terminate the key press as far as the host is concerned.
   */
  if (go_button.fallingEdge()) {
    digitalWrite(go_led, 1);
    Keyboard.press(' ');
  }
  if (stop_button.fallingEdge()) {
    digitalWrite(stop_led, 1);
    Keyboard.press(KEY_ESC);
  }
  if (fwd_button.fallingEdge()) {
    digitalWrite(fwd_led, 1);
    Keyboard.press(KEY_DOWN_ARROW);
  }
  if (rew_button.fallingEdge()) {
    digitalWrite(rew_led, 1);
    Keyboard.press(KEY_UP_ARROW);
  }

  Keyboard.releaseAll();
}
