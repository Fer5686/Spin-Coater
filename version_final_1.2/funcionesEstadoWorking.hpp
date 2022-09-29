void calcularPID(){             //funcion que se encarga de ejecutar el código del PI
  pidController.Input = rpm;    //se actualiza la entrada con el valor de la variable rpm
  pidController.Update();       //se actualiza el PI
  DutyPWM = pidController.Output;//se asigna el valor de la salida del PI a la variable de ciclo de trabajo
  analogWrite(pinPWM,DutyPWM);   //el valor de la variable de ciclo de trabajo se asigna a la salida analógica
}

void convertirSetTimeAstring(){  //convierte la variable de tiempo restante a un string para poder mostrarlo después en pantalla
  stringTime[0]=setTime/600+'0';
  stringTime[1]=(setTime%600)/60+'0';
  stringTime[2]=((setTime%600)%60)/10+'0';
  stringTime[3]=((setTime%600)%60)%10+'0';
}

void setWorkingConfig(){                //configura variables para iniciar el trabajo
  pidController.TurnOn();               //enciende el PI
  pidController.Setpoint = setpointRPM; //configura la referencia del PI con el valor ingresado
  duracion=0xffffffff;                  //variable que se utiliza para calcular los RPM, se reinicia a el valor máximo para que el cociente de aproximadamente 0
}

void lcdSetWorkingInit(){     //funcion que limpia la pantalla anterior y muestra la pantalla del estado
  lcd.clear();                //limpio la pantalla del LCD
  lcd.setCursor(0,0);         //muevo el cursor a la esquina superior izq
  lcd.print("estabilizando"); //imprimo el mensaje
  /*muestro las frases que son estáticas*/
  lcd.setCursor(0,1);         
  lcd.print("V. objetivo:");
  rpmPrint=setpointRPM;
  lcd.setCursor(16,1);
  lcd.print(" RPM");
  lcd.setCursor(0,2);
  lcd.print("V. actual: ");
  lcd.setCursor(16,2);
  lcd.print(" RPM");
}

void mostrarRPM(){           //funcion que muestra la información de la velocidad actual y la objetivo
  rpmPrint=setpointRPM;      //se asigna el setpoint a una variable usada para el print
  /*se acomoda el cursor según la cantidad de cifras a mostrar*/
  lcd.setCursor(12,1); 
  if (rpmPrint<=999){
    lcd.setCursor(13,1);
    if(rpmPrint<=99){
      lcd.setCursor(14,1);
    }
  }
  lcd.print(rpmPrint);      //se muestra la información de la velocidad de referencia
  rpmPrint=rpm;
  lcd.setCursor(12,2);      
  lcd.print("     ");       //se borra el número anterior ya que si se imprime encima puede quedar un número sin reemplazar según la cantidad de cifras
  lcd.setCursor(12,2);
  /*se acomoda el cursor según la cantidad de cifras a mostrar*/
  if (rpmPrint<=999){
    lcd.setCursor(13,2);
    if(rpmPrint<=99){
      lcd.setCursor(14,2);
      if(rpmPrint<=9){
        lcd.setCursor(15,2);
      }
    }
  }
  lcd.print(rpmPrint);
}

void mostrarPWM(){  //funcion no utilizada, en versiones anteriores del código se usaba para ver el valor del PWM
  lcd.setCursor(0,2);
  lcd.print(DutyPWM);
  lcd.print(" PWM");
}

void stateFuncionando(){  //funcion principal del estado de trabajo del spin coater
  stable=false;
  if(millis()-delay1>=2){
    rpm=30000000/duracion;
    calcularPID();
    delay1=millis();
  }
  //muestro el valor de los RPM
  if(millis()-delay2+5>=delayRPM){
    mostrarRPM();
    delay2=millis();
  }
  if(stable==false){ //chequeo si el sistema llegó a la velocidad indicada +-2%
    if(rpm>=(setpointRPM*0.98)&&rpm<=(setpointRPM*1.02)){
      stable=true;
      lcd.setCursor(0,0);
      lcd.print("             ");
      lcd.setCursor(0,0);
      lcd.print("OK!");
    }
    else{
      lcd.setCursor(0,0);
      lcd.print("             ");
      lcd.setCursor(0,0);
      lcd.print("estabilizando");
    }
  }
  if(stable==true){
    if(infTime==false){
      if(millis()-delay3>=1000){ //actualizo el tiempo restante cada un seg
        delay3=millis();
        setTime--;
        convertirSetTimeAstring();
        lcd.setCursor(15,0);
        lcd.print(stringTime[0]);
        lcd.print(stringTime[1]);
        lcd.print(":");
        lcd.print(stringTime[2]);
        lcd.print(stringTime[3]);
      }
    }
    else{
      lcd.setCursor(15,0);
      lcd.print("--:--");
    }
  }
}

void lcdAvisoFinal(){
  lcd.clear();
  lcd.setCursor(1,2);
  lcd.print("Termino el tiempo");
}
