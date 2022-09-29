#include <PIDController.hpp>
#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//variables y constantes globales
int delayRPM = 200;
volatile unsigned long duracion = 0xffffffff;
volatile unsigned long delay1=0;
volatile unsigned long delay2=0;
volatile unsigned long delay3=0;
volatile unsigned long delay4=0;
char stringRPM[5]="0000";
char stringTime[5]="0000";
bool stable,infTime;
unsigned int setTime,setpointRPM;
int posicion;
bool cursorApagado=false;

//config del LCD 20x4
LiquidCrystal_I2C lcd(0x27,20,4);  //asigna la dirección del LCD a 0x27 para un display de 16 caracteres y 2 lineas

//config del teclado matricial 5x4
const byte filas = 5; //5 filas
const byte columnas = 4; //4 columnas

//define los simbolos de los botones del teclado
char teclas[filas][columnas] = {
  {'F','G','#','*'},
  {'1','2','3','U'},
  {'4','5','6','D'},
  {'7','8','9','S'},
  {'L','0','R','E'},
};
byte pinFila[filas] = {38,36,34,32,30}; //conecta los pines a las filas del teclado
byte pinColumna[columnas] = {28,26,24,22}; //conecta los pines a las columnas del teclado

//inicializa una nueva instance de una clase keypad
Keypad customKeypad = Keypad( makeKeymap(teclas), pinFila, pinColumna, filas, columnas);

//acá los parámetros para el contador de RPM
int pinSensorRPM = 2;                   //pin asignado a la salida del sensor, luego se asigna a una interrupción
volatile double rpm=0;                  //variable en la que se guarda el valor de los RPM, la entrada del PID
volatile unsigned int rpmPrint=0;       //variable utilizada para mostrar los RPM en pantalla, se convierte a entero para no mostrar decimales
volatile unsigned long elapsed=0;       //variable utilizada para medir el tiempo entre interrupciones
volatile unsigned long elapsed_prev=0;  //igual que la anterior

//acá van los parámetros para el PWM
int pinPWM=7; //el 4 y 13 van por defecto a 980Hz (el resto de PWM a 490Hz)
volatile unsigned int DutyPWM=0; //el ciclo de trabajo va de 0 a 255 (0 a 100%)

//acá van los parámetros para el PID
PID::PIDParameters<double> parameters(0.1, 0.1, 0); //proporcional, integral y derivativo respectivamente
PID::PIDController<double> pidController(parameters); //configuro los parámetros en el PID

//acá las constantes de la máquina de estados
typedef enum{
  stateIdle,
  stateConfigRPM,
  stateLimites,
  stateConfigTime,
  stateTimeInf,
  stateWorking,
  stateEndTime
}spinCoaterState_t;

spinCoaterState_t spinCoaterState;
