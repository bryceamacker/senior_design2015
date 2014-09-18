import smbus
import time
import sys

address = 0x18

pic1Address = 0x20
pic2Address = 0x18

bus = smbus.SMBus(1)
response = -1


def sendNum(address, num):
    try:
        bus.write_byte(address, int(num))
    except:
        print "Unexpected write error: ", sys.exc_info()[0]


def readNum(address):
    try:
        response = bus.read_byte(address)
    except:
        print "Unexpected read error:", sys.exc_info()[0]

    return response


while True:
    both = False

    decision = raw_input("(T)ransmit or (R)ecieve: ")
    picChoice = raw_input("Pic (1) or Pic (2) or (b)oth: ")
    num = raw_input("Send a number 0-255: ")

    if picChoice.lower() == "b":
        both = True
    elif int(picChoice) == 1:
        address = pic1Address
    elif int(picChoice) == 2:
        address = pic2Address

    try:
        if both is True:
            bus.write_byte(0x18, int(num))
            bus.write_byte(0x20, int(num))
        else:
            bus.write_byte(address, int(num))
    except:
        print "Unexpected write error:", sys.exc_info()[0]

    time.sleep(0.005)

    if (int(num) == 5):
        print ("Pic " + picChoice + " is blinking.")
        finished = False

        while not finished:
            response = readNum(address)
            if response == 255:
                finished = True
                print("Done blinking")

            time.sleep(1)
