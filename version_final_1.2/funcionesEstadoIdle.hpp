void lcdSetIdle(){  //funcion que muestra la pantalla del estado
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Presione cualquier");
  lcd.setCursor(0,1);
  lcd.print("boton para empezar");
}

void vaciarDatos(){   //funcion que reinicia las variables utilizadas por el spin coater
  for (posicion=0;posicion<=3;posicion++){
    stringRPM[posicion]='0';//vacío el string
    stringTime[posicion]='0';//vacío el string
  }
  posicion=0;
  pidController.Output=0;
  DutyPWM = pidController.Output;
  analogWrite(pinPWM,DutyPWM);
  pidController.TurnOff();
  infTime=false;
  setTime=0;
}
