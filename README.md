# Wort-Thermometer
Simple wort temperature controllerOne Wire Digital Temperature Sensor - DS18B20  using arduino

Parts list: 
Arduino Uno https://www.amazon.com/Arduino-Uno-R3-Microcontroller-A000066/dp/B008GRTSV6
One Wire Digital Temperature Sensor - DS18B20 https://www.sparkfun.com/products/245
40 AMP solid state relay
2.8" TFT touch Screen https://www.adafruit.com/product/1651?gclid=EAIaIQobChMIypPA7rv71wIV2YuzCh15fwnkEAQYAiABEgJrGvD_BwE

This program creates a controller using an arduino, shields, sensors and a solid state relay. The program generates a user interface on the TFT touch screen that displays the current temperature, temperature setpoint and has button to increase and decrease the temperature setpoint. The program monitors the setpoint and the current temp when the values match. Output pin2 is set high which will toggle the SSR controlled to an electric 240V heater coil in the brew kettle. 


