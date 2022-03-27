
#include <RH_ASK.h>

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


RH_ASK driver(2400, 11, 2, 10, true);

void setup() {
  //initialize wireless driver
  driver.init();
}

void loop() {
    driver.send((uint8_t *)ON_MESSAGE, strlen(ON_MESSAGE)); //send one byte. To send a whole message you can use strlen(MESSAGE)
    driver.waitPacketSent();
    delay(1000);
}
