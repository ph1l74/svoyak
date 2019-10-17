import sys
import glob
import serial
import threading
import time # Optional (if using time.sleep() below)

def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result


def string_handler(incoming_string):
    print(incoming_string)
    splitted_string = incoming_string.replace('\r\n', '').split(':')
    message_type = splitted_string[0]
    message_parameter = splitted_string[1]
    message_values = splitted_string[2:]
    message = {"type": message_type,
               "parameter": message_parameter,
               "values": message_values}
    return message


def message_handler(message):
    print(message)


def start_session(port, baud_rate=9600):
    usb_port = serial.Serial(port, baud_rate, timeout=1)
    while True:
        if usb_port.in_waiting > 0:
            data_str = usb_port.readline(usb_port.in_waiting).decode('ascii')
            if len(data_str) > 0:
                incoming_message = string_handler(data_str)
                message_handler(incoming_message)
            time.sleep(0.01)


start_session('COM5')
