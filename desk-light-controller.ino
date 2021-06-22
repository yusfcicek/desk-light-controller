#include <ESP8266WiFi.h>

const char* WIFI_NAME = "CICEK";
const char* WIFI_PASSWORD = "27726134";
const uint8_t LEDPIN = D1;

WiFiServer server(80);

IPAddress local_IP(192, 168, 1, 10);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);


void setup() {
  Serial.begin(9600);

  WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
  
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);
  
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  
  if (!client) {
    return;
  }

  while(!client.available()){
    delay(1);
  }

  String request = client.readStringUntil('r');
  client.flush();

  int value = HIGH;
  
  if (request.indexOf("/LED=OFF") != -1) {
    digitalWrite(LEDPIN, HIGH);
    value = HIGH;
  }
 
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(LEDPIN, LOW);
    value = LOW;
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">");
  client.println("<style>p.padding{padding-left: 0.4cm;}p{color: black;}cred{color: red}cgreen{color: green}</style>");
  
  client.print("<br><p class=\"padding\">Desk Light is now : ");

  if(value == HIGH) {
    client.print("<cred>Off</cred>");
  } 
  
  else {
    client.print("<cgreen>On<cgreen></p>");
  }
  
  client.println("<div class=\"w3-container\">");
  client.println("<br>");
  client.println("<a href=\"/LED=ON\"\"><button class=\"w3-btn w3-ripple w3-green\">Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button class=\"w3-btn w3-ripple w3-red\">Turn Off </button></a><br />");
  client.println("</div>");
  client.println("</html>");

}
