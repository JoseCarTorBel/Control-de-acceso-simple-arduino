#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#include <Wire.h>
#include <DS3231.h>

// Iniciamos los pins de la lcd
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);


// Iniciamos los pins del lector
#define RST_PIN 5
#define SS_PIN 53

// Iniciamos los pins de los leds
#define VERDE 3
#define ROJO 2
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Iniciamos los pins del clock
DS3231 clock;
RTCDateTime dt;

/* Set your new UID here! */
#define NEW_UID {0xDE, 0xAD, 0xBE, 0xEF}
MFRC522::MIFARE_Key key;


void setup() {
    
    clock.begin();
    clock.setDateTime(__DATE__, __TIME__);

    
    pinMode(VERDE,OUTPUT);
    pinMode(ROJO,OUTPUT);

    digitalWrite(VERDE,LOW);
    digitalWrite(ROJO,LOW);
   
    Serial.begin(9600);  // Initialize serial communications with the PC
    while (!Serial);     // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();         // Init SPI bus
    mfrc522.PCD_Init();  // Init MFRC522 card
    Serial.println(F(""));
    
    lcd.begin(32, 2);
    lcd.print("Autentificar");

    for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
    }


    
}

void loop() {
  lcd.setCursor(0,0);
  dt = clock.getDateTime();
  lcd.print(clock.dateFormat("d-m-Y H:i:s", dt));
  Serial.println(clock.dateFormat("d-m-Y H:i:s", dt));
  

  
  // put your main code here, to run repeatedly:
  lcd.setCursor(0,1);
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }
  
  if (mfrc522.uid.uidByte[0] == 0x77 &&  mfrc522.uid.uidByte[1] == 0x60 && mfrc522.uid.uidByte[2] == 0xbb && mfrc522.uid.uidByte[3] == 0x1c) {
    lcd.print("Acceso permitido");
    analogWrite(VERDE,20);
  }else{
    lcd.print("Acceso denegado");
    analogWrite(ROJO,20);
  }
  delay(1000);
  analogWrite(VERDE,0);
  analogWrite(ROJO,0);
  
  lcd.setCursor(0,1);
  for(int i=0;i<32;i++){
    lcd.setCursor(i,1);
    lcd.print(" ");
  }
  
  
}
