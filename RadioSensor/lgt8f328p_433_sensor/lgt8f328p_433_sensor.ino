#include <RH_ASK.h> //For 433 module
#include <Adafruit_AHT10.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <math.h>

char SENSOR_ID = '1';
char SEPARATOR = ';';
String END_MEASSAGE = "ffff";

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
  if(temp > 0 && humidity > 0) {
    float correct_humidity = humidity/100.0F;
    const float a = 17.27;
    const float b = 237.7;
    float nu_T_RH = (a*temp)/(b+temp)+log(correct_humidity);
    return (b*nu_T_RH)/(a-nu_T_RH); //Dew point formula
  }
  else return 0;
}

void loop() {

    String message = "";
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
    float dew_point = count_dew_point(temp.temperature, humidity.relative_humidity);
    float tempBMP280 = bmp.readTemperature();
    float pressureBMP280 = bmp.readPressure()*0.007501F;
    message += SENSOR_ID;
    message += SEPARATOR;
    message += String(temp.temperature,2);
    message += SEPARATOR;
    message += String(humidity.relative_humidity,2);
    message += SEPARATOR;
    message += String(dew_point,2);
    message += SEPARATOR;
    message += String(tempBMP280,2);
    message += SEPARATOR;
    message += String(pressureBMP280,2);
    message += SEPARATOR;
    message += END_MEASSAGE;
    
    Serial.println("*********************Measuring!********************");
    Serial.print("Temperature AHT10: ");
    Serial.print(temp.temperature);
    Serial.println(" *C");
    Serial.print("Humidity AHT10: ");
    Serial.print(humidity.relative_humidity);
    Serial.println("% rH");
    Serial.print("Dew point AHT10: ");
    Serial.print(dew_point);
    Serial.println(" *C (+/-0.4 *C)");
    Serial.print(F("Temperature BMP280 = "));
    Serial.print(tempBMP280);
    Serial.println(" *C");
    Serial.print(F("Pressure BMP280 = "));
    Serial.print(pressureBMP280);
    Serial.println(" mm");
    Serial.println("**************************************************");
    
    Serial.println("****************Message sending*******************");
    Serial.print("Message:");
    Serial.println(message);

    //Make it tripple for good receiving
    for(int i = 0;i<3;i++) {
      driver.send((char *)message.c_str(), strlen((char *)message.c_str()));
      driver.waitPacketSent();
    }
    
    Serial.println("**************************************************");
    delay(30000);
}
