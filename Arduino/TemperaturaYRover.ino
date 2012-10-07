#include <EEPROM.h>
//Entrada por Serie:
// Los valores menores de 10 son para leer el estado, Arduino escribira por el serial la salida correspondiente
// Los valores mayores o iguales de 10 son comandos, Arduino activar¬´ o desactivara la bomba

//Modulo temperaturas:
char const sensor1 = 1; //pin de conexion del sensor
char const sensor2 = 2;
char const sensor3 = 3;
char const sensor4 = 4;
char const sensor5 = 5;

//Variable para acceder al estado de la bomba:
   // resultado: ESTADO_APAGADA ESTADO_DIR_NORMAL ESTADO_DIR_INVERSA

char const sensorEstadoBomba = 6;

//Seguimos en el mismo rango de constantes:
char const COM_ACTIVAR_BOMBA = 'E';
char const COM_INVERTIR_BOMBA = 'I';
char const COM_APAGAR_BOMBA = 'A';

//MODULO ROVER POMPE
//constantes
int const ESTADO_APAGADA=0;
int const ESTADO_DIR_NORMAL=1;
int const ESTADO_DIR_INVERSA=2;

int MODO_AUTOMATICO=0;
int MODO_MANUAL=1;

/*
	El rel≈Ω de Encendido estara abierto cuando tenga el valor LOW
	El rel≈Ω de direcci‚Äîn estar‚Ä° en al direcci‚Äîn normal cuando tenga el valor LOW
*/
int PIN_RELE_GENERAL = 3;
int PIN_RELE_DIRECCION = 4;

/*
Pin en el que se conecta el pulsador de cambio de modo. Por defecto debe ser LOW
*/
 int PIN_PULSADOR_AUTO = 5;

/*
Pin en el que se conecta la salida para la DIRECCION NORMAL del conmutador de Rover Pompe
*/
 int PIN_CONMUTADOR_POS_NORMAL = 6;

/*
Pin en el que se conecta la salida para la DIRECCION INVERSA del conmutador de Rover Pompe
*/
 int PIN_CONMUTADOR_POS_INVERSA = 7;
 

int estadoPulsadorManualAutomatico=LOW;

int estadoBomba = ESTADO_APAGADA;
int eepromEstadoBomba = 0;

int modo=MODO_AUTOMATICO;



//Inicio metodos temperatura
void setupSensoresTemperatura(){
  analogReference(INTERNAL);
  pinMode(sensor1, INPUT); //declara pin del LM de entrada
  pinMode(sensor2, INPUT); //declara pin del LM de entrada
  pinMode(sensor3, INPUT); //declara pin del LM de entrada
  pinMode(sensor4, INPUT); //declara pin del LM de entrada
  pinMode(sensor5, INPUT); //declara pin del LM de entrada
  Serial.begin (9600); //inicia comunicacion serial
}

//Si la entrada por serie es menor que 10, es para leer un dato, si es igual o mayor es para ejecutar una accion en arduino
void loopAtiendePeticiones(){
    //Calcula la temperatura usando como referencia 5v
  if (Serial.available())
  {
    char entrada = Serial.read();
   
    if(entrada>='0' and entrada <= '9'){

      int entradaInt = convierteCharEnInt(entrada);
      //Serial.print(entradaInt);
      float t = getDato(entradaInt);
      Serial.println (t,DEC);
    }else if(entrada == COM_ACTIVAR_BOMBA or entrada == COM_APAGAR_BOMBA or entrada == COM_INVERTIR_BOMBA){
      if(modo==MODO_AUTOMATICO){
        ejecutaComando(entrada);
        Serial.println(entrada);
      }else{
        Serial.println('M');
      }
    }else{
	Serial.println(-103,DEC);
    }
  }
  //Serial.flush();
}

int convierteCharEnInt(char caracter){
  int resultado = -1;
  switch(caracter){
    case '0':
      resultado = 0;
      break;
    case '1':
      resultado = 1;
      break;  
    case '2':
      resultado = 2;
      break;
    case '3':
      resultado = 3;
      break;  
    case '4':
      resultado = 4;
      break;
    case '5':
      resultado = 5;
      break;  
    case '6':
      resultado = 6;
      break;
    case '7':
      resultado = 7;
      break;  
    case '8':
      resultado = 8;
      break;
    case '9':
      resultado = 9;
      break;  
  }
  return resultado;
  
}

float getDato(int puerto){
  float resultado = -100;
  if (puerto==sensor1 || puerto==sensor2 || puerto==sensor3 || puerto==sensor4 || puerto==sensor5){
    resultado = getTemperatura(puerto);
    
  }else if (puerto == sensorEstadoBomba){
     resultado = estadoBomba;
  }
  
  return resultado;
}

//Devuelve la temperatura que marca el LM35 conectado al puerto. Aproxima el mejor valor realizando varias medidas.
float getTemperatura(int puerto){
  float min = 100;
  float max = -100;
  float media = 0.0;
  float medidas[10];
  for(int i=0; i<10; i++){
    float t = getTemperaturaFromAnalogPort(puerto);
 
    medidas[i] = t;
  }  
  media = getMedia(medidas);
  
  //quitamos el menor y el mayor para evitar posibles errores
 // float suma = media * sizeof(medidas);
  //suma = suma - max - min;
  //media = suma / (sizeof(medidas)-2);
  return media;
}

