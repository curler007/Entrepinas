#!/usr/bin/env python
print "Content-Type: text/html"
print
import sys
sys.path.append("/home/pi/entrepinas/")
import arduino
import datetime
#import serial;
#ser = serial.Serial('/dev/ttyACM0', 9600);
#ser.write(chr(1));
#t1 = ser.readline();
tiempo1=datetime.datetime.now();
t1 = arduino.getValor(1);
tiempo2=datetime.datetime.now();
t2 =float( arduino.getValor(2));
tiempo3=datetime.datetime.now();

print """\
<html>
<head><title>Entrepinas - Temperaturas </title></head>
<body>
<a href="reset.py">Resetear interface</a> 
<h2>Temperatura actual:</h2>
<ul>
<li>Sensor 1: <b>
"""
print t1
print """ 
&ordm;C</b> Tiempo en obtenerlo: 
"""
print tiempo2-tiempo1
print """
</li>
<li>Sensor 2: <b>
"""
print t2
print """   
&ordm;C</b> Tiempo en obtenerlo: 
"""
print tiempo3-tiempo2
print """
</li>
</ul> 
</body>
</html>
"""
