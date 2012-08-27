import MySQLdb # para o MySQL
import datetime



def accedoDispositivo(dispositivo):
	con = MySQLdb.connect("127.0.0.1","root","anopeludo","entrepinas")
	Cursor = con.cursor()
	res = Cursor.execute('update bloqueos set usando=1 where id=' + str(dispositivo) + ' and usando=0')
	con.commit()
	con.close()
	return res

def sueltoDispositivo(dispositivo):
	con = MySQLdb.connect("127.0.0.1","root","anopeludo","entrepinas")
	Cursor = con.cursor()
	Cursor.execute('update bloqueos set usando=0 where id=' + str(dispositivo) + ' ');
	con.commit()
	con.close()

def almacenaDato(sensor,valor):
	con = MySQLdb.connect("127.0.0.1","root","anopeludo","entrepinas")
	Cursor = con.cursor()
	sql = "Insert into data (fecha,sensor_id,valor) values (now(), " + str(sensor) + ", " + str(valor) + ")"
	Cursor.execute(sql)
	con.commit()
	con.close() 
