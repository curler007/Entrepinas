
//Modulo temperaturas:
int sensor1 = 1; //pin de conexion del sensor
int sensor2 = 2;
void setupSensoresTemperatura(){
  analogReference(INTERNAL);
  pinMode(sensor1, INPUT); //declara pin del LM de entrada
  pinMode(sensor2, INPUT); //declara pin del LM de entrada
  Serial.begin (9600); //inicia comunicacion serial
}
void loopTemperaturas(){
  int tomas = 5.0;
    //Calcula la temperatura usando como referencia 5v
  if (Serial.available())
  {
    int puerto = Serial.read();
    float media=0.0;
    for(int i=0; i<tomas; i++){
      media = media + getTemperatura(puerto);
    }  
    float t = media/tomas;
    Serial.println (t,DEC);
  }
  Serial.flush();
}
float getTemperatura(int puerto){
  float reading, temperatura = 0;
  if (puerto==sensor1 || puerto==sensor2){
    reading = analogRead(puerto);
    temperatura = reading / 9.31;    
  }else{
     temperatura = -1;
  }
  
  return temperatura;
}
//***********************
//Fin modulo temperaturas


//MODULO ROVER POMPE
//constantes
int const ESTADO_APAGADA=0;
int const ESTADO_DIR_NORMAL=1;
int const ESTADO_DIR_INVERSA=2;

int MODO_AUTOMATICO=0;
int MODO_MANUAL=1;

/*
	El relé de Encendido estara abierto cuando tenga el valor LOW
	El relé de dirección estará en al dirección normal cuando tenga el valor LOW
*/
int PIN_RELE_GENERAL = 4;
int PIN_RELE_DIRECCION = 5;

/*
Pin en el que se conecta el pulsador de cambio de modo. Por defecto debe ser LOW
*/
 int PIN_PULSADOR_AUTO_MANUAL = 6;

/*
Pin en el que se conecta la salida para la DIRECCION NORMAL del conmutador de Rover Pompe
*/
 int PIN_CONMUTADOR_POS_NORMAL = 7;

/*
Pin en el que se conecta la salida para la DIRECCION INVERSA del conmutador de Rover Pompe
*/
 int PIN_CONMUTADOR_POS_INVERSA = 8;
 int PIN_LED_AUTO = 9;

/*
	Indican el tiempo en milisegundos en que la bomba debe funcionar en cada una de sus posiciones
*/
long tiempoMaximoDireccionNormal = 10000;
long tiempoMaximoDireccionInversa = 4000;

/*
	Variables que indican el momento en milisegundos, en que la bomba se posicionó en dirección normal o dirección inversa.
*/
long inicioDireccionNormal= 0;
long inicioDireccionInversa = 0;

int estadoPulsadorManualAutomatico=LOW;

int estadoBomba = ESTADO_APAGADA;

int modo=MODO_AUTOMATICO;

void setupRover() {
    //Abrimos los pines de salida y de entrada
    pinMode(PIN_RELE_GENERAL, OUTPUT);
    pinMode(PIN_RELE_DIRECCION, OUTPUT);
    pinMode(PIN_PULSADOR_AUTO_MANUAL, INPUT);
    pinMode(PIN_CONMUTADOR_POS_NORMAL,INPUT);
    pinMode(PIN_CONMUTADOR_POS_INVERSA,INPUT);
    pinMode(PIN_LED_AUTO,OUTPUT);

}
void loopRover() {
	comprobarModo();
	if (modo==MODO_AUTOMATICO){
 		if (seDebeEncender()){
    			cambiarEstadoBomba(ESTADO_DIR_NORMAL);
 		}else if (seDebeInvertir()){
      			cambiarEstadoBomba(ESTADO_DIR_INVERSA);
 		}else if (seDebeApagar()){
			cambiarEstadoBomba(ESTADO_APAGADA);
                }
        }else if (getEstadoConmutadorManual()!=estadoBomba){
		cambiarEstadoBomba(getEstadoConmutadorManual());
	}
}

void comprobarModo(){
	int nuevoEstadoPulsador = digitalRead(PIN_PULSADOR_AUTO_MANUAL);
	if (nuevoEstadoPulsador!=estadoPulsadorManualAutomatico){
		if (nuevoEstadoPulsador == HIGH){
			//Serial.println("Cambio de modo Automatico/manual");
			/*Cambio el modo*/
			if (modo==MODO_AUTOMATICO){
				modo=MODO_MANUAL;
			}else{
				modo=MODO_AUTOMATICO;
			}
		}
		estadoPulsadorManualAutomatico = nuevoEstadoPulsador;
        }
        if (modo==MODO_AUTOMATICO){
          digitalWrite(PIN_LED_AUTO,HIGH);
        }else{
          digitalWrite(PIN_LED_AUTO,LOW);
        }
}


//Cambia el estado de la bomba al que se le indique por parámetro.
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
		delay(1000);
	}
        digitalWrite(PIN_RELE_DIRECCION,LOW);
        digitalWrite(PIN_RELE_GENERAL,HIGH);
        inicioDireccionNormal=millis();
      break;
      case ESTADO_DIR_INVERSA:
        //Serial.println("Invierto");

	if (estadoBomba == ESTADO_DIR_NORMAL){
            digitalWrite(PIN_RELE_GENERAL,LOW);
		delay(1000);
	}
        digitalWrite(PIN_RELE_DIRECCION,HIGH);
        digitalWrite(PIN_RELE_GENERAL,HIGH);
        inicioDireccionInversa=millis();
      break;
    }  
    estadoBomba = nuevoEstado;
  }
}

//	Indica si se debe encender la bomba o no
bool seDebeEncender(){
	bool resultado = 0;
	if (estadoBomba==ESTADO_APAGADA && millis()<tiempoMaximoDireccionNormal){
	  resultado = 1;
	}
	return resultado;
}

//	Indica si se debe invertir la dirección de la bomba o no
bool seDebeInvertir(){
	bool resultado = 0;
	if (estadoBomba==ESTADO_DIR_NORMAL && (millis() - inicioDireccionNormal) > tiempoMaximoDireccionNormal){
	  resultado = 1;
	}
	return resultado;
}

//	Indica si se debe apagar la bomba
bool seDebeApagar(){
	bool resultado = 0;
	if (!seDebeInvertir()  && estadoBomba==ESTADO_DIR_INVERSA && (millis() - inicioDireccionInversa) > tiempoMaximoDireccionInversa){
		resultado = 1;
	}
	return resultado;
}


/*Devuelve el estado de funcionamiento a partir de la posición del conmutador manual*/
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

void loop(){
  loopTemperaturas();
  loopRover();
}






