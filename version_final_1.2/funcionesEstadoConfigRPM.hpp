void lcdSetConfigRPM(){       //funcion que muestra la pantalla del estado
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Velocidad:");
  lcd.setCursor(5,1);
  lcd.print("RPM");
  lcd.setCursor(0,1);
  lcd.print(stringRPM);       //se muestra la velocidad guardada, inicialmente '0000'
  lcd.setCursor(0,1);
}

void convertirStringRPMAInt(){  //funcion que transforma los caracteres del string en un entero
  setpointRPM=0;
  setpointRPM=(stringRPM[0]-'0')*1000;
  setpointRPM=setpointRPM+(stringRPM[1]-'0')*100;
  setpointRPM=setpointRPM+(stringRPM[2]-'0')*10;
  setpointRPM=setpointRPM+(stringRPM[3]-'0');
}

void cursorParpadeoRPM(){       //funcion que genera un parpadeo para ayudar al usuario a ver donde está parado el cursor
  if(posicion<=3){
    if(millis()-delay4>=333){
      delay4=millis();
      if(cursorApagado){
        lcd.print(stringRPM[posicion]);
        cursorApagado=false;
        lcd.setCursor(posicion,1);
      }
      else{
        lcd.print(" ");
        cursorApagado=true;
        lcd.setCursor(posicion,1);
      }
    }
  }
}

void borradoUnicoRPM(){   //funcion que sirve para borrar el caracter de la derecha
  posicion--;
  stringRPM[posicion]='0';
  lcd.setCursor(0,1);
  lcd.print(stringRPM);
  lcd.setCursor(posicion,1);
}

void borradoTotalRPM(){   //funcion que borra todos los caracteres ingresados
  for (posicion=0;posicion<=3;posicion++){
    stringRPM[posicion]='0';
  }
  posicion=0;
  lcd.setCursor(0,1);
  lcd.print(stringRPM);
  lcd.setCursor(0,1);
}
