import sys
import time
sys.path.append("/home/pi/entrepinas/")
import bd
import serial
import logging
intentos = 5
LOG_FILENAME = '/home/pi/entrepinas/log/log.out'
logging.basicConfig(filename=LOG_FILENAME,level=logging.INFO,)

def getSerial():
	try:
		ser = serial.Serial('/dev/ttyACM0',9600,timeout=3)
	except Exception, e:
		ser = serial.Serial('/dev/ttyACM1',9600,timeout=3)
	ser.open()
	#println('Creo nuevo serial, ser.open()')
	return ser

def readSerial(sensor):
	logging.info('Declaro el serial')
	#ser = serial.Serial('/dev/ttyACM0', 9600,timeout=10)
	ser = getSerial()
	try:
		logging.info('Escribo a Arduino pidiendole un puerto')
		#println('Escribo a Arduino pidiendole un puerto')
		#println('ser.flush()')
		ser.flush()
		#print('ser.write(chr(sensor)), sensor:')
		#println(str(sensor))
		i=0
		dato=''
		while dato=='' and i<intentos:
			
			ser.write(chr(sensor))

			logging.info('Leo el dato')
			#println('dato = ser.readline()')
			dato = ser.readline()
			logging.info('Dato leido: ' + dato)
			if dato=='':
				logging.info('El dato es nulo, vuelvo a repetir hasta 5 veces')
			i=i+1
		datored = dato[:5]
		
	finally:
		ser.close()
	
	return datored

def getValor(sensor):
	logging.info('Entro en getValor, voy a comprobar el bloqueo del dispositivo')
	if bd.accedoDispositivo(1) == 1:
		try:
			logging.info('Dispositivo libre, vy a acceder a el')
			v = readSerial(sensor)
			logging.info('Voy a soltar el bloqueo')
		finally:
			bd.sueltoDispositivo(1)
		logging.info('He soltado el bloqueo')
		return v	
	else:
		logging.info('Dispositivo bloqueado!')
		return -1

def reset():
	ser = serial.Serial('/dev/ttyACM0', 9600)
	ser.write(chr(1))
	bd.sueltoDispositivo(1)
	
