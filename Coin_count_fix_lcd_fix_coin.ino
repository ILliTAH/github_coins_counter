#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <EEPROM.h>


////// กรณีใช้แบบ Pararell  

//#include <LiquidCrystal.h> 
//LiquidCrystal lcd(12, 11, 7, 8, 9, 10);

/////////////////////////////////////////////

////// กรณีใช้แบบ I2C 

LiquidCrystal_I2C lcd(0x27, 20, 4);

/////////////////////////////////////////////


#define sensor1 2   // D2
#define sensor2 3    // D3
#define sensor3 4     // D4

//#define button_change_dis 6 // D6  ไม่ใช้เนื่องจากเปลี่ยนขนาดจอเป็น 20x4
#define button_reset_data 5 // D5
#define LCD_NB_COLUMNS 20

unsigned long int sum_money;
String a;
int coin_1,coin_5,coin_10;
int rissing=0;


unsigned long previousMillis = 0;
unsigned long interval = 3600000;



byte START_DIV_0_OF_1[8] = {
  B01111, 
  B11000,
  B10000,
  B10000,
  B10000,
  B10000,
  B11000,
  B01111
}; 

byte START_DIV_1_OF_1[8] = {
  B01111, 
  B11000,
  B10011,
  B10111,
  B10111,
  B10011,
  B11000,
  B01111
}; 

byte DIV_0_OF_2[8] = {
  B11111, 
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111
}; 

byte DIV_1_OF_2[8] = {
  B11111, 
  B00000,
  B11000,
  B11000,
  B11000,
  B11000,
  B00000,
  B11111
}; 

byte DIV_2_OF_2[8] = {
  B11111, 
  B00000,
  B11011,
  B11011,
  B11011,
  B11011,
  B00000,
  B11111
}; 

byte END_DIV_0_OF_1[8] = {
  B11110, 
  B00011,
  B00001,
  B00001,
  B00001,
  B00001,
  B00011,
  B11110
};

byte END_DIV_1_OF_1[8] = {
  B11110, 
  B00011,
  B11001,
  B11101,
  B11101,
  B11001,
  B00011,
  B11110
}; 



void setup_progressbar() {


  lcd.createChar(0, START_DIV_0_OF_1);
  lcd.createChar(1, START_DIV_1_OF_1);
  lcd.createChar(2, DIV_0_OF_2);
  lcd.createChar(3, DIV_1_OF_2);
  lcd.createChar(4, DIV_2_OF_2);
  lcd.createChar(5, END_DIV_0_OF_1);
  lcd.createChar(6, END_DIV_1_OF_1);
  
}

void draw_progressbar(byte percent , String state) {
 
  lcd.setCursor(6, 0);
  lcd.print(state);
  lcd.setCursor(0, 0);
  lcd.print(percent);
  lcd.print(F(" %  ")); 

 
  
  lcd.setCursor(0, 1);
 
  /* Map la plage (0 ~ 100) vers la plage (0 ~ LCD_NB_COLUMNS * 2 - 2) */
  byte nb_columns = map(percent, 0, 100, 0, LCD_NB_COLUMNS * 2 - 2);
  // Chaque caractère affiche 2 barres verticales, mais le premier et dernier caractère n'en affiche qu'une.

  /* Dessine chaque caractère de la ligne */
  for (byte i = 0; i < LCD_NB_COLUMNS; ++i) {

    if (i == 0) { // Premiére case

      /* Affiche le char de début en fonction du nombre de colonnes */
      if (nb_columns > 0) {
        lcd.write(1); // Char début 1 / 1
        nb_columns -= 1;

      } else {
        lcd.write((byte) 0); // Char début 0 / 1
      }

    } else if (i == LCD_NB_COLUMNS -1) { // Derniére case

      /* Affiche le char de fin en fonction du nombre de colonnes */
      if (nb_columns > 0) {
        lcd.write(6); // Char fin 1 / 1

      } else {
        lcd.write(5); // Char fin 0 / 1
      }

    } else { // Autres cases

      /* Affiche le char adéquat en fonction du nombre de colonnes */
      if (nb_columns >= 2) {
        lcd.write(4); // Char div 2 / 2
        nb_columns -= 2;

      } else if (nb_columns == 1) {
        lcd.write(3); // Char div 1 / 2
        nb_columns -= 1;

      } else {
        lcd.write(2); // Char div 0 / 2
      }
    }
  }
}
void setup() {

  setup_progressbar();
  pinMode(sensor1,INPUT);
  pinMode(sensor2,INPUT);
  pinMode(sensor3,INPUT);
//  pinMode(button_change_dis, INPUT);
  pinMode(button_reset_data, INPUT);
  

  Serial.begin(9600);

  lcd.begin ();   
  lcd.backlight();                                          

  lcd.print(" > Autocounter <");                    
  
  lcd.setCursor(0, 1);                                        
  
  lcd.print("   Piggy Bank ");  
    delay(2000);

 lcd.clear();
 draw_progressbar(0,"starting    ");
 delay(500);
 draw_progressbar(40,"Loading    ");
 
  if ( EEPROM.read ( 0 ) != 0xff ){
    
     sum_money = EEPROM.read ( 0 );
     coin_1 = EEPROM.read ( 8 );
     coin_5 = EEPROM.read ( 16 );
     coin_10 = EEPROM.read ( 24 );
  
}
 delay(1000);

  draw_progressbar(60,"Rich!!!   ");
 
 delay(500);
 draw_progressbar(100," success  ");
 delay(1000);
 
 lcd.clear();
 lcd.setCursor(0, 0); 

   Serial.println(" Piggy Bank : Started ");
  Serial.println("type help for a list of available commands ");
  
}

