# arduino-weather-station
Dust sensor plus simple weather station built with arduino.
The station measures the amount of dust in the air, temperature and humidity.
Measuremetnts are displayed on an LCD.

## Shopping list
* [SDS021 Air Quality Sensor](https://www.aliexpress.com/wholesale?catId=0&initiative_id=SB_20180222113508&SearchText=SDS021)
* [DHT 22 Temperature/Humidity sensor](http://www.santy.cz/senzory-c24/sa0306-i329/)
* [LCD Display](http://www.santy.cz/zobrazovace-c7/lcd-1602-yellow-i2c-i171/)
* [Arduino Leonardo Clone](http://www.santy.cz/arduino-usb-c15/arduino-leonardo-atmel-i59/)

## Installing the Dependencies
The code is built on [PlatformIO](https://platformio.org/). To run the code, you will need to
[install PlatformIO](http://docs.platformio.org/en/latest/installation.html) first. TL;DR:
````
pip install -U platformio
```

## Building the Code
To build the code and run it on the arduino, run the following from the toplevel directory:
```
platformio run -t upload
```

