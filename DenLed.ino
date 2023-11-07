#include <SPI.h>
#include <Ethernet.h>
//#include <avr/wdt.h>
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
#define LedXanh 2
#define LedDo 3
#define BAT HIGH // Sua muc bat tat
#define TAT LOW

byte mac[] = {

  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(192, 168, 1, 177); // Ip của mạch đèn, trên web phải khai báo địa chỉ ip này để điều khiển đèn

IPAddress myDns(192, 168, 1, 1);

IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 255, 0);

// telnet defaults to port 23

EthernetServer server(23); // --------------------Web khai bao dung

bool alreadyConnected = false; // whether or not the client was connected previously
long timeOut=0;

void setup() {

  pinMode(LedXanh,OUTPUT);
  pinMode(LedDo,OUTPUT);
  Ethernet.begin(mac, ip, myDns, gateway, subnet);

  // Open serial communications and wait for port to open:

  Serial.begin(9600);

   while (!Serial) {

    ; // wait for serial port to connect. Needed for native USB port only

  }

  // Check for Ethernet hardware present

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {

    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");

    while (true) {

      delay(1000); // do nothing, no point running without Ethernet hardware

    }

  }

  if (Ethernet.linkStatus() == LinkOFF) {

    Serial.println("Ethernet cable is not connected.");

  }

  // start listening for clients

  server.begin();

  Serial.print("Chat server address:");

  Serial.println(Ethernet.localIP());
  timeOut= millis();
  //wdt_enable(WDTO_2S);
}

void loop() {

  // wait for a new client:

  EthernetClient client = server.available();

  // when the client sends the first byte, say hello:

  if (client) {

    if (!alreadyConnected) {

      // clear out the input buffer:

      client.flush();

      Serial.println("We have a new client");

      client.println("Hello, client!"); 

      alreadyConnected = true;

    }

    if (client.available() > 0) { // Đọc gói tin từ client(web) gửi đén
      char thisChar = client.read(); // Đọc gói tin

      if(thisChar=='T') // truetime
      {
          digitalWrite(LedXanh,BAT); // Xem module nhaajn muc nao
          digitalWrite(LedDo,TAT); 
          timeOut=millis();
          Serial.println("Get T form client");
          delay(100);       
      }
      if(thisChar=='F')// flase
      {
          digitalWrite(LedXanh,TAT);
          delay(1);
          digitalWrite(LedDo,BAT);
          timeOut=millis();
          Serial.println("Get F ");
          
      }
      Serial.write(thisChar);
    }
    if(millis()-timeOut >1000) // Neesu sau 10 ms ma ko nhan phan hoi thi tat den
    {
        if(digitalRead(LedXanh)==BAT || digitalRead(LedDo)==BAT)
        {
            digitalWrite(LedXanh,TAT);
            digitalWrite(LedDo,TAT);
        }
        //Serial.println("xxx");
    }
     
  }    
} 