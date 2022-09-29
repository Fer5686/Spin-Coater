void lcdSetConfigTime(){    //funcion que muestra la pantalla del estado
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tiempo:");
  lcd.setCursor(0,1);
  lcd.print(stringTime[0]);
  lcd.print(stringTime[1]);
  lcd.print(":");
  lcd.print(stringTime[2]);
  lcd.print(stringTime[3]);
  lcd.setCursor(0,1);
}

void convertirStringTimeAInt(){ //funcion que transforma los caracteres del string en un entero, calculando los segundos configurados
  setTime=0;
  setTime=(stringTime[0]-'0')*600;
  setTime=setTime+(stringTime[1]-'0')*60;
  setTime=setTime+(stringTime[2]-'0')*10;
  setTime=setTime+(stringTime[3]-'0');
}

void cursorParpadeoTime(){  //funcion que genera un parpadeo para ayudar al usuario a ver donde está parado el cursor
  if(posicion<=3){
    if(millis()-delay4>=333){
      delay4=millis();
      if(cursorApagado){
        if(posicion>=2){ //se divide en dos opciones debido a que hay un ':' en el medio
          lcd.setCursor(2,1);
          lcd.print(":");
          lcd.setCursor(posicion+1,1);
          lcd.print(stringTime[posicion]);
          cursorApagado=false;
          lcd.setCursor(posicion+1,1);
        }
        else{
          lcd.print(stringTime[posicion]);
          cursorApagado=false;
          lcd.setCursor(posicion,1);
        }
      }
      else{
        if(posicion>=2){
          lcd.setCursor(2,1);
          lcd.print(":");
          lcd.setCursor(posicion+1,1);
          lcd.print(" ");
          cursorApagado=true;
          lcd.setCursor(posicion+1,1);
        }
        else{
          lcd.print(" ");
          cursorApagado=true;
          lcd.setCursor(posicion,1);
        }
      }
    }
  }
}

void borradoUnicoTime(){  //funcion que sirve para borrar el caracter de la derecha
  posicion--;
  stringTime[posicion]='0';
  lcd.setCursor(0,1);
  lcd.print(stringTime[0]);
  lcd.print(stringTime[1]);
  lcd.print(":");
  lcd.print(stringTime[2]);
  lcd.print(stringTime[3]);
  lcd.setCursor(posicion,1);
}

void borradoTotalTime(){  //funcion que borra todos los caracteres ingresados
  for (posicion=0;posicion<=3;posicion++){
    stringTime[posicion]='0';  
  }
  posicion=0;
  lcd.setCursor(0,1);         //se imprime todo el string en pantalla
  lcd.print(stringTime[0]);
  lcd.print(stringTime[1]);
  lcd.print(":");
  lcd.print(stringTime[2]);
  lcd.print(stringTime[3]);
  lcd.setCursor(0,1);
}
