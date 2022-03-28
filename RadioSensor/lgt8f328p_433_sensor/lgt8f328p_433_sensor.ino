
#include <RH_ASK.h> //For 433 module
#include <Adafruit_AHT10.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <math.h>


const char* ON_MESSAGE = "Test data!" ;//binary 0101 0101

//setup the driver as such:
// speed: 2400 bits per second
// rxPin - receiving pin: 11 --not used for this transmitting script
// txPin - transmitting pin: 12
// pttPin - transmitting enable pin: 10 --not used for this transmitting script. 
//    RadioHead can enable the transmitter only when transmitting data
//    but this is not supported on the SYN115 breakout board
// pttInverted: true - LOW signal will enable the tranmitter if set to true. Since we don't control the transmitter
//    we want this value to be true


#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

//Create 433 module transmitter
RH_ASK driver(2400, 3, 2, 1, true);
Adafruit_AHT10 aht;
Adafruit_BMP280 bmp(BMP_CS);




void setup() {
  //initialize wireless driver
  Serial.begin(9600);
  Serial.println("Connecting to AHT10!");
  if (! aht.begin()) {
    Serial.println("Could not find AHT10? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 found");

  while ( !Serial ) delay(100);   // wait for native usb
  Serial.println(F("Connecting to BMP280!"));
  while (!bmp.begin()) {
    Serial.println(F("Could not find a valid bmp280SPI sensor, check wiring!"));
    delay(2000);
  }
  driver.init();
}

//Count dew poin with standart formula
float count_dew_point(float &temp,float &humidity) //temp in celsius and humidity in %
{
  float correct_humidity = humidity/100.0F;
  const float a = 17.27;
  const float b = 237.7;
  float nu_T_RH = (a*temp)/(b+temp)+log(correct_humidity);
  return (b*nu_T_RH)/(a-nu_T_RH); //Dew point formula
}




void loop() {
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
    Serial.println("*********************Measuring!********************");
    Serial.print("Temperature AHT10: ");
    Serial.print(temp.temperature);
    Serial.println(" degrees C");
    Serial.print("Humidity AHT10: ");
    Serial.print(humidity.relative_humidity);
    Serial.println("% rH");
    Serial.print("Dew point AHT10: ");
    
    float dew_point = count_dew_point(temp.temperature, humidity.relative_humidity);
    Serial.print(dew_point);
    Serial.println(" degrees C (+-0.4C)");

    Serial.print(F("Temperature BMP280 = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure BMP280 = "));
    Serial.print(bmp.readPressure()*0.007501F);
    Serial.println(" mm");
    Serial.println("**************************************************");

    
    driver.send((uint8_t *)ON_MESSAGE, strlen(ON_MESSAGE)); //send one byte. To send a whole message you can use strlen(MESSAGE)
    driver.waitPacketSent();
    delay(30000);
}
