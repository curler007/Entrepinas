import sys
import time
sys.path.append("/home/pi/entrepinas/")
import bd
import serial
import logging
intentos = 5
BLOQUEADO = -101
ERROR_LLAMANDO_ARDUINO = -102
LOG_FILENAME = '/home/pi/entrepinas/log/log.out'
logging.basicConfig(filename=LOG_FILENAME,level=logging.INFO,)

def getSerial():
	try:
		ser = serial.Serial('/dev/ttyACM0',9600,timeout=3)
	except Exception, e:
		ser = serial.Serial('/dev/ttyACM1',9600,timeout=3)
	ser.open()

	return ser

def writeSerial(com):
	ser = getSerial()
	retorno = ERROR_LLAMANDO_ARDUINO
	try:
		ser.flush()
		i =0
		dato=''
		while dato=='' and i<intentos:
			ser.write(com)
			dato = ser.readline()
			i = i+1
		if dato!='':
			retorno = str(dato)
	finally:
		ser.close()
	return retorno
	

def readSerial(sensor):
	logging.info('Declaro el serial')
	
	ser = getSerial()
	try:
		logging.info('Escribo a Arduino pidiendole un puerto')
		ser.flush()
	
		i=0
		dato=''
		while dato=='' and i<intentos:
			ser.write(sensor)
			logging.info('Leo el dato')
			dato = ser.readline()
			logging.info('Dato leido: ' + dato)
			if dato=='':
				logging.info('El dato es nulo, vuelvo a repetir hasta 5 veces')
			i=i+1

		fdatored = float(dato)
	finally:
		ser.close()
	
	return fdatored;

def getValor(sensor,reintentar):
	logging.info('Entro en getValor, voy a comprobar el bloqueo del dispositivo')

	if (dameBloqueoDispositivo(reintentar)):
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
		return BLOQUEADO

def ejecuta(com):
	retorno = BLOQUEADO 
	if dameBloqueoDispositivo(0):
		try:
			retorno = writeSerial(com)
		finally:
			bd.sueltoDispositivo(1)
	return retorno


def reset():
	bd.sueltoDispositivo(1)

def dameBloqueoDispositivo(reintentar):
	paso=0
	if(reintentar):
                while(paso==0):
			paso = bd.accedoDispositivo(1)
        elif bd.accedoDispositivo(1) == 1:
                paso=1
	return paso;
