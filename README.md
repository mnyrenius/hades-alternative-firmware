# hades-alternative-firmware
Alternative firmware for Hades MIDI to CV circuit.

--- DISCLAIMER ---

This is just a hobby project and I'm not affiliated with the company
behind the synth in any way. The purpose of this project is mainly to
have fun an learn stuff. If you're looking for something really stable
with perfect MIDI implementation, I would recommend to look into porting
a more mature open source project to work with the Hades.

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

Default midi channel is 1.

Midi channel 16 is used for configuring different modes. Notes c to b (any octave) are used to set
a mode.

Curently, the implemented modes are:

  c  - Note priority last played

  c# - Midi channel learn

  d  - Note priority high

  e  - Note priority low

  f  - Turing machine

Settings are stored in eeprom and will be restored on startup.

--- MIDI LEARN MODE ---

Example, set midi channel 2 and set C2 as the base note that should output 0V:

  1. On channel 16, press c# to enter midi channel learn mode.
  2. On channel 2, press C3 (three short pulses will be seen on gate output).
  3. On channel 16, press c to enter mode "Note priority last played" again.
  4. The device should now listen on channel 2 and C2 should output 0V.


--- TURING MACHINE MODE ---

This mode mimics the eurorack module by Music Thing Modular.

It's clocked by external MIDI sync.

c  - turn random knob clockwise (bit invert probability [1/2, 1/4, off, 1/8, 1/16])

c# - turn random knob counter-clockwise

d  - turn length knob clockwise ([2, 3, 4, 5, 8, 12, 16])

d# - turn length knob counter-clockwise

f  - turn range knob clockwise

f# - turn range knob counter-clockwise

