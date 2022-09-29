void lcdAskTimeInf(){
  lcd.setCursor(0,0);
  lcd.print("Quiere programar un");
  lcd.setCursor(0,1);
  lcd.print("tiempo indefinido ?");
  lcd.setCursor(0,2);
  lcd.print("S:Ent    /    N:Esc");
}
