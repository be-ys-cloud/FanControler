#include <dht.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCDPIN       A3 // Pin D3 Arduino. / LCD Switch

#define DHTPIN       13 // Pin D13 Arduino. / DHT22 sensor

#define FAN1PWM      5  // Pin D5 Arduino. / Fan 1 RWM from 0 to 255
#define FAN2PWM      6  // Pin D6 Arduino. / Fan 2 RWM from 0 to 255 
#define FAN3PWM      9  // Pin D9 Arduino. / Fan 3 RWM from 0 to 255
#define FAN4PWM      10  // Pin D10 Arduino. / Fan 4 RWM from 0 to 255
#define FAN5PWM      11  // Pin D11 Arduino. / Fan 5 RWM from 0 to 255 

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

dht DHT;

LiquidCrystal_I2C lcd(0x27, 20, 4);

uint8_t check[8] ={
  B00000,
  B00001,
  B00011,
  B10110,
  B11100,
  B01000,
  B00000,
  B00000
};

uint8_t skull[8] ={
  B00000,
  B01110,
  B10101,
  B11011,
  B01110,
  B01110,
  B00000,
  B00000
};

uint8_t bell[8] ={
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  B00000
};

//SigmaLambdaPi letters
uint8_t sigma[8] = {
  B11111,
  B01000,
  B00100,
  B00010,
  B00100,
  B01000,
  B11111
};
uint8_t lambda[8] = {
  B00100,
  B00100,
  B01010,
  B01010,
  B01010,
  B10001,
  B10001
};
uint8_t pi[8] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001
};

//DHT22 variables
float temp; //Stores temperature value
String dhtState;
int dhtStateLCD;

//FAN variables
int speedFan = 0;
int percentValue = 0;
 
void setup () {
  Serial.begin(9600);
  lcd.init(); // initialisation de l'afficheur
  lcd.createChar(0, sigma);
  lcd.createChar(1, lambda);
  lcd.createChar(2, pi);  
  lcd.createChar(3, check);
  lcd.createChar(4, skull);
  lcd.createChar(5, bell);

  pinMode(LCDPIN, INPUT_PULLUP);
}

int getSpeed(int temperature){
 return max(0,(temperature-20)*255/50);
}

void StopLCD(){
  lcd.noDisplay();
  lcd.noBacklight();
}

void StartLCD(){
  lcd.display();
  lcd.backlight();
}

uint16_t wait = 0;

void loop() {
  if (wait % 100==0){
    wait=0;
    //Read data and store it to variables hum and temp
    int chk = DHT.read22(DHTPIN);
    
    switch (chk){
      case DHTLIB_OK:  
        dhtState="OK"; 
        dhtStateLCD=3;
        break;
      case DHTLIB_ERROR_CHECKSUM: 
        dhtState="Checksum error"; 
        dhtStateLCD=5;
        break;
      case DHTLIB_ERROR_TIMEOUT: 
        dhtState="Time out error";
        dhtStateLCD=5; 
        break;
      default: 
        dhtState="Unknown error";
        dhtStateLCD=4; 
        break;
    }
    Serial.print("DHT22 Status              = ");
    Serial.println(dhtState);
    
    temp= DHT.temperature;
    
    if (temp!=-999.00){
      speedFan = getSpeed(temp);
    }else{
      temp=0;
    }
  
    Serial.print("Temp                      = ");
    Serial.print(temp);
    Serial.println(" °C");

    analogWrite(FAN1PWM, speedFan);              // RPM du fan 1
    analogWrite(FAN2PWM, speedFan);              // RPM du fan 2
    analogWrite(FAN3PWM, speedFan);              // RPM du fan 3
    analogWrite(FAN4PWM, speedFan);              // RPM du fan 4
    analogWrite(FAN5PWM, speedFan);              // RPM du fan 5
  
    Serial.print("Speed fan                 = ");
    Serial.println(speedFan);
    Serial.println();
    
    percentValue = (int)((speedFan/255.0)*100.0);
    
    lcd.setCursor(0, 0);   
    
    lcd.printByte(0);
    lcd.printByte(1);
    lcd.printByte(2);
    
    lcd.print(String(" - Status: "));
    lcd.printByte(dhtStateLCD);
    lcd.setCursor(0,1);
    lcd.print(String(String(temp)+(char)223+" Spd:"+percentValue+(char)37));
    lcd.print(String("   "));
  }
  
  if(digitalRead(LCDPIN)==HIGH){
    StartLCD();
  }else{
    StopLCD();
  }
  
  wait++;
  delay(50);
}
