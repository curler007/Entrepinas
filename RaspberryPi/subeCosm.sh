#!/bin/sh
#Se encarga de recuperar los datos de temperatura y subirlos a Cosm
cd /home/pi/entrepinas
chmod 777 /dev/ttyACM0
python cosm.py