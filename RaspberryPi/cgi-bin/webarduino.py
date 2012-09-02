#!/usr/bin/env python
print "Content-Type: text/plain"
print
import sys
sys.path.append("/home/pi/entrepinas/")
import arduino
import web
#import serial;
#ser = serial.Serial('/dev/ttyACM0', 9600);
#ser.write(chr(1));
#t1 = ser.readline();

ACCION_LECTURA="lec"
ACCION_COMANDO="com"
urls = (
    '/(.*)', 'webarduino'
)
app = web.application(urls, globals())

class webarduino: 


	def GET(self,name): 
		param=web.input()
		salida = "No se que accion es"
		if param.action == ACCION_LECTURA:
			salida = arduino.getValor(param.sensor,1)
		elif param.action == ACCION_COMANDO:
			salida = arduino.ejecuta(param.com)
			
		print salida				

if __name__ == "__main__":
    app.run()
