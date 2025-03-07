#include "WiFi_init.h"
#include "WiFi.h"
#include "led.h"
#include "graphics.h"
#include "typedef.h"
#include "NTPClient.h"
#include "WiFiUdp.h"

//-------------------------------------------------------------------------------//
// Enter the WIFI name and password that you can access to the Internet. 
//It is recommended to use an Android phone for web control to ensure that the ESP32 is in the same network.
const char* ssid = "fuck_esp";
const char* password = "93074zar";
WiFiServer server(80); // Set the web server port number to 80
String header;  // Variable to store the HTTP request
String output25State = "off"; // Auxiliary variable for storing the current output state
String output26State = "off"; //state blue led
unsigned long currentTime = millis(); // Current time
unsigned long previousTime = 0; // Previous time
static const long timeoutTime = 2000; // Define the timeout in milliseconds (eg: 2000ms = 2s)

WiFiUDP ntpUDP; //Объект ntp
NTPClient timeClient(ntpUDP, NTPserver1, 3600, 60000);
String Time_Str;
const char * c_Time = "00:00:00";
String Date_Str;
const char * d_Time = "2000-01-01";

//-------------------------------------------------------------------------------//
control_LED LED_blue (BLUEled, OFF);
control_LED LED_green  (GREENled, OFF);

//-------------------------------------------------------------------------------//
static void init_NTP_client (void);

//-------------------------------------------------------------------------------//
 void NTPtimedata::getTimeData()
 {
    flags.status_NTP = ptr_NTP->update(); //Обновляем дату  
    if (flags.status_NTP == true )
    {
      sec =  ptr_NTP->getSeconds();
      minute = ptr_NTP->getMinutes();
      hour = ptr_NTP->getHours();
      day = ptr_NTP->getDay();
    }
    else
    { Serial.println ("ntp_error"); }
 }

//-------------------------------------------------------------------------------//
void init_WiFi_connection (void)
{
  flags.status_WiFi = OFF;
  Serial.print("wait connect ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);   // Connect to Wi-Fi network using SSID and password
  for (uint8_t count = 0; count < 10; count++)
  {
    if (WiFi.status() != WL_CONNECTED) 
    { 
      delay(100);  
      continue;
    }
    else
    {
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP()); //start the web server
      flags.status_WiFi = ON;
      init_NTP_client ();
      return;
    }
  }
  Serial.println("not_connected");
  return;
}

//-------------------------------------------------------------------------------//
static void init_NTP_client (void)
{
  timeClient.begin(); //Запускаем клиент времени  
  timeClient.setTimeOffset(10800); 
}

//-------------------------------------------------------------------------------//
void get_NTP_time (void)
{
  if (flags.status_WiFi == ON)
  {
    flags.status_NTP = timeClient.update(); //Обновляем дату  
    if (flags.status_NTP == true )
    {
      Time_Str=timeClient.getFormattedTime(); //время формата `hh:mm:ss`  
      c_Time  = Time_Str.c_str(); //конверсия типа string`a в тип char *
      Serial.println (c_Time);
      time_data_update (c_Time);
      Date_Str=(timeClient.getFormattedDate().substring(0, 10)); //дата формата '2004-02-12T15:19:21+00:00'
      d_Time = Date_Str.c_str();
      Serial.println (d_Time);
    }
    else
    { Serial.println ("ntp_error"); }
  }
  else
  { Serial.println ("WiFi_not_connected");  }
}

//-------------------------------------------------------------------------------//
static void start_HTTP_server (void)
{
  server.begin();
}

//-------------------------------------------------------------------------------//
void WiFi_connection_check (void)
{
    WiFiClient client = server.available();   // Monitor clients
  if (client)  // If a new client connects
  {                            
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // Print a message in the serial port
    String currentLine = "";                // Create a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) // Loop while client connects
    {  
      currentTime = millis();
      if (client.available())  // If you want to read bytes from the client
      {            
        char c = client.read();             // Then read a byte
        Serial.write(c);                    // Print out on serial monitor
        header += c;
        if (c == '\n')        // If the byte is a newline
        {                   
          // If the current line is empty, there are two newlines on a line.
          if (currentLine.length() == 0) // This is the end of the client HTTP request, so send a response:
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // Then the content-type, so the client knows what to expect, followed by the empty line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /16/on") >= 0)  // turn GPIO 
            {
              output25State = "on";
              Gled->switch_led (ON);
            } 
            else 
            {
              if (header.indexOf("GET /16/off") >= 0) 
              {
                output25State = "off";
                Gled->switch_led (OFF);
              }
              else 
              {
                if (header.indexOf("GET /17/on") >= 0) 
                {
                  output26State = "on";
                  Bled->switch_led (ON);
                } 
                else 
                {
                  if (header.indexOf("GET /17/off") >= 0) 
                  {
                    output26State = "off";
                    Bled->switch_led (OFF);
                  }
                }
              }
            }
            client.println("<!DOCTYPE html><html>"); // Display HTML pages
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off button
            // Feel free to change the background color and font size properties to suit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");            
            client.println("<body><h1>ESP32 Web Server LED </h1>"); // Page title
            client.println("<p>GREEN LED - State " + output25State + "</p>"); // Display current status and ON/OFF button of GPIO 16
            if (output25State=="off") // If output26State is off, show ON button
            { client.println("<p><a href=\"/16/on\"><button class=\"button\">ON</button></a></p>"); } 
            else 
            { client.println("<p><a href=\"/16/off\"><button class=\"button button2\">OFF</button></a></p>"); } 
               
            client.println("<p> BLUE LED - State " + output26State + "</p>");  // Display current status and ON/OFF button of GPIO 17
            if (output26State=="off") // If output26State is off, show ON button
            { client.println("<p><a href=\"/17/on\"><button class=\"button\">ON</button></a></p>"); } 
            else 
            { client.println("<p><a href=\"/17/off\"><button class=\"button button2\">OFF</button></a></p>"); }
            client.println("</body></html>");            
            client.println(); // HTTP response ends with another blank line
            Serial.println("close_connect");  
            break; // Out of the while loop
          } 
          else // If you have a newline then clear currentLine
          { currentLine = ""; }
        } 
        else 
        {
          if (c != '\r') // If you get characters other than carriage return
          {  currentLine += c; }     // Add it to the end of currentLine
        }
      }
    }
  }
  header = "";  // Clear header variable
 
  client.stop();  // Close the connection
}

//-------------------------------------------------------------------------------//
void switch_net_blueled (uint8_t comm)
{
  if (comm == ON)
  {   output26State = "on"; }
  else 
  {
      if (comm == OFF)
      {   output26State = "off";   }
  }
}

//-------------------------------------------------------------------------------//
void switch_net_greenled (uint8_t comm)
{
  if (comm == ON)
  {   output25State = "on"; }
  else 
  {
      if (comm == OFF)
      {   output25State = "off";   }
  }
}

