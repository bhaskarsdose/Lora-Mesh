// LoRa Communication with Arduino
// Definition of libraries to use in the project
#include <SPI.h>             
#include <LoRa.h>
#include <ArduinoJson.h>

//GPS
double a ;
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
static const int RXPin =  3, TXPin = 4;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

//end of gps

//Definition of constants
const int csPin = 10;         // Chip Select (Slave Select do protocolo SPI) do modulo Lora
const int resetPin = 0;       // Reset do modulo LoRa
const int irqPin = 4;         // Pino DI0
 
String outgoing;              // outgoing message
byte localAddress = 0xAA;     // Address of this LoRa device
byte msgCount = 0;            // Sent Message Counter
byte msgHandshakeCount = 0;   // Sent Message Counter
byte destination = 0xFF;      // Device address to send message (0xFF sends to all devices)

long lastSendTime = 0;        // TimeStamp of last message sent
int interval = 15000;         // Message Interval in ms (initial 5s)
 
boolean foundServer = false;

// Function that sends a LoRa message
void sendMessage(String outgoing) 
{
  LoRa.beginPacket();
  LoRa.write(destination);              // Add destination address
  LoRa.write(localAddress);             // Add sender address
    
  LoRa.write(1);

  LoRa.write(msgCount);                 // Message Counter
  LoRa.write(outgoing.length());        // Message Size in bytes
  LoRa.print(outgoing);                // Message Vector
  LoRa.endPacket();                     // Finaliza o pacote e envia
  msgCount++;                           // Contador do numero de mensagnes enviadas
}

// Function that sends a LoRa message
void sendHandshake(String outgoing, byte sender, byte isServer) 
{
  LoRa.beginPacket();                   // Inicia o pacote da mensagem
  LoRa.write(sender);                   // Adiciona o endereco de destino
  
  LoRa.write(localAddress);             // Adiciona o endereco do remetente
  LoRa.write(isServer);                 // isServer Id
  LoRa.write(msgHandshakeCount);        // Contador da mensagem handshake
  LoRa.write(outgoing.length());        // Tamanho da mensagem em bytes
  LoRa.print(outgoing);                 // Vetor da mensagem 
  LoRa.endPacket();                     // Finaliza o pacote e envia
  msgHandshakeCount++;                  // Contador do numero de mensagnes enviadas
  Serial.println("I sent "+outgoing+" para: 0x" + String(sender, HEX));
  Serial.println("");  
}
 
// Function to receive message
void onReceive(int packetSize) 
{
  if (packetSize == 0) return;          // Se nenhuma mesnagem foi recebida, retorna nada
 
  // Read a package, let's decode?
  int recipient = LoRa.read();          // Endereco de quem ta recebendo
  byte sender = LoRa.read();            // Endereco do remetente
  byte handShake = LoRa.read();         // Mensagem
  
  byte incomingMsgId = LoRa.read();     // Mensagem
  byte incomingLength = LoRa.read();    // Tamanho da mensagem
 
  String incoming = "";
 
  while (LoRa.available())
  {
    incoming += (char)LoRa.read();
  }
 
  if (incomingLength != incoming.length()) 
  {   
    // check length for error
    Serial.println("Error !: The message size does not match the content!");
    return;                        
  }
 
  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF)
  {
    Serial.println("This message is not for me.");
    return;                      // skip rest of function
  }

  // If the message is for this device, it prints the details
  Serial.println("Received from device: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message Size: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();


   // Mesh server placement
    switch (handShake) {
    case 0:
        // statements
        Serial.println("I found a SERVER!");
        destination = sender;
        break;
    case 1:
        // statements
        Serial.println("I found SELF-server!");
        destination = sender;
        break;
    case 2:
        // statements
        Serial.println("I found PATH to server!");
        destination = sender;
        break;        
    default:
        // statements
        break;
    }
 
 
}

//StaticJsonDocument<200> doc;
const size_t CAPACITY = JSON_ARRAY_SIZE(12);
StaticJsonDocument<CAPACITY> doc;
JsonArray array = doc.to<JsonArray>();


//char array[10];
String Values;
void makeData(){
  
array = StaticJsonDocument<200>();

/*
// add some values

 
  doc["id"] = "1C0000000";
  //doc["date_of_value"] = 1556969160;
  doc["longitude"] = a;
/*  doc["air_humidity"] = 28.7;
  doc["air_pressure"] = 982;
  doc["air_CO2"] = 1351824120;
  doc["air_TVOC"] = 468;
  doc["lux"] = 540;
  doc["flame"] = 10;
  doc["soil_humidity"] = 230;
  doc["sound"] = 40;
 
*/

// add some values
String a;
a =gps.location.lat();
Serial.println("hello I am here");
Serial.println(a);
array.add("1C0000000");
array.add(a);



// serialize the array and send the result to Serial
  
  serializeJson(doc, Values);
  serializeJson(doc, Serial);
  Serial.println("");
}


// Setup do Microcontrolador
void setup() 
{
  //makeData();
  // inicializacao da serial 
  Serial.begin(115200);
  ss.begin(GPSBaud);
                     
  while (!Serial);
 
  Serial.println(" LoRa Duplex Communication - IPG Mesh");
  // Initialize the LoRa radio at 868MHz and cheka if it's ok!
  if (!LoRa.begin(433E6)) 
  {             
    Serial.println(" Error starting LoRa module. Check the connection of your pins !!");
    while (true);                      
  }
  Serial.println("LoRa module started successfully !!!");
  //gps start
  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println(F("Edited By www.maxphi.com"));
  Serial.println();
  //gps end

  
}
//gps function
 
void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  { 
    a = gps.location.lat();
    Serial.print(gps.location.lat(),6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(),6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }  
  Serial.println();
}






//Microcontroller Loop - LoRa Communication Operations
void loop()
{
  
  //lora loop start
       if (millis() - lastSendTime > interval)
  { 
    Serial.println(Values);
    
    sendMessage(Values);
    lastSendTime = millis();           // timestamp the message
    interval = random(10000) + 10000;  // 10 seconds
    LoRa.receive();                    // go back into receive mode
    makeData(); //Function that houses the data for LoRa
  }
 
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    onReceive(packetSize);
  }

      //lora loop end
  
  //gps loop start
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();
      
    if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  //gps loop end
  
 

}


