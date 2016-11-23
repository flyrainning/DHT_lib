//
//    FILE: DHT.h
//  AUTHOR: FlyRainning
// VERSION: 0.1.0
// PURPOSE: DHT Temperature & Humidity Sensor library for Arduino
//     URL: http://www.fengpiao.net
//
//

#ifndef DHT_h
#define DHT_h

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#define DHT_LIB_VERSION "0.1.0"

#define DHTLIB_OK                0
#define DHTLIB_ERROR_CHECKSUM   -1
#define DHTLIB_ERROR_TIMEOUT    -2
#define DHTLIB_INVALID_VALUE    -999

#define DHTLIB_DHT11_WAKEUP     18
#define DHTLIB_DHT_WAKEUP       1

// max timeout is 100 usec.
// For a 16 Mhz proc 100 usec is 1600 clock cycles
// loops using DHTLIB_TIMEOUT use at least 4 clock cycli
// so 100 us takes max 400 loops
// so by dividing F_CPU by 40000 we "fail" as fast as possible
#define DHTLIB_TIMEOUT (F_CPU/40000)

class DHT
{
public:
    DHT();
    DHT(int _PIN);
    void load(int _PIN);
    //取得一次结果
    String get(double &d_w,double &d_s);
    String get();
    //取得平均结果
    String get_a(double &d_w,double &d_s,int count,long delay_ms=1000);
    String get_a(int count,long delay_ms=1000);

    // return values:
    // DHTLIB_OK
    // DHTLIB_ERROR_CHECKSUM
    // DHTLIB_ERROR_TIMEOUT
    int read11(uint8_t pin);
    int read(uint8_t pin);

    inline int read21(uint8_t pin) { return read(pin); };
    inline int read22(uint8_t pin) { return read(pin); };
    inline int read33(uint8_t pin) { return read(pin); };
    inline int read44(uint8_t pin) { return read(pin); };

    double humidity;
    double temperature;
    int PIN;



private:
    uint8_t bits[5];  // buffer to receive data
    int _readSensor(uint8_t pin, uint8_t wakeupDelay);
};
#endif
//
// END OF FILE
//
