#include <Arduino.h>
#include <DHT.h>

#define DHT_DATA_pin 8

DHT dht;

void setup() {
	Serial.begin(115200);

	// load DHT Data pin
	dht.load(DHT_DATA_pin);
	
	//取一次
	String result=dht.get();
	
	Serial.println(result);
	
	
	//取平均
	double d_temperature=0;
	double d_humidity=0;
	double d_temp,d_hum;
	for (size_t i = 0; i < 10; i++) {
		dht.get(d_temp,d_hum);
		d_temperature=(d_temperature + d_temp)/2;
		d_humidity=(d_humidity + d_hum)/2;
		delay(1000);
	}
	String Data="";
	Data+=d_temperature;
	Data+=",";
	Data+=d_humidity;
	
	Serial.println(Data);


}
