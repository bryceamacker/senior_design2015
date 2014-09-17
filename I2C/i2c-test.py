import smbus
import time
import sys

address = 0x18
bus = smbus.SMBus(1)
response = -1

while True:
    decision = raw_input("(T)ransmit or (R)ecieve: ")

    if (decision.upper() == "T"):
        num = raw_input("Send a number 0-9: ")
        try:
            bus.write_byte(address, int(num))
        except:
            print "Unexpected write error:", sys.exc_info()[0]

        time.sleep(0.005)

    elif (decision.upper() == "R"):
        try:
            response = bus.read_byte(address)
        except:
            print "Unexpected read error:", sys.exc_info()[0]

        time.sleep(0.005)
        print(response)

    else:
        print("Not a valid decision")
