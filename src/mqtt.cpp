/*
 *
 *  Project 41-mqtt_00 - Austral - EAM
 *
 *  mqtt.cpp
 *      Manage conversation with MQTT broker
 */

/*
 *  System includes
 */

#include <Arduino.h>
#include <PubSubClient.h>
//#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <TelnetStream.h>

/*
 *  Project includes
 */

#include "mqtt.h"               //  Prototypes of this module public functions
#include "mqtt_actions.h"       //  Actions to be called in case of subscription topics news
#include "mqtt_def.h"           //  User configuration file

/*
 *  Objects instantiations
 */

//WiFiClientSecure espClient = WiFiClientSecure();
WiFiClient espClient = WiFiClient();
PubSubClient client(espClient);

/*
 *  Private variables
 */

static char client_id[100];
static char tp_string[100];
static char sub_header[50];

/*
 *  Private constants
 */

static const char *mqttServer =     BROKER_NAME;
static const int  mqttPort =        BROKER_PORT;
static const char *mqttUser =       BROKER_USER;
static const char *mqttPassword =   BROKER_PASS;
static const char CERT_CA[] = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDVzCCAj+gAwIBAgIUEFL4q4FGTGyfg4cYwgENXU6+G7cwDQYJKoZIhvcNAQEL
BQAwOzELMAkGA1UEBhMCQVIxDDAKBgNVBAoMA0lPVDEPMA0GA1UECwwGY2xpZW50
MQ0wCwYDVQQDDAR0ZXN0MB4XDTIzMTExNTAyMjA0NloXDTI4MTEyMTAyMjA0Nlow
OzELMAkGA1UEBhMCQVIxDDAKBgNVBAoMA0lPVDEPMA0GA1UECwwGY2xpZW50MQ0w
CwYDVQQDDAR0ZXN0MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA62Lx
hAxF9zCzcU949QZBHKpszh05ljrA4QDdCLnp1chfYHhIR+hOrRK075DFXqUw5r8n
Lbvht9OkrOyTqZ53tcxW3q1oW3pcBuG2+c8ksO/QBj4PUnGgpd97CQJ+GthRO7o9
xHXKjIjPEHD4oZKBUqyIg178UmjV0GaLHPPnwTVK6aJGGu4t7RNf7nrWQEik+Dsx
LYhKP6/JlrSo/sGFJeS3zz6F2QWTa+F+TNcKXS7v0JHERsT5s5CHL26YpMmKuRJ2
PaHO5cE3B5zKwxx55+NvO+Niw13MYsUKZqCJvgPxLBHXYvwWYTNfsJ/4umkc8Vlg
0K3+NzMShACG11TINQIDAQABo1MwUTAdBgNVHQ4EFgQUBBxINjY+czyWOLuILTX+
iU/fHBIwHwYDVR0jBBgwFoAUBBxINjY+czyWOLuILTX+iU/fHBIwDwYDVR0TAQH/
BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEApkH4YficfYT6TfGyvWIYNV06GbEx
I7etPa/dbplE/E4NaldJhtCou2KN/qAEmqkRpDfe72dVxW3NThqxofS/zKZ9e/ez
PYca5IS9eoHFapO/TvnnORoCaTKrI3aVjlhqaUkIg5OshiHBfQYFRRFkg9Au5IVg
aGLB0Xj1zMNq6DOqhqQBs1L/BrpSJsd4HXS4J/Z4benRfrKuhffWduPNSF/WoCaD
9bk0penKCEeicqm0MWevaat9D7anQnp6cgCimd9kTbBfYcBATQ8DyzIvq0SQl1mB
CQ8e+TdrMARilFY3rZkVvoaJShVpE4Bxi2UGHT33mJzo11kiGUNu7uHQcg==
-----END CERTIFICATE-----
)EOF";


