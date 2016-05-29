/*
hodnocení výuky
*/

#include <SPI.h>
#include <Ethernet.h>

//inicializace ethernetu
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 177);
EthernetClient client;

//inicializace tlacitek 
int cervena = 0;  // pin 3
int zluta = 0;    // pin 2
int zelena = 0;   // pin 7

int delayTime = 100; // zpozdeni pro blikani zarovek

//údaje pro thingspeak
char server[] = "api.thingspeak.com";
String APIKey = "U0G8C0VYJ22EC4M0";


//rozsviti zarovky po stisknutí tlacitka
void lightLedOn(int pin, int delayTime) {
  digitalWrite(pin, HIGH);
  delay(delayTime);
}

void lightLedOff(int pin, int delayTime) {
  digitalWrite(pin, LOW);
  delay(delayTime);
}

void setup() {
  // NASTAVENI ZAROVEK A TLACITEK
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);

  pinMode(3, INPUT);
  pinMode(2, INPUT);
  pinMode(7, INPUT);

  Serial.begin(9600);
  

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }

  delay(1000);
}


void posliData(String tsData) {
  Serial.print("Posilam data: ");
  Serial.print(tsData);
  
  if (client.connect(server, 80)) {

    Serial.println("jsme uvnitr posli data");
    Serial.println("connected");

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + APIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);
    
    client.flush();
    client.stop();
    Serial.println("uspesne poslano");
  }

}

void loop()
{
  cervena = digitalRead(3);
  zluta = digitalRead(2);
  zelena = digitalRead(7);


// a.k.a. "mohlo by se hodit" - v případě nějaké chyby

//  if (client.available()) {
//    char c = client.read();
//    Serial.print(c);
//  }

  // ZELENA
  if (zelena == HIGH) {
    lightLedOn(6, delayTime);

    posliData("field1=3");

    lightLedOff(6, delayTime);
  }


  // ZLUTA
  if (zluta == HIGH) {
    lightLedOn(5, delayTime);

    posliData("field1=2");

    lightLedOff(5, delayTime);
  }

  // CERVENA
  if (cervena == HIGH) {
    lightLedOn(4, delayTime);

    posliData("field1=1");

    lightLedOff(4, delayTime);
  }
}

