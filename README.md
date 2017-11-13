# hades-alternative-firmware
Alternative firmware for Hades MIDI to CV circuit.

--- DISCLAIMER ---

This is just a hobby project and I'm not affiliated with the company
behind the synth in any way.

I don't own the Hades myself, this firmware is tested with the midi to cv
circuit (from the provided schematics) on breadboard only.
This means that it *might* work, but you will have to find that out yourself :)

No support is provided for this project. The implementation is really simple and
many features are probably missing. Feel free to improve it as you like!

If you want to try it, I strongly recommend that you either dump and save the original
firmware, or flash this firmware on a separate atmega328 and keep the original.

--- END DISCLAIMER ---

--- USAGE ---

No hardware modifications are required.

Default note priority is last note played (also currently the only implemented note priority).

Default midi channel is 1.

Midi channel 16 is used for configuring different modes. Notes c to b (any octave) are used to set
a mode.

Curently, the implemented modes are:

  c  - Note priority last played
  c# - midi channel learn

Settings are stored in eeprom and will be restored on startup.

Example, set midi channel 2:

  1. On channel 16, press c# to enter midi channel learn mode (three short pulses will be seen on the gate output).
  2. On channel 2, send any note on (three short pulses will be seen again).
  3. On channel 16, press c to enter mode "Note priority last played" again.
  4. The device should now listen on channel 2.
