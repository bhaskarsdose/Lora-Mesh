#include <SPI.h>              // include libraries
#include <LoRa.h>
#include <SoftwareSerial.h>
//uart start
SoftwareSerial NodeMCU(2, 3);
const long frequency = 433E6;  // LoRa Frequency
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
String final_string;
//uart end

const int csPin = 10;          // LoRa radio chip select
const int resetPin = 9;        // LoRa radio reset
const int irqPin = 2;          // change for your board; must be a hardware interrupt pin

void setup() {
  Serial.begin(9600);                   // initialize serial
  delay(500);
  Serial.println();
  NodeMCU.begin(9600);
  while (!Serial);

  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(frequency)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
  Serial.println();
  Serial.println("LoRa Simple Node");
  Serial.println("Only receive messages from gateways");
  Serial.println("Tx: invertIQ disable");
  Serial.println("Rx: invertIQ enable");
  Serial.println();

//  LoRa.onReceive(onReceive);
  LoRa_rxMode();
}

void loop() {
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
      Serial.println(final_string);
      //char final_buf[350];
      //final_string.toCharArray(final_buf, sizeof(final_buf));
      //Serial.println(final_buf);
      //client.publish("Eziosense", final_buf);
    }

  }  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  if (runEvery(1000)) { // repeat every 1000 millis

    //String message = "HeLoRa World! ";
    //message += "I'm a Node! ";
    //message += millis();

    LoRa_sendMessage(final_string); // send a message

    Serial.println("Send Message!");
  }
}

void LoRa_rxMode(){
  LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode
}

void LoRa_sendMessage(String message) {
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket();                     // finish packet and send it
  LoRa_rxMode();                        // set rx mode
}

void onReceive(int packetSize) {
  String message = "";

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }

  Serial.print("Node Receive: ");
  Serial.println(message);

}

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}

