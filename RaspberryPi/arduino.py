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

def writeSerial(com):
	ser = getSerial()
	retorno = "Error con el comando: " + str(com)
	try:
		ser.flush()
		i =0
		dato=''
		while dato=='' and i<intentos:
			ser.write(com)
			dato = ser.readline()
			i = i+1
		retorno = "Comando ejecutado correctamente: " + str(dato)
	finally:
		ser.close()
	return retorno
	

def readSerial(sensor):
	logging.info('Declaro el serial')
	
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
			

			ser.write(sensor)

			logging.info('Leo el dato')
			#println('dato = ser.readline()')
			dato = ser.readline()
			logging.info('Dato leido: ' + dato)
			if dato=='':
				logging.info('El dato es nulo, vuelvo a repetir hasta 5 veces')
			i=i+1
		#indice = 0 
		#salir = False
		#datored = ''
		
		#while (indice < len(dato)) and not salir: 
		#	datored = datored + dato[indice]
		#	if (dato[indice]=='.'):
		#		i=0
		#		for i in range(2):
		#			datored = datored + dato[indice+i+1]
		#		salir = True
		#	indice = indice + 1 
		#datored = dato[:6]
		#fdatored = float(int(float(datored)*100)/100)
		fdatored = float(dato)
	finally:
		ser.close()
	
	return fdatored;
	#return float(datored)

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

def ejecuta(com):
	retorno = "Error llamando a ejecuta " + str(com)
	if bd.accedoDispositivo(1) == 1:
		try:
			retorno = writeSerial(com)
		finally:
			bd.sueltoDispositivo(1)
	return retorno



def reset():
	#ser = serial.Serial('/dev/ttyACM0', 9600)
	#ser.write("1\n")
	bd.sueltoDispositivo(1)
	
