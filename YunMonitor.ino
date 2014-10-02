//==Declarations======
//  Sensors
//    DHT22
#include <DHT.h>
#define DHTPIN 3
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
//    Light Sensor
const int lightPin = A0;

//  Console
#include <Console.h>
#include <Bridge.h>
#include <YunClient.h>
#include <YunServer.h>

//  Remote host site
char server[] = "data.sparkfun.com";

//  Phant Stuff
const String publicKey = "q55vdxMXjJiwpvzx4MGo";
const String privateKey = "BVVlp7dAwBH9lrkXa65Y";
const byte NUM_FIELDS = 3;
const String fieldNames[NUM_FIELDS] = {"Humidity", "Light", "Temperature"};
String fieldData[NUM_FIELDS];

YunClient client;

void setup() 
{
  dht.begin();
  Console.begin();
  
}

void loop() 
{
  //  Reading Sensor Data
  int lightVolts = analogRead( lightPin );
  float fLight = ( float )lightVolts*100/1023;
  //    read the temperature and humidity on pin 1 (as defined in the definition section)
  float fTemp = dht.readTemperature(); 
  float fHumid = dht.readHumidity();  
  
  serialPrint ( fLight, fTemp, fHumid );
  
  //  Formatting Data 
  formatSensorData ( fLight, fTemp, fHumid );
  
  //  Output Data
  postData();
  delay( 3000 );
}

void serialPrint ( float fLight, float fTemp, float fHumid )
{
  Serial.print( "Temperature: " );
  Serial.print( fTemp ); 
  Serial.println( " degrees Celsius" );
  
  Serial.print( "Light Percentage: " );
  Serial.print( fLight ); 
  Serial.println( "%" );
  
  Serial.print( "Humidity: " );
  Serial.print( fHumid ); 
  Serial.println( "%" );  
  
  return;
}

void formatSensorData ( float fLight, float fTemp, float fHumid )
{
  fieldData[0] = String( int( fHumid ) ) + "." + String( int( ( fHumid - int( fHumid ) ) * 100 ) );
  fieldData[1] = String( int( fLight ) ) + "." + String( int( ( fLight - int( fLight ) ) * 100 ) ); //Light
  fieldData[2] = String( int( fTemp ) ) + "." + String( int( ( fTemp - int( fTemp ) ) * 100 ) ); //temperature
  
  return;
}

void postData()
{ 
  if ( !client.connect( server, 80 )
  {
      Serial.println( "Ruh roh. Something bad happened" );
  }
  client.print("GET /input/");
  client.print(publicKey);
  client.print("?private_key=");
  client.print(privateKey);
  for (int i=0; i<NUM_FIELDS; i++)
  {
    client.print("&");
    client.print(fieldNames[i]);
    client.print("=");
    client.print(fieldData[i]);
  }
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println();
}
