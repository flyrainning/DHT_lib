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

	//取温湿度数值
	double d_temp,d_hum;
	dht.get(d_temp,d_hum);
	Serial.println(d_temp);
	Serial.println(d_hum);

	//取平均

	String Data="";
	//取3次平均值，每次间隔1000ms
	Data=dht.get_a(3,1000);
	//如果需要单独取数值
	dht.get_a(d_temp,d_hum,3,1000);
	Serial.println(d_temp);
	Serial.println(d_hum);
	Serial.println(Data);


}
