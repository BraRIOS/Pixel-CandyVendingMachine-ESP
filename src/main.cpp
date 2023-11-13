/*
 *  Project 41-mqtt_00 - Austral - EAM
 *
 *      File main.cpp
 */

#include <Arduino.h>
#include <TelnetStream.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include "wifi_ruts.h"
#include "mqtt.h"
#include "hw.h"
#include "websocket.h"

int board;
bool dispensing = 0;
int candiesStock = 40;
int price = 5;
int buyed = 0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void notifyDispensing() {
  ws.textAll("Dispensing:"+String(dispensing));
}

void notifyCandies() {
  ws.textAll("Candies:"+String(candiesStock));
}

void notifyPrice() {
  ws.textAll("Price:"+String(price));
}

void handleWebSocketMessage(uint32_t id, void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    Serial.printf("WebSocket client #%u message: %s\n", id, (char*)data);
    if (strncmp((char*)data, "buy", 3) == 0) {
      int buy = atoi((char*)&data[4]);
      if (buy <= candiesStock && buy > 0){
        Serial.printf("Dispensing %d candies\n", buy);
        buyed = buy;
        candiesStock -= buy;
        dispensing = 1;
        notifyDispensing();
        notifyCandies();
      }    
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(client->id(), arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (dispensing){
      return "Dispensing...";
    }
    else{
      return "Ready";
    }
  }
  else if(var == "CANDIES"){
    return String(candiesStock);
  }
  else if(var == "PRICE"){
    return String(price);
  }
  return String();
}


void
setup(void)
{
    Serial.begin(BAUD);

    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    } 
    Serial.println("SPIFFS mounted successfully");

    connect_wifi();

    init_hw();
    board = get_board_num();
    printf("Board = %d\n", board);

    init_mqtt(board);
    do_publish("init_values","");
    test_mqtt();

    TelnetStream.begin();
    
    initWebSocket();

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        //request->send_P(200, "text/html", index_html, processor);
        request -> send(SPIFFS, "/index.html", "text/html", false, processor);
    });

    server.serveStatic("/", SPIFFS, "/");

    // Start server
    server.begin();
}

void
loop(void)
{
  if(dispensing){
    do_publish("sold", String(buyed).c_str());
    dispense_candies(buyed);
    dispensing = 0;
    notifyDispensing();
  }
  test_mqtt();//  Test news from broker
}
