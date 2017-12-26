import pytest
import serial
import struct
import time

modes = { "midilearn": (1, b'\x03'),
          "prio_last": (0, b'\x00'),
          "prio_hi"  : (2, b'\x01'),
          "prio_lo"  : (4, b'\x02'),
          "turing"   : (5, b'\x04'), }

###############################################################################
# TEST CASES

def test_mode_last(serialport):
    set_mode(serialport, "midilearn")
    note_on(serialport, 0, 0)
    set_mode(serialport, "prio_last")

    note_on(serialport, 0, 0)
    check_cv_gate(serialport, 0, True)
    note_on(serialport, 12, 0)
    check_cv_gate(serialport, 819, True)
    note_on(serialport, 24, 0)
    check_cv_gate(serialport, 1638, True)
    note_on(serialport, 36, 0)
    check_cv_gate(serialport, 2457, True) 
    note_on(serialport, 48, 0)
    check_cv_gate(serialport, 3276, True)
    note_on(serialport, 60, 0)
    check_cv_gate(serialport, 4095, True)

    note_off(serialport, 12, 0)
    check_cv_gate(serialport, 4095, True)
    note_off(serialport, 0, 0)
    check_cv_gate(serialport, 4095, True)
    note_off(serialport, 24, 0)
    check_cv_gate(serialport, 4095, True)
    note_off(serialport, 60, 0)
    check_cv_gate(serialport, 3276, True)
    note_off(serialport, 48, 0)
    check_cv_gate(serialport, 2457, True)
    note_off(serialport, 36, 0)
    check_cv_gate(serialport, None, False)

def test_mode_midilearn(serialport):
    set_mode(serialport, "midilearn")
    note_on(serialport, 36, 5)
    set_mode(serialport, "prio_last", expected_channel = b'\x05', expected_base_note = b'\x24')

    # verify that it's only listening on the configured channel
    note_on(serialport, 0, 5)
    check_cv_gate(serialport, None, True)
    note_off(serialport, 0, 5)
    check_cv_gate(serialport, None, False)
    note_on(serialport, 0, 4)
    check_noting_received(serialport)
 
    set_mode(serialport, "midilearn")
    note_on(serialport, 0, 4)
    set_mode(serialport, "prio_last", expected_channel = b'\x04', expected_base_note = b'\x00')
    note_on(serialport, 0, 4)
    check_cv_gate(serialport, None, True)

def test_turing(serialport):
    set_mode(serialport, "midilearn")
    note_on(serialport, 0, 0)
    set_mode(serialport, "turing")

    # not started - no output expected
    for j in range(0, 6):
            rt_clock(serialport)
            time.sleep(0.01)
    check_noting_received(serialport)

    # start - expect output
    rt_start(serialport);
    expect_gate = True

    rt_clock(serialport)
    check_cv_gate(serialport, None, expect_gate)
    expect_gate = not expect_gate

    for i in range(0,64):
        for j in range(0, 6):
            rt_clock(serialport)
            time.sleep(0.01)
        check_cv_gate(serialport, None, expect_gate)
        expect_gate = not expect_gate

    # stopped - no output expected
    rt_stop(serialport)
    for j in range(0, 6):
            rt_clock(serialport)
            time.sleep(0.01)
    check_noting_received(serialport)

###############################################################################
# HELPER FUNCTIONS

def check_cv_gate(ser, expected_cv, expected_gate):
    rec = ser.read(3)
    val = struct.unpack("<H?", rec)
    print("Received cv = {} gate = {}".format(val[0], val[1]))
    if expected_cv is not None:
        assert val[0] == expected_cv
    if expected_gate is not None:
        assert val[1] == expected_gate

def check_start_msg(ser, expected_mode, expected_channel, expected_base_note):
    rec = ser.read(4)
    vals = struct.unpack("<cccc", rec)
    assert vals[0] == b'\xb6'
    print("Received start msg: {} mode: {} channel: {} base note: {}".format(vals[0], vals[1], vals[2], vals[3]))

    if expected_mode is not None:
        assert vals[1] == expected_mode
    if expected_channel is not None:
        assert vals[2] == expected_channel
    if expected_base_note is not None:
        assert vals[3] == expected_base_note

def check_noting_received(ser):
    rec = ser.read(10)
    assert len(rec) == 0, "Unexpected data received: {}".format(rec)
 
def note_on(ser, note, channel):
    cmd = (144 + channel).to_bytes(1, byteorder='little') + note.to_bytes(1, byteorder='little') + b"\x7f"
    assert ser.write(cmd) == 3
    time.sleep(0.1)
 
def note_off(ser, note, channel):
    cmd = (128 + channel).to_bytes(1, byteorder='little') + note.to_bytes(1, byteorder='little') + b"\x7f"
    assert ser.write(cmd) == 3
    time.sleep(0.1)

def rt_start(ser):
    assert ser.write(b'\xfa') == 1

def rt_stop(ser):
    assert ser.write(b'\xfc') == 1

def rt_continue(ser):
    assert ser.write(b'\xfb') == 1

def rt_clock(ser):
    assert ser.write(b'\xf8') == 1
 
def set_mode(ser, mode, expected_channel=None, expected_base_note=None):
    print("Setting mode: {}".format(mode))
    note_on(ser, modes[mode][0], 15)
    check_start_msg(ser, modes[mode][1], expected_channel, expected_base_note)
 
