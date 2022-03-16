#include "wifi.h"

const char *ssid = SSID;
const char *password = PASSPHRASE;

WiFiClient espClient;

void setup_wifi()
{

  delay(WIFI_INITIAL_DELAY);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(WIFI_CONNECTION_RETRY);
  }
}