void lcdAvisoLimites(){   //funcion que muestra la pantalla del estado
  lcd.setCursor(0,0);
  lcd.print("La velocidad esta");
  lcd.setCursor(0,1);
  lcd.print("limitada entre 300");
  lcd.setCursor(0,2);
  lcd.print("y 9000 RPM");
}
