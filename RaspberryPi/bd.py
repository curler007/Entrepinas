import MySQLdb # para o MySQL
import datetime

def getConn():
        con = MySQLdb.connect("127.0.0.1","root","anopeludo","entrepinas")
        return con
	


def accedoDispositivo(dispositivo):
	con = getConn()
	Cursor = con.cursor()
	res = Cursor.execute('update bloqueos set usando=1 where id=' + str(dispositivo) + ' and usando=0')
	con.commit()
	con.close()
	return res

def sueltoDispositivo(dispositivo):
	con = getConn()
	Cursor = con.cursor()
	Cursor.execute('update bloqueos set usando=0 where id=' + str(dispositivo) + ' ');
	con.commit()
	con.close()

def almacenaDato(sensor,valor):
	con = getConn()
	Cursor = con.cursor()
	sql = "Insert into data (fecha,sensor_id,valor) values (now(), " + str(sensor) + ", " + str(valor) + ")"
	Cursor.execute(sql)
	con.commit()
	con.close() 

def getDatos(fecha):
	con = getConn()
	cursor = con.cursor()
	sql = ""
	if fecha == None:
		sql = "select fecha, sensor_id, valor from data"
	else:
		sql = "select fecha, sensor_id, valor from data where fecha > " + fecha
	cursor.execute(sql)
	res = cursor.fetchall()
	con.close()
	return res
