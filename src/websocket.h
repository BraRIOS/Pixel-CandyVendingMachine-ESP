/**
 * @file websocket.h
 * @brief Websocket server and client
 * 
 * @details This file contains the functions to handle the websocket server and client
*/
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

void notifyDispensing();
void notifyCandies();
void notifyPrice();

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void initWebSocket();
String processor(const String& var);
    