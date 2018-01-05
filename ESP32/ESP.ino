int deviceID = 0;
int hostPartA = 0;
int start_time;
int time_out_time = 10000;
void init_ESP() {
  Serial.println();
  Serial.println();
  connect_to_wifi();
}

int IPPart(int part, IPAddress address)
{
  return (address[part]);
}

void connect_to_wifi() {
  WiFi.disconnect();
  String ssid = read_ssid();
  String pass = read_pass();

  char char_ssid[ssid.length()];
  ssid.toCharArray(char_ssid, ssid.length() + 1);

  char char_pass[pass.length()];
  pass.toCharArray(char_pass, pass.length() + 1);

  Serial.print("Connecting to: ");
  Serial.println(char_ssid);
  Serial.print("With this password: ");
  Serial.println(char_pass);

  WiFi.begin(char_ssid, char_pass);
  start_time = millis();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (start_time + time_out_time < millis()) {
      Serial.println();
      Serial.println("Could not connect to the wifi network, check if SSID and PSK are correct");

      break;
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    deviceID = (IPPart(3, WiFi.localIP()));
    hostPartA = (IPPart(2, WiFi.localIP()));

    init_OSC();
  }

}


