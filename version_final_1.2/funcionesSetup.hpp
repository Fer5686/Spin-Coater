#include "datos.h"

void cuentoRevolucion()    //Funcion que se ejecuta durante cada interrupion producida por el sensor de velocidad
{
  duracion = elapsed - elapsed_prev; //calcula la diferencia de tiempo entre una interrupcion y la otra
  elapsed_prev=elapsed;              //reasigna la variable para la siguiente vez
  elapsed=micros();                  //toma el tiempo en el que fué ejecutada la interrupcion
}

void arduinoSetupInit(){             //funciones que se ejecutan cuando el Arduino se enciende
  attachInterrupt(digitalPinToInterrupt(pinSensorRPM), cuentoRevolucion, RISING);  // genero la interrupción por el pin digital 2 cuentoRevolucion() //LOW, CHANGE, RISING, FALLING.
  elapsed_prev = micros(); //tomo el tiempo para luego calcular los RPM en la interrupción.
  delay1=millis(); //tomo el tiempo para luego calcular los intervalos entre tareas.
  
  //salidas en alto
  pinMode(53, OUTPUT);
  pinMode(52, OUTPUT);
  digitalWrite(52, HIGH); //salida a 5V para el LCD
}

void moduleLCDInit(){     //inicializando el LCD.
  Serial.begin(115200);   //inicializa el puerto serie que va al LCD.
  lcd.init();             //inicializa el lcd.
  lcd.backlight();        //enciende la luz trasera.
}

void fsmSpinCoaterInit(){
  spinCoaterState = stateIdle;   //configuro el estado inicial
}
