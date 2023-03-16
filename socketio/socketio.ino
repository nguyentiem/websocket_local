/*
 * WebSocketClientSocketIO.ino
 *
 *  Created on: 06.06.2016
 *
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <SocketIOclient.h>
#define LED 2
ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;
#define USE_SERIAL Serial

String output;

 int tem =0; 
 int hum =0; 
 
char mess; 
char*message; 


void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case sIOtype_DISCONNECT:
            USE_SERIAL.printf("[IOc] Disconnected!\n");
            break;
        case sIOtype_CONNECT:
            USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);

            // join default namespace (no auto join in Socket.IO V3)
            socketIO.send(sIOtype_CONNECT, "/");
            break;
        case sIOtype_EVENT:
             message = (char*)payload;
             
             if(message[2]=='l'){// set let 
                char val = (message[strlen(message)-5]);
                if(val=='t'){// on
                  digitalWrite(LED,0);
                  }else{// of 
                    digitalWrite(LED,1);
                    }
                Serial.println((char)val); 
              }
            
             //Serial.println(message);
            break;
        case sIOtype_ACK:
            USE_SERIAL.printf("[IOc] get ack: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_ERROR:
            USE_SERIAL.printf("[IOc] get error: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_BINARY_EVENT:
            USE_SERIAL.printf("[IOc] get binary: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_BINARY_ACK:
            USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
            hexdump(payload, length);
            break;
    }
}

void setup() {
    // USE_SERIAL.begin(921600);
    USE_SERIAL.begin(9600);
    pinMode(LED, OUTPUT);
      digitalWrite(LED,1);
    //Serial.setDebugOutput(true);
    USE_SERIAL.setDebugOutput(true);
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    WiFiMulti.addAP("Hai Anh", "123456789");

    //WiFi.disconnect();
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }
   
    String ip = WiFi.localIP().toString();
     USE_SERIAL.printf("[SETUP] WiFi Connected %s\n", ip.c_str());
  
    socketIO.begin("192.168.1.35", 8888, "/socket.io/?EIO=4");
    //socketIO.beginSSL("smarthome2110.herokuapp.com", 443,"/socket.io/?EIO=4");
    // event handler
    socketIO.onEvent(socketIOEvent);
}


unsigned long messageTimestamp = 0;
void loop() {
    socketIO.loop();
}
