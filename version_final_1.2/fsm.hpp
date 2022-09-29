#include "funcionesEstadoIdle.hpp"
#include "funcionesEstadoConfigRPM.hpp"
#include "funcionesEstadoLimites.hpp"
#include "funcionesEstadoConfigTime.hpp"
#include "funcionesTimeInf.hpp"
#include "funcionesEstadoWorking.hpp"

void fsmSpinCoaterUpdate(){ //estado en el que el Spin Coater está esperando a que haya algun comando
  switch( spinCoaterState ){
    case stateIdle:     
      lcdSetIdle();  //muestro la pantalla de inicio
      vaciarDatos(); //vacio variables
      /* chequeo condiciones de salida del estado*/
      while(true){
        char customKey = customKeypad.getKey(); //recibo el caracter presionado del teclado
        if (customKey){                         //si se recibe algun caracter cambio de estado
          spinCoaterState=stateConfigRPM;       //voy al estado de configuracion de RPM
          break;
        }
      }
    break;

    case stateConfigRPM:{   //estado en el que se configura las revoluciones objetivo
      posicion=0;           //posicion utilizada en el string, la reseteo para que apunte al primer caracter
      lcdSetConfigRPM();    //muestro la pantalla de configuración
      while(true){
        cursorParpadeoRPM();                     //funcion que genera un parpadeo para ayudar al usuario a ver donde está parado el cursor
        char customKey = customKeypad.getKey();  //recibo el caracter presionado del teclado
        if (customKey){                          //si se recibe algun caracter:
          /*uso del teclado numérico para ingresar la configuración*/
          if (customKey>='0'&&customKey<='9'&&posicion<=3){
            stringRPM[posicion]=customKey;
            lcd.print(stringRPM[posicion]);
            posicion++;
          }
          if (customKey=='*'&&posicion>0){
            borradoUnicoRPM(); //funcion que borra un caracter
          }
          if (customKey=='#'&&posicion>0){ 
            borradoTotalRPM(); //funcion que borra todos los caracteres
          }
          /*cuando se presiona Ent se acepta el número ingresado y se pasa al estado de configuración de tiempo*/
          if (customKey=='E'){
            spinCoaterState=stateConfigTime;
            convertirStringRPMAInt();
            if(setpointRPM<300||setpointRPM>9000){ //si la velocidad está fuera de los límites de trabajo se pasa a la pantalla de aviso
              spinCoaterState=stateLimites;
                for (posicion=0;posicion<=3;posicion++){  //vacío el string antes de salir
                stringRPM[posicion]='0';
              }
            }
            break;
          }
          /*cuando se presiona Esc se vuelve a la pantalla inicial*/
          if (customKey=='S'){ 
            spinCoaterState=stateIdle;
            for (posicion=0;posicion<=3;posicion++){  //vacío el string antes de salir
              stringRPM[posicion]='0';    
            }
            break;
          }
        }
      }
    }
    break;
    
    case stateLimites:     
      lcdAvisoLimites(); //aviso al usuario de los límites de trabajo del spin coater
      while(true){
        char customKey = customKeypad.getKey(); //recibo el caracter presionado del teclado
        if (customKey){
          if (customKey=='S'||customKey=='E'){ //si presiono Esc o Ent vuelvo a la pantalla de configuración de RPM
            spinCoaterState=stateConfigRPM;
            break;
          }
        }
      }
    break;
    
    case stateConfigTime:{//Estado en el que se configura el tiempo activo del spin coater
      posicion=0;         //posicion utilizada en el string, la reseteo para que apunte al primer caracter
      lcdSetConfigTime(); //nuestro la pantalla de configuración
      while(true){
        cursorParpadeoTime(); //funcion que genera un parpadeo para ayudar al usuario a ver donde está parado el cursor
        char customKey = customKeypad.getKey();   //recibo el caracter presionado del teclado
        if (customKey){                           //si se recibe algun caracter:
          /*uso del teclado numérico para ingresar la configuración*/
          if(customKey>='0'&&customKey<='9'&&posicion==3){
            stringTime[posicion]=customKey;
            lcd.setCursor(posicion+1,1);
            lcd.print(stringTime[posicion]);
            posicion++;
          }
          if(customKey>='0'&&customKey<='5'&&posicion==2){
          stringTime[posicion]=customKey;
          lcd.setCursor(posicion+1,1);
          lcd.print(stringTime[posicion]);
          posicion++;
          }
          if (customKey>='0'&&customKey<='9'&&posicion<=1){
            stringTime[posicion]=customKey;
            lcd.print(stringTime[posicion]);
            posicion++;
          }
          if (customKey=='*'&&posicion>0){
            borradoUnicoTime(); //funcion que borra un caracter
          }
          if (customKey=='#'&&posicion>0){
            borradoTotalTime(); //funcion que borra todos los caracteres
          }
          if (customKey=='E'){
            spinCoaterState=stateWorking;
            while (posicion<=3){
              stringTime[posicion]='0'; //en caso de no completar el número se agregan 0 luego de aceptar
              posicion++;
            }
            convertirStringTimeAInt(); //funcion que convierte los caracteres del string a un entero
            infTime=false;             //el tiempo configurado es finito
            if(setTime==0){            //si el usuario apreta Ent con el tiempo en 00:00 se pregunta si quiere un tiempo indefinido
              spinCoaterState=stateTimeInf;//voy al estado de config de tiempo indefinido
              }
            break;
          }
          //si el usuario apreta F1 se pregunta si quiere un tiempo indefinido
          if (customKey=='F'){
            spinCoaterState=stateTimeInf;
            break;
          }
          /*si se aprieta la tecla Esc se vuelve a la pantalla anterior, la configuración de RPM*/
          if (customKey=='S'){
            for (posicion=0;posicion<=3;posicion++){
            stringTime[posicion]='0';   //vacío el string antes de salir
            }
            spinCoaterState=stateConfigRPM;
            break;
          }
        }
      }
    }
    break;

    case stateTimeInf:     
      lcdAskTimeInf(); //pregunto al usuario si quiere un tiempo indefinido
      while(true){
        char customKey = customKeypad.getKey(); //recibo el caracter presionado del teclado
        if (customKey){
          if (customKey=='E'){ //si presiono Ent configuro un tiempo indefinido
            spinCoaterState=stateWorking;
            infTime=true;
            setTime=0xff;
            break;
          }
          if (customKey=='S'){ //si presiono Esc vuelvo a la pantalla de configuración de tiempo
            spinCoaterState=stateConfigTime;
            infTime=false;
            break;
          }
        }
      }
    break;
    
    case stateWorking:{       //estado de trabajo del spin coater
      digitalWrite(53, HIGH); //habilito el enable del LN298 para arrancar el motor.
      setWorkingConfig();     //funcion que prepara el Arduino para ejecutar el PI
      lcdSetWorkingInit();    //funcion que prepara el LCD
      while(true){
        stateFuncionando();                       //funcion que contiene el controlador y actualiza la información en pantalla
        char customKey = customKeypad.getKey();   //recibo el caracter presionado del teclado
        if (customKey){                           //si se recibe algun caracter:
          if (customKey=='U'){ //si presiono la tecla hacia arriba agrego 30 segundos al contador
            setTime=setTime+30;
          }
          /*al presionar Esc detengo el spin coater y vuelvo a la pantalla de inicio*/
          if (customKey=='S'){
            spinCoaterState=stateIdle;
            digitalWrite(53, LOW);
            break;
          }
        }
        /*cuando el tiempo restante llega a 0 paso a la pantalla de aviso de fin de trabajo*/
        if(setTime==0&&infTime==false){
          spinCoaterState=stateEndTime;
          break;
        }
      }
    }
    break;
    
    case stateEndTime:{      //estado en el que se avisa que el tiempo de trabajo terminó y se detiene el motor
      lcdAvisoFinal();       //muestro la pantalla de aviso que se terminó el tiempo
      digitalWrite(53, LOW); //quito el enable del LN298, se apaga el motor.
      while(true){
        char customKey = customKeypad.getKey();   //recibo el caracter presionado del teclado
        if (customKey){
          if (customKey=='S'||customKey=='E'){ //si presiono Esc o Ent vuelvo a la pantalla de inicio
            spinCoaterState=stateIdle;
            break;
          }
        }
      }
    }
    break;
  }
}
