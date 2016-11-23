//
//    FILE: DHT.cpp
//  AUTHOR: FlyRainning
//
// inspired by DHT11 library
//
// Released to the public domain
//

#include "DHT.h"

/////////////////////////////////////////////////////
//
// PUBLIC
//

DHT::DHT(){}
DHT::DHT(int _PIN){
  load(_PIN);
}
void DHT::load(int _PIN){
  PIN=_PIN;
}

// return values:
// DHTLIB_OK
// DHTLIB_ERROR_CHECKSUM
// DHTLIB_ERROR_TIMEOUT
int DHT::read11(uint8_t pin)
{
    // READ VALUES
    int rv = _readSensor(pin, DHTLIB_DHT11_WAKEUP);
    if (rv != DHTLIB_OK)
    {
        humidity    = DHTLIB_INVALID_VALUE; // invalid value, or is NaN prefered?
        temperature = DHTLIB_INVALID_VALUE; // invalid value
        return rv;
    }

    // CONVERT AND STORE
    humidity    = bits[0];  // bits[1] == 0;
    temperature = bits[2];  // bits[3] == 0;

    // TEST CHECKSUM
    // bits[1] && bits[3] both 0
    uint8_t sum = bits[0] + bits[2];
    if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;

    return DHTLIB_OK;
}


// return values:
// DHTLIB_OK
// DHTLIB_ERROR_CHECKSUM
// DHTLIB_ERROR_TIMEOUT
int DHT::read(uint8_t pin)
{
    // READ VALUES
    int rv = _readSensor(pin, DHTLIB_DHT_WAKEUP);
    if (rv != DHTLIB_OK)
    {
        humidity    = DHTLIB_INVALID_VALUE;  // invalid value, or is NaN prefered?
        temperature = DHTLIB_INVALID_VALUE;  // invalid value
        return rv; // propagate error value
    }

    // CONVERT AND STORE
    humidity = word(bits[0], bits[1]) * 0.1;
    temperature = word(bits[2] & 0x7F, bits[3]) * 0.1;
    if (bits[2] & 0x80)  // negative temperature
    {
        temperature = -temperature;
    }

    // TEST CHECKSUM
    uint8_t sum = bits[0] + bits[1] + bits[2] + bits[3];
    if (bits[4] != sum)
    {
        return DHTLIB_ERROR_CHECKSUM;
    }
    return DHTLIB_OK;
}

/////////////////////////////////////////////////////
//
// PRIVATE
//

// return values:
// DHTLIB_OK
// DHTLIB_ERROR_TIMEOUT
int DHT::_readSensor(uint8_t pin, uint8_t wakeupDelay)
{
    // INIT BUFFERVAR TO RECEIVE DATA
    uint8_t mask = 128;
    uint8_t idx = 0;

    // replace digitalRead() with Direct Port Reads.
    // reduces footprint ~100 bytes => portability issue?
    // direct port read is about 3x faster
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
    volatile uint8_t *PIR = portInputRegister(port);

    // EMPTY BUFFER
    for (uint8_t i = 0; i < 5; i++) bits[i] = 0;

    // REQUEST SAMPLE
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW); // T-be
    delay(wakeupDelay);
    digitalWrite(pin, HIGH);   // T-go
    delayMicroseconds(40);
    pinMode(pin, INPUT);

    // GET ACKNOWLEDGE or TIMEOUT
    uint16_t loopCntLOW = DHTLIB_TIMEOUT;
    while ((*PIR & bit) == LOW )  // T-rel
    {
        if (--loopCntLOW == 0) return DHTLIB_ERROR_TIMEOUT;
    }

    uint16_t loopCntHIGH = DHTLIB_TIMEOUT;
    while ((*PIR & bit) != LOW )  // T-reh
    {
        if (--loopCntHIGH == 0) return DHTLIB_ERROR_TIMEOUT;
    }

    // READ THE OUTPUT - 40 BITS => 5 BYTES
    for (uint8_t i = 40; i != 0; i--)
    {
        loopCntLOW = DHTLIB_TIMEOUT;
        while ((*PIR & bit) == LOW )
        {
            if (--loopCntLOW == 0) return DHTLIB_ERROR_TIMEOUT;
        }

        uint32_t t = micros();

        loopCntHIGH = DHTLIB_TIMEOUT;
        while ((*PIR & bit) != LOW )
        {
            if (--loopCntHIGH == 0) return DHTLIB_ERROR_TIMEOUT;
        }

        if ((micros() - t) > 40)
        {
            bits[idx] |= mask;
        }
        mask >>= 1;
        if (mask == 0)   // next byte?
        {
            mask = 128;
            idx++;
        }
    }
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);

    return DHTLIB_OK;
}
String DHT::get(){
  double w=0;
  double s=0;
  return get(w,s);
}
String DHT::get(double &d_w,double &d_s){

  String Data="";

  int chk = read11(PIN);

    switch (chk)
    {
      case DHTLIB_OK:

        d_w=temperature;
        d_s=humidity;
        break;
      case DHTLIB_ERROR_CHECKSUM:
  //		Serial.print("Checksum error,\t");
        break;
      case DHTLIB_ERROR_TIMEOUT:
  //		Serial.print("Time out error,\t");
        break;
      default:
  //		Serial.print("Unknown error,\t");
        break;
    }
     Data+=d_w;
     Data+=",";
     Data+=d_s;

     return Data;
}

String DHT::get_a(double &d_w,double &d_s,int count,long delay_ms){
  double d_temperature=0;
  double d_humidity=0;
  double d_temp,d_hum;

  for (size_t i = 0; i < count; i++) {
    get(d_temp,d_hum);
    d_temperature += d_temp;
    d_humidity += d_hum;
    delay(delay_ms);
  }
  d_w = d_temperature / count;
  d_s = d_humidity / count;

  String Data="";
  Data+=d_w;
  Data+=",";
  Data+=d_s;

  return Data;
}
String DHT::get_a(int count,long delay_ms){
  double w=0;
  double s=0;
  return get_a(w,s,count,delay_ms);

}

//
// END OF FILE
//
