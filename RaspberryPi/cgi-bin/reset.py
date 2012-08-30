#!/usr/bin/env python
print "Content-Type: text/html"
print
import sys
sys.path.append("/home/pi/entrepinas/")
import arduino
#import serial;
#ser = serial.Serial('/dev/ttyACM0', 9600);
#ser.write(chr(1));
#t1 = ser.readline();

arduino.reset()


print """\
<html>
<head><title>Reset </title></head>
<body>
<h2>Reseteado</h2>

</body>
</html>
"""
