import sys
sys.path.append("/home/pi/entrepinas/")
import arduino
import bd

s1 = arduino.getValor(1)
s2 = arduino.getValor(2)
if s1 != '-1':
	bd.almacenaDato(1,s1)
	print "Almacenado	T1: " + s1
if s2 != '-1':
	bd.almacenaDato(2,s2)
	print "Almacenado	T2: " + s2