static const char *cert_file = "-----BEGIN CERTIFICATE-----\n"
"MIIDKjCCAhICFHMfAYWdNu9s5317R5Nt/d8gyhhvMA0GCSqGSIb3DQEBCwUAMDYx\n"
"CzAJBgNVBAYTAkFSMRUwEwYDVQQIDAxidWVub3MgYWlyZXMxEDAOBgNVBAcMB2Nh\n"
"bXBhbmEwHhcNMjMxMTE1MDAzMjU0WhcNMjQxMTA5MDAzMjU0WjBtMQswCQYDVQQG\n"
"EwJBUjEVMBMGA1UECAwMQlVFTk9TIEFJUkVTMRAwDgYDVQQHDAdDQU1QQU5BMSEw\n"
"HwYDVQQKDBhMb2NhbGhvc3QgTVFUVCBCcm9rZXIgU0ExEjAQBgNVBAMMCWxvY2Fs\n"
"aG9zdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANNzsUCQvuf8cWVn\n"
"fouENSrtYVf5+4/UR5LFnQgHkGDCBdINzBuXwc4Rf87AGXLXQKb80SOKhebHlrKL\n"
"mqRGnOvGpW2NmIqAt/+d7bmaiJfHQvtVlAtkZgOmcRwzXjEugY2jGQu5E8OlQDJk\n"
"lFf83atMuacS0ZDqDvy6OijZ/v6m6KSb9/Wi8dOLgkutxJmLv3h9oLXBuTTCVuiG\n"
"j+OKla/4bKpks3iu9dqg5yriJS0mxHL6eiktZJMHQTAz97EH6LACD646eTNrsIKp\n"
"4wi6sFily467jSiQp3uvaLaUmfqqWle0C21MpG5Zj+OuwLEm2lj6+UfsoZm1/X4a\n"
"7RFVsAECAwEAATANBgkqhkiG9w0BAQsFAAOCAQEAsszfcrI0PS3U8mLisPavIIXL\n"
"FwhCfK8PLtLb3vTOFG4eQYA0dfBxOOdzsjkQJI/1TJQOCY6CKTOcOinWkhZEsFUT\n"
"IVRodHHZ7CVxxR08AGgIvezv1j/WmAwkrAf2PNLm4mQSokKRSzd74gcB9EYzjZbA\n"
"cj7bRmNDO3hRCQ/ssgAHHgIw+l85dL/neRSMbBAjMKqsJtRCMCd0ZppYmFDUDZzH\n"
"NEr4LXQdKvS0OL+V4qrgoc2vVb1+yt/StTTRYObLL0AuvLFQl7ajT+fmIQRz184S\n"
"jy7pmdUbJXi76iVlOwGuAbOqnmGe5RcirIvrZ08kHwex1sW8d19kUrJN7zDt/w==\n"
"-----END CERTIFICATE-----";
static const char *key_file = "-----BEGIN PRIVATE KEY-----\n"
"MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDTc7FAkL7n/HFl\n"
"Z36LhDUq7WFX+fuP1EeSxZ0IB5BgwgXSDcwbl8HOEX/OwBly10Cm/NEjioXmx5ay\n"
"i5qkRpzrxqVtjZiKgLf/ne25moiXx0L7VZQLZGYDpnEcM14xLoGNoxkLuRPDpUAy\n"
"ZJRX/N2rTLmnEtGQ6g78ujoo2f7+puikm/f1ovHTi4JLrcSZi794faC1wbk0wlbo\n"
"ho/jipWv+GyqZLN4rvXaoOcq4iUtJsRy+nopLWSTB0EwM/exB+iwAg+uOnkza7CC\n"
"qeMIurBYpcuOu40okKd7r2i2lJn6qlpXtAttTKRuWY/jrsCxJtpY+vlH7KGZtf1+\n"
"Gu0RVbABAgMBAAECggEAT52sC8d0WhzvBWNmflmVO0iN8nYphgpDuLu3WKKgdg4X\n"
"6vNO+sS1MEXefZZO/rKVoUpn5J3vtOlJkIiH/Xqo8w2klA9QdDXrEdABBnVtINji\n"
"G4gpafCviIhO5nNg79u25AGZGJTjIDNEbLT6Nfb779e/v2ySg6HdasMjyuXp4S9/\n"
"VCgQifAxQ4uDk/rvonP2YpJDoeqH2+gZglcjORP5Tt6tE44RD8CK/2jpRR9bfHMb\n"
"nYA8O2CbZm9RT6J++r/UZiu05CHzM7KMPDP30Bj73rl3GKS8dE4RdIriZES2idxT\n"
"zImXw+u5WltH5jze1Theg/l7ja152wkPbb9WUnEikQKBgQDUhLRzJvGOc7giuiEK\n"
"txN3lj3XlR9YsyHSiNELepDjVMarZkZ6nqXoPnD2codOZoKP2TE93MK6eviEpPYh\n"
"nr8VnVMUZApH+U3gJ5JF6JJTOaGHjIzju0UCRtH8oWEtpYgsEr7n4vdQe3EHgHdB\n"
"tNc/s9KLBoDI03mIKAHUw8HdtwKBgQD+tyDxzSZg+F0SeIq8+4nonrm5kF2OIPAD\n"
"H9ZTZh7P/2a/iqG45SPfrCo04UPiZbeVkyXh1ljAA8dHp8tGb0B+BfFOCcERbC+o\n"
"nSeXG5WrWPmqMh0uCwtDk5nVVH6L7Vv4UDBlAhnubHLnkVq3cUKTlkKe5lKplgbm\n"
"jgIlWzxgBwKBgB4a/oUhiy6m7frdazjdXoCeY/xaP6qNFcmBYDvEDSpnJA2721gb\n"
"iV0xG0eOSeStWOmP1w3wbBWGvhXsWIHCv0cMlmxZD6vKgWGzuWD15WSs0Vkma/8G\n"
"1GvSJIUpR170CCpWLX0kBez8z7JEXKCOQb/gdX8EWNIaQykqobfGU3xZAoGBAPVE\n"
"XWK4ilYhkW1xyRZEl/z4qhn22z86lCJOC9Duqojag5O46BTj//EnFNUqfHWKQIa0\n"
"1GtsPLVYbhGfjutZwR4YlszwM4qVRCb2fOLSWwJNgMrTyqlFVSlS/XQgEmKmwkKO\n"
"5Kq+pT2iC9rTaijt1I/MxoW/fqVxo19qKsLR4YcNAoGBAJb4i+bF0dIb6kdnDSeH\n"
"wRYsArfKPw1pRRbv7kHYFxyadMIipskGPhBEgqXbxr/uCcetBmwQko5YthkVU3SK\n"
"UgwZjwXN4ZYEMi2wMNmD/PsHvgsk/BjO/NED/ontOS2aFKMYTWTRfugEbWLBRlso\n"
"RGx2c7vPP2n4hbWl3IU1oR0j\n"
"-----END PRIVATE KEY-----";

