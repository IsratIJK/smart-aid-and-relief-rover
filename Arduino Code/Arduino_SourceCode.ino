#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define SS_PIN 53
#define RST_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x3F, 20, 4);

int val;
int tempPin = A0;
int fan=7 ;
Servo myServo1;
Servo myServo2;


#define BUTTON_PIN_1 8
#define BUTTON_PIN_2 10
#define BUTTON_PIN_3 12 


String uid1 = "D3 5C 2B 2E";
String uid2 = "E3 6D 11 2E";
String uid3 = "72 FB 4B 1A";
void setup()
{

pinMode(fan,OUTPUT);
digitalWrite(fan,LOW);

  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);

    myServo1.attach(2);
    myServo1.write(0);

    myServo2.attach(3);
    myServo2.write(0);

lcd.init();
lcd.backlight();
lcd.setCursor(1, 0);
lcd.print("Hold the card");
    
SPI.begin();
mfrc522.PCD_Init();
Serial.begin(9600);

}
void loop()
{
  
  
  
val = analogRead(tempPin);
float mv = ( val/1024.0)*5000; 
float cel = mv/10;


if(cel<80)
{
  digitalWrite(fan,HIGH);
}
else
{ digitalWrite(fan,LOW);
}
  int button1State = digitalRead(BUTTON_PIN_1);
  int button2State = digitalRead(BUTTON_PIN_2);
  int button3State = digitalRead(BUTTON_PIN_3);


  if (button1State == LOW)
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("rice");
    Serial.println("rice");
    delay(1000);
    lcd.clear();
    
    lcd.setCursor(1, 0);
    lcd.print("Select Item");
    lcd.setCursor(0, 1);
    lcd.print("rice other quit");
    
    Serial.print("Select Item 1.rice 2.other 3.quit");
    

    myServo1.write(45);
    delay(1000);
    myServo1.write(0);
    delay(1000);



  }

  if (button2State == LOW)
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("others");
    Serial.println("others");
    delay(1000);
    lcd.clear();

    lcd.setCursor(1, 0);
    lcd.print("Select Item");
    lcd.setCursor(0, 1);
    lcd.print("rice other quit");
    
    Serial.print("Select Item 1.rice 2.other 3.quit");


    myServo2.write(45);
    delay(1000);
    myServo2.write(0);
    delay(1000);
  }
  if (button3State == LOW)
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Hold the card");
    Serial.println("Hold the card");


  }
if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }

if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  Serial.print("UID tag:");
  String content = "";
  byte letter;

  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  lcd.clear();
  Serial.println();
  Serial.print("Message: ");
  content.toUpperCase();

  if (content.substring(1) == uid1 || content.substring(1) == uid2 || content.substring(1) == uid3)
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Authorized");
    delay(2000);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Select Item");
    lcd.setCursor(0, 1);
    lcd.print("rice other quit");
    delay(1000);


  }
  else
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Not Authorized");
    lcd.setCursor(0, 1);
    lcd.print("Please quit");
    delay(500);
  }


}