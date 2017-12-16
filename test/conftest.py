import pytest
import serial

@pytest.fixture
def serialport(scope="function"):
    print("")
    sp = '/dev/ttyUSB0'
    ser = serial.Serial(sp, baudrate=9600, timeout=3)
    ser.flush()
    print("Flushing {}...".format(sp))
    rec = ser.read(20)
    print("Done flusing {}".format(sp))
    yield ser
    print("Closing {}".format(sp))
    ser.close()
     
