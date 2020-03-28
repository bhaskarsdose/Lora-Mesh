#include <SoftwareSerial.h>
#include <SPI.h>
#include <LoRa.h>

SoftwareSerial NodeMCU(2, 3);

String final_string;
long lastMsg = 0;

char x;
int i = 0;
String node;

String pm1_0;
String pm2_5;
String pm10;
//String Co2telra;
String temperature;
String humidity;
String pressure;
//String Co2css;
//String Tvoc;

void setup()
{
//  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(9600);
  delay(500);
  Serial.println();
  //Serial.print("MAC: ");
  //Serial.println(WiFi.macAddress());
  NodeMCU.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }


}



void loop()
{

  while (NodeMCU.available() > 0)
  {
    //    xx = NodeMCU.read();
    x = NodeMCU.read();
    //  Serial.println(x);
    node = node + x;


    if (i == 1)
    {
      if (x != 'H')
      {
        pm1_0 = pm1_0 + x;
      }

    }
    else if (i == 2)
    {
      if (x != 'I')
        pm2_5 = pm2_5 + x;

    }
    else if (i == 3)
    {
      if (x != 'L')
        pm10 = pm10 + x;

    }
    else if (i == 4)
      /*{
        if(x!='L')
         Co2telra=Co2telra+x;

        }
        else if(i==5)*/
    {
      if (x != 'M')
        pressure = pressure + x;

    }
    else if (i == 5)
    {
      if (x != 'N')
        humidity = humidity + x;

    }
    else if (i == 6)
    {
      if (x != 'Z')
        temperature = temperature + x;
    }

    //else if(i==8)
    /*{
      if(x!='H')
      Co2css=Co2css+x;

      }
      else if(i==9)
      {
      if(x!='I')
      Tvoc=Tvoc+x;

      }*/

    if (x == 'G')
    {
      //node = "";
      pm1_0 = "";
      pm2_5 = "";
      pm10 = "";
      humidity = "";
      temperature = "";
      pressure = "";
      //Co2telra="";
      //Co2css="";
      //Tvoc="";

      i = 1;

    }

    else  if (x == 'H')

    {
      i = 2;

    }
    else if (x == 'I')
    {
      i = 3;

    }
    else if (x == 'L')
    {
      i = 4;

    }
    else if (x == 'M')
    {
      i = 5;

    }
    else if (x == 'N')
    {
      i = 6;

    }
    /*else if(x=='Z')
      {
      i=7;

      }
      else if(x=='G')
      {
      i=8;

      }


      else if(x=='H')

      {
      i=9;
      }*/
    else if (x == 'Z')
    {
      i = 7;

      //String stringOne = String("\n{") + String("\n\t\"DeviceId\":\"") + mqttDeviceID + String("\"") + String(",");
      //String stringOne = String("\n{"\n Device id\": ) + mqttDeviceID + String(",");
      String stringTwo = String("\n\t\"pm1_0\"") + String(":") + String("\"") + pm1_0 + String("\"") + String(",");
      String stringThree = String("\n\t\"pm2_5\"") + String(":") + String("\"") + pm2_5 + String("\"") + String(",");
      String stringFour = String("\n\t\"pm10\"") + String(":") + String("\"") + pm10 + String("\"") + String(",");

      //String stringFive = String("\n\t\"telaireco2\"")+String(":")+String("\"")+Co2telra+String("\"")+String(",");
      String stringFive = String("\n\t\"temperature\"") + String(":") + String("\"") + temperature + String("\"") + String(",");
      String stringSix = String("\n\t\"humidity\"") + String(":") + String("\"") + humidity + String("\"") + String(",");

      String stringSeven = String("\n\t\"pressure\"") + String(":") + String("\"") + pressure + String("\"") + String("\n}");

      //String stringNine = String("\n\t\"ccs811co2\"")+String(":")+String("\"")+Co2css+String("\"")+String(",");
      //String stringTen = String("\n\t\"tvoc\"")+String(":")+String("\"")+Tvoc+String("\"")+String("\n}");

      String final_string = stringTwo + stringThree + stringFour + stringFive + stringSix + stringSeven;
      //Serial.println(final_string);
      //char final_buf[350];
      //final_string.toCharArray(final_buf, sizeof(final_buf));
      //Serial.println(final_buf);
      //client.publish("Eziosense", final_buf);
    
   Serial.print("Sending packet: ");
   Serial.println(final_string);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(final_string);
  LoRa.endPacket();
  delay(5000);
  }
   
  }

}
