#include<ESP8266WiFi.h>
#include<Keyboard.h>
#include<FirebaseArduino.h>
#include<DHT.H>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);

#define FIREBASE_HOST "covid19-healthcare-bot.firebaseio.com";

int in1 = 3;
int in2 = 5;
int in3 = 6;
int in4 = 9;
int ena = 10;
int enb = 11;

WiFiClient client;
WiFiServer server(80);

const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

String data ="";

void setup()
{
  Serial.begin(9600);

   WiFi.begin (ssid, password);
   while (WiFi.status() != WL_CONNECTED)
   {
    delay(500);
    Serial.print(".");
   }
   dht.begin();
   Serial.println ("");
   Serial.println ("WiFi Connected!");
   Firebase.begin(FIREBASE_HOST);
  
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  server.begin();
}

void loop()
{
    client = server.available();
    if (!client) return; 
    data = checkClient ();

    if (data == "forward") MotorForward();
    else if (data == "backward") MotorBackward();
    else if (data == "left") TurnLeft();
    else if (data == "right") TurnRight();
    else if (data == "stop") MotorStop();
    
    Serial.println();

    int chk = dht.read(DHTPIN);

    float t = dht.readTemperature();
    Serial.print(t);
    Firebase.setFloat ("Temp",t);

    delay(2000);
}


void motorDirection()
{
  char i;
  i = Serial.read();

  analogWrite(ena, 255);
  analogWrite(enb, 255);

  if (i == 'w')
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(2000);
  }
  else if (i == 's')
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(2000);
  }
  else if (i == 'a')
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(2000);
  }
  else if (i == 'd')
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(2000);
  }
  else if (i == 'q')
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    delay(2000);
  }
}

void MotorForward(void)   
{
  digitalWrite(ena,HIGH);
  digitalWrite(enb,HIGH);
  digitalWrite(int1,HIGH);
  digitalWrite(int3,HIGH);
  digitalWrite(int2,LOW);
  digitalWrite(int4,LOW);
}

void MotorBackward(void)   
{
  digitalWrite(ena,HIGH);
  digitalWrite(enb,HIGH);
  digitalWrite(int1,HIGH);
  digitalWrite(int3,HIGH);
  digitalWrite(int2,LOW);
  digitalWrite(int4,LOW);
}

void TurnLeft(void)   
{
  digitalWrite(ena,HIGH);
  digitalWrite(enb,HIGH); 
  digitalWrite(int1,LOW);
  digitalWrite(int3,HIGH);
  digitalWrite(int2,LOW);
  digitalWrite(int4,HIGH);  
}

void TurnRight(void)   
{
  digitalWrite(ena,HIGH);
  digitalWrite(enb,HIGH);
  digitalWrite(int1,HIGH);
  digitalWrite(int3,LOW);
  digitalWrite(int2,HIGH);
  digitalWrite(int4,LOW);
}

void MotorStop(void)   
{
  digitalWrite(ena,LOW);
  digitalWrite(enb,LOW);
  digitalWrite(int1,LOW);
  digitalWrite(int3,LOW);
  digitalWrite(int2,LOW);
  digitalWrite(int4,LOW);
}

String checkClient (void)
{
  while(!client.available()) delay(1); 
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length()-9,9);
  return request;
}