void disTotal_money(){
  
    lcd.setCursor(0, 0); 
    lcd.print("Money");
    lcd.setCursor(7, 0); 
    lcd.print(sum_money);
    lcd.setCursor(12, 0); 
    lcd.print("Baht");


      lcd.setCursor(0, 1); 
      lcd.print("coin 1 = ");
      lcd.setCursor(10, 1); 
      lcd.print(coin_1);
      lcd.setCursor(15, 1); 
      lcd.print("C");


      lcd.setCursor(0, 2); 
      lcd.print("coin 5 = ");
      lcd.setCursor(10, 2); 
      lcd.print(coin_5);
      lcd.setCursor(15, 2); 
      lcd.print("C");


      lcd.setCursor(0, 3); 
      lcd.print("coin 10 = ");
      lcd.setCursor(10, 3); 
      lcd.print(coin_10);
      lcd.setCursor(15, 3); 
      lcd.print("C");

      

//    if(rissing == 0){
//  
//      lcd.setCursor(0, 1); 
//      lcd.print("coin 1 = ");
//      lcd.setCursor(10, 1); 
//      lcd.print(coin_1);
//      lcd.setCursor(15, 1); 
//      lcd.print("C");
//    }else if(rissing == 1){
//      
//      lcd.setCursor(0, 1); 
//      lcd.print("coin 5 = ");
//      lcd.setCursor(10, 1); 
//      lcd.print(coin_5);
//      lcd.setCursor(15, 1); 
//      lcd.print("C");
//      
//      }else if(rissing == 2){
//      
//      lcd.setCursor(0, 1); 
//      lcd.print("coin 10 = ");
//      lcd.setCursor(10, 1); 
//      lcd.print(coin_10);
//      lcd.setCursor(15, 1); 
//      lcd.print("C");
//      
//      }
     
  
  }

void clearAllData(){

  
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }

  sum_money = 0;
  coin_1 = 0;
  coin_5 = 0;
  coin_10 = 0;

  Serial.println("Clear Data, Done !!");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Clear Data, Done !!");
  delay(1500);
  lcd.clear();
  

  
  }

void check_nev_coins(){
  
  if(sum_money < 0){
    sum_money = 0;  
    }
  if(coin_1 < 0){
    coin_1 = 0;  
    }

   if(coin_5 < 0){
    coin_5 = 0;  
    }

    if(coin_10 < 0){
    coin_10 = 0;  
    }
  
  }


  

void loop() {

  check_nev_coins();

unsigned long currentMillis = millis();

if (currentMillis - previousMillis > interval) {
 previousMillis = currentMillis;
 
 EEPROM.write ( 0, sum_money );
  EEPROM.write ( 8, coin_1 );
   EEPROM.write ( 16, coin_5 );
    EEPROM.write ( 24, coin_10 );

    Serial.println("saved Data");
}

  
disTotal_money();


  

    if (Serial.available() > 0) {
               
                a= Serial.readString();

                
                Serial.print("I received: ");
                Serial.println(a);

                if(a == "help"){
                  
//                  Serial.println("type 'dis 1' : Show coin 1 Baht");
//                  Serial.println("type 'dis 5' : Show coin 5 Baht");
//                  Serial.println("type 'dis 10' : Show coin 10 Baht");
//                  Serial.println("type 'dis sc' : Show Scoll All coin");
                  Serial.println("type 'res data' : Show Scoll All coin");
                  }

//                if(a == "dis 1"){
//                  
//                  Serial.println("ok , Show coin 1 Bath");
//                  rissing = 0;
//                  
//                  }
//                if(a == "dis 5"){
//                  
//                  Serial.println("ok , Show coin 5 Bath");
//                  rissing = 1;
//                  
//                  }
//                if(a == "dis 10"){
//                  
//                  Serial.println("ok , Show coin 10 Bath");
//                  rissing = 2;
//                  
//                  }
//                  if(a == "dis sc"){
//                  
//                  Serial.println("ok ,  Show Scoll All coin");
//                  rissing = 3;
//                  
//                  }
                  if(a == "res data"){
                  
                  Serial.println("ok ,  reset data complete !!");
                  clearAllData();
                  
                  }
        }



  
 if (digitalRead(sensor1)==HIGH) { 
    while(digitalRead(sensor1)==HIGH);  
     sum_money = sum_money+1;
    coin_1 = coin_1+1;
    lcd.clear();
    rissing = 0;
  }
  
 if (digitalRead(sensor2)==HIGH) { 
    while(digitalRead(sensor2)==HIGH);   
    sum_money = sum_money+4;
    coin_1 = coin_1-1;
    coin_5 = coin_5+1;
    lcd.clear();
    rissing = 1;
  }

  if (digitalRead(sensor3)==HIGH) { 
    while(digitalRead(sensor3)==HIGH);   
    sum_money = sum_money+5;
    coin_5 = coin_5-1;
    coin_10 = coin_10+1;
    lcd.clear();
    rissing = 2;
  }


//  if (digitalRead(button_change_dis)==HIGH) { 
//    while(digitalRead(button_change_dis)==HIGH);   
//    rissing++;
//    if(rissing>2) rissing=0;
//  }

  

  if (digitalRead(button_reset_data)==HIGH) { 
    while(digitalRead(button_reset_data)==HIGH);   
    clearAllData();
  }

  

 
}
