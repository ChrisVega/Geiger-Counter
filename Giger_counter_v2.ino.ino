#include <LiquidCrystal.h>

//Pins for the LCD screen
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define Conv_Fac 0.0057 //Conversion factor for CPM to uSV/h !!!For the SBM-20 Soviet G-M Tube!!!

long Period = 10000; //Time for which particles are counted in milliseconds
int Geiger_in = 2; //Input pin from Geiger counter
long count = 0;
long CPM = 0;
float multiplier = 60000.0/Period; //muultiplied by the count to esimate CPM
long lastmeasurement_time = 0;
float Dose = 0.0;

void setup() {
  pinMode(Geiger_in, INPUT);
  digitalWrite(Geiger_in,HIGH);
  
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Get out of here");
  lcd.setCursor(0,1);
  lcd.print("S.T.A.L.K.E.R.");
  attachInterrupt(digitalPinToInterrupt(Geiger_in),PulseCount,FALLING);
}

void PulseCount(){
  detachInterrupt(0);
  count++;
  attachInterrupt(digitalPinToInterrupt(Geiger_in),PulseCount,FALLING);
}

void loop() {
  if(millis() - lastmeasurement_time > Period){
    CPM = multiplier*count; //Estimation of CPM 
    Dose = CPM*Conv_Fac; //Doesage is esimated by multiplying the CPM by your tube's conversion factor
    lastmeasurement_time = millis();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CPM=");
    lcd.setCursor(4,0);
    lcd.print(CPM);
    lcd.setCursor(0,1);
    lcd.print(Dose,4);
    lcd.setCursor(6,1);
    lcd.print(" uSv/h");
    count = 0;
  }
}
