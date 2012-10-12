import sys
sys.path.append("/home/pi/entrepinas/")
import eeml
import arduino
import bd
import os
from datetime import *

#1. Tomamos los datos de arduino y los almacenamos en variables

s1 = "{0:.2f}".format(arduino.getValor('1',1))
s2 = "{0:.2f}".format(arduino.getValor('2',1))
s3 = "{0:.2f}".format(arduino.getValor('3',1))
s4 = "{0:.2f}".format(arduino.getValor('4',1))
s5 = "{0:.2f}".format(arduino.getValor('5',1))
eb = "{0:.2f}".format(arduino.getValor('6',1))

#2. Volcamos los datos en un fichero csv en $HOME/entrepinas/entrepinas.csv

f = open("entrepinas.csv", "a")
f.write("%s,%s,%s,%s,%s,%s,%s\n" % (datetime.today(),s1,s2,s3,s4,s5,eb))

#3. Almacenamos los datos en base de datos

bd.almacenaDato(1,s1)
bd.almacenaDato(2,s2)
bd.almacenaDato(3,s3)
bd.almacenaDato(4,s4)
bd.almacenaDato(5,s5)
bd.almacenaDato(6,eb)


#4. Enviamos los datos a Cosm

API_KEY = 'qeYiY5k6Io4qpnhqlIlJu7g_c4OSAKx2d3ZQTkdlK0xzOD0g'
FEED = '79947'

API_URL = '/v2/feeds/{feednum}.xml' .format(feednum = FEED)

pac = eeml.Pachube(API_URL, API_KEY)
pac.update([eeml.Data(1, s1, unit=eeml.Celsius())])
pac.put()
pac.update([eeml.Data(2, s2, unit=eeml.Celsius())])
pac.put()
pac.update([eeml.Data(3, s3, unit=eeml.Celsius())])
pac.put()
pac.update([eeml.Data(4, s4, unit=eeml.Celsius())])
pac.put()
pac.update([eeml.Data(5, s5, unit=eeml.Celsius())])
pac.put()

#5. Pasamos los datos por http a nuestra base de datos en Madrid. 

os.system("wget \"http://www.hotelpene.com/entrepinas/cargadatos.php?s1=%s&s2=%s&s3=%s&s4=%s&s5=%s&eb=%s\"" % (s1,s2,s3,s4,s5,eb))






