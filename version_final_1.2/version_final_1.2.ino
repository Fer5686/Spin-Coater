//inicializaciones 
#include "funcionesSetup.hpp"
#include "fsm.hpp"

void setup() {
  arduinoSetupInit();  //inicializo el módulo Arduino con sus salidas, interrupciones y tiempos
  moduleLCDInit();     //inicializo el módulo LCD
  fsmSpinCoaterInit(); //inicializo la máquina de estados del Spin Coater
}                                        
 
void loop() {
  fsmSpinCoaterUpdate(); //actualizo la máquina de estados
}
