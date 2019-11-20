# FanContoler

Ce programme permet de controler la vitesse de 5 ventilateurs en fonction de la température et d'afficher les informations sur un écran LCD.

## Hardware

### Composants

Matériel nécessaire: 

- 5 ventilateurs PWM (Pulse Width Modulation)
- une sonde de température DHT22
- un résistance de 4.7k
- un écran OLED I2C Display SSD1306 128x32
- Arduino
- Une alimentation 12V

### Montage

![Montage](images/FanControler.png)

### Connexions

Le programme fonctionne avec les branchements suivant:

- D5 : PWM Command Fan #1
- D6 : PWM Command Fan #2
- D9 : PWM Command Fan #3
- D10 : PWM Command Fan #4
- D11 : PWM Command Fan #5
- D13 : Signal DHT22
- A5 : SDA I2C OLED
- A4 : SLC I2C OLED
- 5V : VCC DHT22
- 5V : VCC I2C OLED
- GND : GND 12V DC
- GND : GND I2C OLED
- GND : GND DHT22

## Software

Le programme lit la température sur la sonde thermiquie, puis ajuste la vitesse des ventilateurs dans la plage 20°C à 70°C.

Il faut ajouter la lib [DHTLIB](https://github.com/be-ys/Arduino/tree/master/libraries/DHTlib) au projet, et inclure les bibliothèques Arduino LiquidCrystal version 1.0.7, Adafruit GFX Library 1.5.7, Adafruit SSD1306 version 1.3.0 et LuiquidCrystal I2C 1.1.2.

