#include <DHT.h>

#define DHTPIN       13 // Pin D13 Arduino. / DHT22 sensor

#define FAN1PWM      3  // Pin D3 Arduino. / Fan 1 RWM from 0 to 255
#define FAN2PWM      4  // Pin D4 Arduino. / Fan 2 RWM from 0 to 255
#define FAN3PWM      5  // Pin D5 Arduino. / Fan 3 RWM from 0 to 255
#define FAN4PWM      6  // Pin D6 Arduino. / Fan 4 RWM from 0 to 255
#define FAN5PWM      2  // Pin D7 Arduino. / Fan 5 RWM from 0 to 255

dht DHT;

//DHT22 variables
float hum;  //Stores humidity value
float temp; //Stores temperature value

//FAN variables
int   speedFan = 0;
int   speed_1 = 50; // Speed Fan 1 0-255
int   speed_2 = 50; // Speed Fan 2 0-255
int   speed_3 = 50; // Speed Fan 3 0-255
int   speed_4 = 50; // Speed Fan 4 0-255
int   speed_5 = 50; // Speed Fan 5 0-255
 
void setup () {
  Serial.begin(9600);
}

int getSpeed(int temperature){
 return max(0,(temperature-20)*255/50);
}
 
void loop() {
  //Read data and store it to variables hum and temp
  int chk = DHT.read22(DHTPIN);
  
  Serial.print("DHT22 Status              = ");
  switch (chk){
    case DHTLIB_OK:  
      Serial.println("OK"); 
      break;
    case DHTLIB_ERROR_CHECKSUM: 
      Serial.println("Checksum error"); 
      break;
    case DHTLIB_ERROR_TIMEOUT: 
      Serial.println("Time out error"); 
      break;
    default: 
      Serial.println("Unknown error"); 
      break;
  }

  hum = DHT.humidity;
  temp= DHT.temperature;

  Serial.print("Humidity                  = ");
  Serial.print(hum);
  Serial.println(" %");
  Serial.print("Temp                      = ");
  Serial.print(temp);
  Serial.println(" °C");
  if (!isnan(temp)){
    speedFan = getSpeed(temp);
  }
  speed_1 = speedFan;
  speed_2 = speedFan;
  speed_3 = speedFan;
  speed_4 = speedFan;
  speed_5 = speedFan;

  analogWrite(FAN1PWM, speed_1);              // RPM du fan 1
  analogWrite(FAN2PWM, speed_2);              // RPM du fan 2
  analogWrite(FAN3PWM, speed_3);              // RPM du fan 3
  analogWrite(FAN4PWM, speed_4);              // RPM du fan 4
  analogWrite(FAN5PWM, speed_5);              // RPM du fan 5

  Serial.print("Speed fan                 = ");
  Serial.println(speedFan);

  Serial.println();
  delay(10000);
}
