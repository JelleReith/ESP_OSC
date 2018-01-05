WiFiUDP Udp;
IPAddress outIP(192, 168, 0, 0);
int outPort = 9999;
int localPort = 8888;
bool confirm = false;
OSCErrorCode error;

void init_OSC() {

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
#ifdef ESP32
  Serial.println(localPort);
#else
  Serial.println(Udp.localPort());
#endif

  outIP[2] = hostPartA;
  outIP[3] = EEPROM.read(0);

}

void getMessage() {
  OSCMessage msg;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      msg.dispatch("/computerID", computerID);
      msg.dispatch("/hi", reply);
    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}
void computerID(OSCMessage &msg) {
  int message = msg.getInt(0);
  Serial.println(message);

}
void reply(OSCMessage &msg) {//reply with the same number as send previously
  int message = msg.getInt(0);

  OSCMessage response("/response");
  response.add((int)message);

  Udp.beginPacket(outIP, outPort);
  response.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  response.empty(); // free space occupied by message
  state = !state;
}

void confirmation(OSCMessage &msg) {
  bool confirm = msg.getInt(0);
}

void awake() { //send my IP over OSC
  Serial.print("Sending message to: ");
  Serial.print(outIP);
  Serial.println(" @ /deviceID");
  OSCMessage response("/deviceID");
  response.add((int)deviceID);
  Udp.beginPacket(outIP, outPort);
  response.send(Udp);
  Udp.endPacket();
  response.empty();
}



