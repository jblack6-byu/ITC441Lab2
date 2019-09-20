#include <cQueue.h>
#include <ESP8266WiFi.h>


int redLED = D6;
int yellowLED = D7;
int greenLED = D8;
const int offState = 0;
const int redState = 1;
const int greenState = 2;
const int yellowState = 3;
bool automode = false;
const char* ssid = "WIFI NAME";
const char* password = "WIFI PASSWORD";
unsigned long currentMillis;
WiFiServer server(80);
int curState;
Queue_t stateQueue;


void greenLight() {
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, HIGH);
}
void yellowLight() {
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, HIGH);
}

void redLight() {
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, HIGH);
}

void offLight() {
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(10);
  Serial.println("Serial Output Beginning.");
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  Serial.println("Done Initializing LEDs.");
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Server Started...");
  curState = offState;
  q_init(&stateQueue, sizeof(int), 20, FIFO, false);
  Serial.println("Queue created...");
  currentMillis = millis();
  startMillis = currentMillis - 1;

}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Red");
  currentMillis = millis();
  WiFiClient client = server.available();
  if(client) {
    while (!client.available()) {
      delay(10);
    }
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if(request.indexOf("/Off") != -1) {
    automode = false;
    //Queuing up this request
    q_push(&stateQueue, &offState);
    Serial.println("Queued Off State.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("")
  }
  
  if(request.indexOf("/Green") != -1) {
    automode = false;
    //Queuing up this request
    q_push(&stateQueue, &greenState);
    Serial.println("Queued Green State.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("")
  }
  else if(request.indexOf("/Red") != -1) {
    automode = false;
    //Queuing up this request
    q_push(&stateQueue, &redState);
    Serial.println("Queued Red State.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("")
  }
  else if(request.indexOf("/Yellow") != -1) {
    automode = false;
    //Queuing up this request
    q_push(&stateQueue, &yellowState);
    Serial.println("Queued Yellow State.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("")
  }
  else if(request.indexOf("/Status") != -1) {
    //We need to print out the status here.
    Serial.println("Request for Status.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("")  
  }
  else if(request.indexOf("/Auto") != -1) {
    automode = true;
    //Clearing the Queue
    q_clean(&stateQueue);
    Serial.println("Auto Mode.");
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("")
    
  }
  

}