float getMedia(float medidas[]){
  float suma = 0;
  for(int i=0;i<sizeof(medidas);i++){
    suma = suma + medidas[i];
  }
  return suma/sizeof(medidas);
}

float getTemperaturaFromAnalogPort(int puerto){
  float reading, temperatura = 0;
  reading = analogRead(puerto);

  temperatura = reading / 9.31;  
  return temperatura;  
}
//***********************
//Fin modulo temperaturas





//Inicio metodos Rover Pompe




void setupRover() {
    //Abrimos los pines de salida y de entrada
    pinMode(PIN_RELE_GENERAL, OUTPUT);
    pinMode(PIN_RELE_DIRECCION, OUTPUT);
    pinMode(PIN_PULSADOR_AUTO, INPUT);
    pinMode(PIN_CONMUTADOR_POS_NORMAL,INPUT);
    pinMode(PIN_CONMUTADOR_POS_INVERSA,INPUT);
   

}

/*void loopRover() {
        
	comprobarModo();

     //   if (getEstadoConmutadorManual()!=estadoBomba){
		cambiarEstadoBomba(getEstadoConmutadorManual());
//	}
}*/

/*void comprobarModo(){
	int nuevoEstadoPulsador = digitalRead(PIN_PULSADOR_AUTO);

	if (nuevoEstadoPulsador == HIGH){
	  modo=MODO_AUTOMATICO;
        }else{
	  modo=MODO_MANUAL;
	}
}*/

void ejecutaComando(char comando){
  switch(comando){
    case COM_ACTIVAR_BOMBA:
      
      cambiarEstadoBomba(ESTADO_DIR_NORMAL);
    break;
    case COM_INVERTIR_BOMBA:
    
      cambiarEstadoBomba(ESTADO_DIR_INVERSA);
    break;
    case COM_APAGAR_BOMBA:
      
      cambiarEstadoBomba(ESTADO_APAGADA);
    break;
  }
}

//Cambia el estado de la bomba al que se le indique por par‚Ä°metro.
void cambiarEstadoBomba(int nuevoEstado){
  if (estadoBomba != nuevoEstado){
    switch (nuevoEstado){
      case ESTADO_APAGADA:
        //Serial.println("Apago");
 	digitalWrite(PIN_RELE_GENERAL,LOW);
	digitalWrite(PIN_RELE_DIRECCION,LOW);
      	break;
      case ESTADO_DIR_NORMAL:
         
        //Serial.println("Enciendo");
	if (estadoBomba == ESTADO_DIR_INVERSA){
		delay(600);
	}
        digitalWrite(PIN_RELE_DIRECCION,LOW);
        digitalWrite(PIN_RELE_GENERAL,HIGH);
        
      break;
      case ESTADO_DIR_INVERSA:
        //Serial.println("Invierto");

	if (estadoBomba == ESTADO_DIR_NORMAL){
            digitalWrite(PIN_RELE_GENERAL,LOW);
		delay(600);
	}
        digitalWrite(PIN_RELE_DIRECCION,HIGH);
        digitalWrite(PIN_RELE_GENERAL,HIGH);
        
      break;
    }  
    estadoBomba = nuevoEstado;
    if (EEPROM.read(eepromEstadoBomba)!=estadoBomba){
    	EEPROM.write(eepromEstadoBomba,estadoBomba);
    }
  }
}



/*Devuelve el estado de funcionamiento a partir de la posici‚Äîn del conmutador manual*/
int getEstadoConmutadorManual(){
  int estado = ESTADO_APAGADA;
  int pinNormal =digitalRead(PIN_CONMUTADOR_POS_NORMAL);
  int pinInverso =digitalRead(PIN_CONMUTADOR_POS_INVERSA);
  if (pinNormal==HIGH && pinInverso==LOW){
	estado = ESTADO_DIR_NORMAL;
  }else if(pinInverso==HIGH && pinNormal==LOW){
	estado = ESTADO_DIR_INVERSA;
  }else{
	estado=ESTADO_APAGADA;
  }
  return estado;
}

void setup(){
  setupSensoresTemperatura();
  setupRover();
}

void loopCompruebaInterruptor(){
  if (getEstadoConmutadorManual()!=ESTADO_APAGADA){
    modo = MODO_MANUAL;
    if (getEstadoConmutadorManual()!=estadoBomba){
       cambiarEstadoBomba(getEstadoConmutadorManual());
    }
  }else{
    if (modo== MODO_MANUAL){
      modo = MODO_AUTOMATICO;
      cambiarEstadoBomba(ESTADO_APAGADA);
    }
  }
}


void loopVerificaPersistencia(){
	if(EEPROM.read(eepromEstadoBomba)!=estadoBomba){
		cambiarEstadoBomba(EEPROM.read(eepromEstadoBomba));
	}
}

void loop(){
  loopVerificaPersistencia();
  loopCompruebaInterruptor();
  loopAtiendePeticiones();
//  loopRover();
}