/*
 *  Private functions
 */

static int
get_origin(char *st, char *slast )
{
    char *sfirst;

    *slast = '\0';
    if( ( sfirst = strrchr(st,'/') ) == NULL )
        return -1;
    return atoi(++sfirst);
}

/*
 *  callback:
 *      Function to asynchronously call when someone
 *      publish in broker a topic that we are subscribed
 */

static void
callback(char *topic, byte *payload, unsigned int length)
{
    char *pl;
    int origin_num;

    payload[length] = 0;
    Serial.printf( "For topic: \"%s\", message \"%s\"\n", topic, payload );
    if( ( pl = strrchr(topic,'/') ) == NULL )
        return;
    origin_num = get_origin(topic,pl);
    Serial.printf("main topic = %s\n", ++pl );

    for( int i = 0; topics[i].sub_topic != NULL; ++i )
        if( strcmp( pl, topics[i].sub_topic ) == 0 )
        {
            (*topics[i].action)( origin_num, (char *)payload );
            break;
        }
    Serial.println("-----------------------");
}

/*
 *  client_connect:
 *      Connection to broker
 *      Tries until connected
 */

static void
client_connect(void)
{
    Serial.printf("Connecting to MQTT...\n");
    Serial.printf("server = %s\n",mqttServer);

    //espClient.setCACert( CERT_CA );
    //espClient.setCertificate(cert_file);
    //espClient.setPrivateKey(key_file);

    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    while (!client.connected())
    {
        if (client.connect(client_id, mqttUser, mqttPassword))
            Serial.printf( "%s: client %s connected\n", __FUNCTION__, client_id );
        else
        {   
            Serial.printf("%s: failed with state = %d\n", __FUNCTION__, client.state());
            delay(2000);
        }
    }
    Serial.printf("Connected to %s\n", mqttServer);
}

/*
 *  subscribe_to:
 *      Function to subscribe to one topic
 */

static void
subscribe_to( const char *ptopic )
{
    int status;
 
    status = client.subscribe(ptopic);
    Serial.printf("%s -> topic: %s [status = %d]\n", __FUNCTION__, ptopic, status);
}

/*
 *  init_subscriptions:
 *      Function to subscribe to all topics
 *      In this case, all topics that are in a topic table
 *      outside this file (obtained in from 'mqtt_def.h'
 */
    

static void
init_subscriptions(void)
{
    const char **p;
    char subs_string[100];

    for( p = subs; *p != NULL; ++p )
    {
        sprintf(subs_string,"%s/%s", sub_header, *p );
        subscribe_to(subs_string);
    }
}

/*
 *  Public functions
 */

/*
 *  init_mqtt:
 *      All iniializations needed to set variables
 *      connect to broker and initialize subscriptions
 */


void
init_mqtt(int num)
{
    subs_string(sub_header);

    id_string(client_id,num);
    Serial.printf("client_id: %s\n", client_id );

    topic_string(tp_string,num);
    Serial.printf("tp_string: %s\n", tp_string );

    client_connect();
    init_subscriptions();
}

/*
 *  do_publish:
 *      Function to make a publication in broker
 *      receives pointer to topic and message string
 */

void
do_publish(const char *ptopic, const char *msg)
{
    static char tp[100];

    tp[sizeof(tp)-1] = '\0';
    snprintf( tp, sizeof(tp)-1, "%s/%s", tp_string, ptopic );
    client.publish( tp, msg );
    Serial.printf( "%s: %s %s\n\r", __FUNCTION__, tp, msg );
}

/*
 *  test_mqtt:
 *      update to see broker news
 */

void
test_mqtt(void)
{
    client.loop();
}


