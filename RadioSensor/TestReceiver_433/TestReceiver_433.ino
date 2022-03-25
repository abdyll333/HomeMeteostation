//Receiver code
#include <RH_ASK.h>
RH_ASK driver(2400, 11, 12, 10, true);

void setup()
{
    driver.init();
    Serial.begin(9600);  
}
void loop()
{
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      
      if(buflen >= 1){
        Serial.write(buf, buflen);
        Serial.println();
      }
    }
}
