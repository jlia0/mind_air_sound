#include <Arduino.h>
#include "wifi.h"
#include <PubSubClient.h>
#include <string>


// MQTT
#include "wifi.h"

#define MQTT_SERVER "tailor.cloudmqtt.com"
#define MQTT_PORT 18184

#define CLIENT_ID "AllEarsClient"
#define ATTENTION_TOPIC "attention"
#define MEDITATION_TOPIC "meditation"
#define DELTA_TOPIC "delta"
#define THETA_TOPIC "theta"
#define GAMMA_TOPIC "gamma"
#define ALPHA_TOPIC "alpha"
#define BETA_TOPIC "beta"


#define USERNAME "kvuwrinm"
#define PASSWORD "tucVHt31q7Gx"


const char *mqtt_server = MQTT_SERVER;
PubSubClient client(espClient);

// ---------------------------------------

#define airPump 5
#define vacuum 4
#define airValve 0


// ---------------------------------------

void mqtt_hanlder(char *topic,
                  byte *payload,
                  unsigned int length)
{
  if (length == 0)
    return;

  char message[length + 1];
  getMessageFromPayload(message, payload, length);
  int num = std::stoi(message);


  if (strcmp(topic, ATTENTION_TOPIC) == 0)
  {
    if (num < 50) {
      pinMode (airPump, OUTPUT);
      pinMode(airValve, OUTPUT);
      pinMode (vacuum, INPUT);
    } else {
      pinMode (airPump, INPUT);
      pinMode(airValve, INPUT);
      pinMode (vacuum, OUTPUT);
    }
    if (num == 100) {
      pinMode (airPump, INPUT);
      pinMode(airValve, INPUT);
      pinMode (vacuum, INPUT);
    }
  }
  else if (strcmp(topic, MEDITATION_TOPIC) == 0)
  {
    if (num > 50) {
      pinMode (airPump, OUTPUT);
      pinMode(airValve, OUTPUT);
      pinMode (vacuum, INPUT);
    } else {
      pinMode (airPump, INPUT);
      pinMode(airValve, INPUT);
      pinMode (vacuum, OUTPUT);
    }
    if (num == 0) {
      pinMode (airPump, INPUT);
      pinMode(airValve, INPUT);
      pinMode (vacuum, INPUT);
    }
  } else if (strcmp(topic, DELTA_TOPIC) == 0)
  {
    // sleep

  } else if (strcmp(topic, THETA_TOPIC) == 0)
  {
    // drowsiness
    
    if (num > 50) {
      pinMode (4, OUTPUT);
    } else {
      pinMode (4, INPUT);
    }

  } else if (strcmp(topic, GAMMA_TOPIC) == 0)
  {
    // concentration
    
    if (num > 50) {
      pinMode (0, OUTPUT);
    } else {
      pinMode (0, INPUT);
    }

  } else if (strcmp(topic, ALPHA_TOPIC) == 0)
  {
    // restuful
    
    if (num > 50) {
      pinMode (2, OUTPUT);
    } else {
      pinMode (2, INPUT);
    }

  } else if (strcmp(topic, BETA_TOPIC) == 0)
  {
    // busy

    if (num > 50) {
      pinMode (5, OUTPUT);
    } else {
      pinMode (5, INPUT);
    }

  }
}

void setup_mqtt()
{
  client.setServer(mqtt_server, MQTT_PORT);
  client.setCallback(mqtt_hanlder);
}

void reconnect()
{
  while (!client.connected())
  {
    String clientId = CLIENT_ID;
    if (client.connect(clientId.c_str(), USERNAME, PASSWORD))
    {
      client.subscribe(ATTENTION_TOPIC);
      client.subscribe(MEDITATION_TOPIC);
      client.subscribe(DELTA_TOPIC);
      client.subscribe(THETA_TOPIC);
      client.subscribe(GAMMA_TOPIC);
      client.subscribe(ALPHA_TOPIC);
      client.subscribe(BETA_TOPIC);
    }
    else
    {
      delay(5000);
    }
  }
}

void mqtt_loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}

void getMessageFromPayload(char *message, byte *payload, unsigned int length)
{
  for (unsigned int i = 0; i < length; i++)
  {
    message[i] = payload[i];
  }
  message[length] = '\0';
}


void setup()
{
  Serial.begin(9600);

  setup_wifi();
  setup_mqtt();
}

void loop()
{
  mqtt_loop();
  delay(1);        // delay in between reads for stability
}
