#!/bin/sh
#Se encarga de recuperar los datos de temperatura, almacenarlos en la bbdd y enviarlos a una bbdd remota
cd /home/pi/entrepinas
chmod 777 /dev/ttyACM0
python tomaDatos.py

