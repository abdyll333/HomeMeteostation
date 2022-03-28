# HomeMeteostation
This is my implemetation of home meteostation

##Used libs:

RadioHead v-1.121 - http://www.airspayce.com/mikem/arduino/RadioHead/RadioHead-1.121.zip
Adafruit_AHT10 v0.1.0 - https://github.com/adafruit/Adafruit_AHTX0

##Maybe will use!
SoftWire v-2.0.7 - https://github.com/stevemarple/SoftWire


##Message protocol:

Example: 1;23.43;26.68;3.18;25.99;727.48;ffff

where: 
         "1" - id of sensor
         "23.43" - temperature of AHT10
         "26.68" - humidity of AHT10
         "3.18" - counted dew point
         "25.99" - temperature of BMP280
         "727.48" - pressure of BMP280
         ";" - separator
         "ffff" - terminator (end of message)
         
 

