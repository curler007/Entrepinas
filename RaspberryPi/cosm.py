import eeml
import arduino

# parameters
API_KEY = 'qeYiY5k6Io4qpnhqlIlJu7g_c4OSAKx2d3ZQTkdlK0xzOD0g'
FEED = '72840'

API_URL = '/v2/feeds/{feednum}.xml' .format(feednum = FEED)

pac = eeml.Pachube(API_URL, API_KEY)
t1 = arduino.getValor('1')*100
t1 = float(int(t1))/float(100)
t2 = (arduino.getValor('2')-3)*100
t2 = float(int(t2))/float(100)
t3 = arduino.getValor('3')
pac.update([eeml.Data(0, t1, unit=eeml.Celsius())])
pac.put()
pac.update([eeml.Data(1, t2, unit=eeml.Celsius())])
pac.put()
pac.update([eeml.Data(2, t3)])
pac.put()

