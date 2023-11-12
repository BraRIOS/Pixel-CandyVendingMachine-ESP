
/*
 *  Project 41-mqtt_00 - Austral - EAM
 *
 *  mqtt.h
 *      Manage conversation with MQTT broker
 */

void init_mqtt(int num);                                //  All initializations needed for conversation with broker
void do_publish(const char *topic, const char *msg);    //  Called to publish in broker
void test_mqtt(void);                                   //  Update to see broker news


