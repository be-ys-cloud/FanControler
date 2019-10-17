#include <dht.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN       13 // Pin D13 Arduino. / DHT22 sensor
#define FAN1PWM      5  // Pin D5 Arduino. / Fan 1 RWM from 0 to 255
#define FAN2PWM      6  // Pin D6 Arduino. / Fan 2 RWM from 0 to 255 
#define FAN3PWM      9  // Pin D9 Arduino. / Fan 3 RWM from 0 to 255
#define FAN4PWM      10  // Pin D10 Arduino. / Fan 4 RWM from 0 to 255
#define FAN5PWM      11  // Pin D11 Arduino. / Fan 5 RWM from 0 to 255 

//DHT22 variables
dht DHT;
float temp; //Stores temperature value
String dhtState;
int dhtStateLCD;
struct
{
    uint32_t total;
    uint32_t ok;
    uint32_t crc_error;
    uint32_t time_out;
    uint32_t connect;
    uint32_t ack_l;
    uint32_t ack_h;
    uint32_t unknown;
} stat = { 0,0,0,0,0,0,0,0};

//FAN variables
int speedFan = 0;
int percentValue = 0;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

static const unsigned char PROGMEM check[] ={
  B00000,
  B00001,
  B00011,
  B10110,
  B11100,
  B01000,
  B00000,
  B00000
};
static const unsigned char PROGMEM skull[] ={
  B00000,
  B01110,
  B10101,
  B11011,
  B01110,
  B01110,
  B00000,
  B00000
};
static const unsigned char PROGMEM bell[] ={
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  B00000
};
static const unsigned char PROGMEM sigma[] = {
  B11111,
  B01000,
  B00100,
  B00010,
  B00100,
  B01000,
  B11111
};
static const unsigned char PROGMEM lambda[] = {
  B00100,
  B00100,
  B01010,
  B01010,
  B01010,
  B10001,
  B10001
};
static const unsigned char PROGMEM pi[] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001
};
static const unsigned char PROGMEM deg[] = {
  B00010,
  B00101,
  B00010,
  B00000,
  B00000,
  B00000,
  B00000
};

// look at line 27 to 30 of Adafruit_SSD1306.h inside the library to select the dimensions
#if (SSD1306_LCDHEIGHT != 32) // 
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int getSpeed(int temperature){
 return max(0,(temperature-20)*255/50);
}

void setup() {
  Serial.begin(9600);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
//  display.display();
//  delay(2000);
   // Clear the buffer.
  display.clearDisplay();
}

void loop() {
  //Read data and store it to variables hum and temp
  int chk = DHT.read22(DHTPIN);
  
  stat.total++;
  switch (chk){
    case DHTLIB_OK:  
      stat.ok++;
      dhtState="OK"; 
      dhtStateLCD=3;
      break;
    case DHTLIB_ERROR_CHECKSUM: 
      stat.crc_error++;
      dhtState="Checksum error"; 
      dhtStateLCD=5;
      break;
    case DHTLIB_ERROR_TIMEOUT: 
      stat.time_out++;
      dhtState="Time out error";
      dhtStateLCD=5; 
      break;
    case DHTLIB_ERROR_CONNECT:
      stat.connect++;
      dhtState="Connect error";
      dhtStateLCD=4; 
      break;
    case DHTLIB_ERROR_ACK_L:
      stat.ack_l++;
      dhtState="Ack Low error";
      dhtStateLCD=4; 
      break;
    case DHTLIB_ERROR_ACK_H:
      stat.ack_h++;
      dhtState="Ack High error";
      dhtStateLCD=4; 
      break;
    default: 
      stat.unknown++;
      dhtState="Unknown error";
      dhtStateLCD=4; 
      break;
  }
  Serial.print("DHT22 Status              = ");
  Serial.println(dhtState);

  if (stat.total % 10 == 0)
  {
      Serial.println("\nTOT\tOK\tCRC\tTO\tCON\tACK_L\tACK_H\tUNK");
      Serial.print(stat.total);
      Serial.print("\t");
      Serial.print(stat.ok);
      Serial.print("\t");
      Serial.print(stat.crc_error);
      Serial.print("\t");
      Serial.print(stat.time_out);
      Serial.print("\t");
      Serial.print(stat.connect);
      Serial.print("\t");
      Serial.print(stat.ack_l);
      Serial.print("\t");
      Serial.print(stat.ack_h);
      Serial.print("\t");
      Serial.print(stat.unknown);
      Serial.println("\n");
  }
  
  temp= DHT.temperature;
  
  if (chk==DHTLIB_OK){
    speedFan = getSpeed(temp);
  }else{
    temp     = 0;
  }

  Serial.print("Temp                      = ");
  Serial.print(temp);
  Serial.println(" °C");

  // Pause 1 sec entre read digital et write digital
  delay(1000);
  analogWrite(FAN1PWM, speedFan);              // RPM du fan 1
  analogWrite(FAN2PWM, speedFan);              // RPM du fan 2
  analogWrite(FAN3PWM, speedFan);              // RPM du fan 3
  analogWrite(FAN4PWM, speedFan);              // RPM du fan 4
  analogWrite(FAN5PWM, speedFan);              // RPM du fan 5

  Serial.print("Speed fan                 = ");
  Serial.println(speedFan);
  Serial.println();

  // Pause 1 sec entre write digital et write analog
  delay(1000);
  
  display.clearDisplay();
 
  display.drawBitmap(4, 3, sigma, 8, 7, 1); 
  display.drawBitmap(4, 13, lambda, 8, 7, 1); 
  display.drawBitmap(4, 23, pi, 8, 7, 1); 

  robojaxText("Status:", 24, 3, 1, false, 1);
  
  robojaxText("Temp:", 24, 13, 1, false, 1);
  robojaxText((String)temp, 65, 13, 1, false, 1);
  robojaxText("C ", 103, 13, 1, false, 1);
  display.drawBitmap(94, 13, deg, 8, 7, 1); 
  
  robojaxText("Speed:", 24, 23, 1, false, 1);
  robojaxText((String)speedFan, 65, 23, 1, false, 1);
  robojaxText("%", 79, 23, 1, false, 1);
  
  switch (dhtStateLCD){
    case 3 :
    display.drawBitmap(65, 3, check, 8, 8, 1);
    break;
    case 4 :
    display.drawBitmap(65, 3, skull, 8, 8, 1); 
    break;
    case 5 :
    display.drawBitmap(65, 3, bell, 8, 8, 1); 
    break;
  }

   display.display();
  // Pause 4 sec avant reboot
   delay(4000); 
}

void robojaxText(String text, int x, int y,int size, boolean d, uint16_t color) {
  display.setTextSize(size);
  display.setTextColor(color);
  display.setCursor(x,y);
  display.println(text);
  if(d){
    display.display();
  }
}