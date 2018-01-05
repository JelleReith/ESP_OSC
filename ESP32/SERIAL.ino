String help = "-help";
String change_ip = "-change_ip";
String show_ip = "-show_ip";
String awake_message = "-awake";
String show_wifi = "-show_wifi";
String change_wifi = "-change_wifi";
String connect_wifi = "-connect_wifi";
String inputString = "";
boolean stringComplete = false;
boolean correct_message = false;
int lastDigits = 0;
unsigned int timeout = 0;

void init_SERIAL() {
  EEPROM.begin(512);
}

void prompt_setup() {
  Serial.println();
  Serial.println();
  Serial.println("Hi there! How are you doing?");
  Serial.println();
  Serial.println("Type -help for help");
  Serial.println("Make sure the serial monitor is set to 'Newline'");
  Serial.println();
}
void serialEvent() {
  correct_message = false;
  while (Serial.available() == 1) {
    inputString = Serial.readStringUntil('\n');

    if (inputString.equals(change_ip)) {
      Serial.println("Please enter the last digits of the new IP address within five seconds");
      for (int i = 0; i < 50; i++) {
        delay(100);
        Serial.print(".");
        if (Serial.available()) {
          break;
        }
      }
      Serial.println();
      lastDigits = Serial.parseInt();
      Serial.print("Thank you, changing the IP address in the flash memory to this: ");
      Serial.println(lastDigits);
      Serial.println();
      EEPROM.write(0, lastDigits);
      EEPROM.commit();
      outIP[3] = EEPROM.read(0);
      inputString = "";
      stringComplete = false;
      correct_message = true;
    }

    else if (inputString.equals(show_ip)) {
      Serial.print("The current IP address stored in flash memory is this: ");
      Serial.println(EEPROM.read(0));
      Serial.println();
      inputString = "";
      stringComplete = false;
      correct_message = true;
    }

    else if (inputString.equals(awake_message)) {
      awake();
      inputString = "";
      stringComplete = false;
      correct_message = true;
    }
    else if (inputString.equals(show_wifi)) {
      Serial.print("SSID stored in flash: ");
      Serial.println(read_ssid());
      Serial.print("PSK stored in flash: ");
      Serial.println(read_pass());
      inputString = "";
      stringComplete = false;
      correct_message = true;
    }
    else if (inputString.equals(change_wifi)) {
      Serial.println("Please enter the SSID");
      for (int i = 0; i < 50; i++) {
        delay(100);
        Serial.print(".");
        if (Serial.available()) {
          break;
        }
      }
      Serial.println();
      inputString = Serial.readStringUntil('\n');
      Serial.println("Writing SSID to flash");
      write_ssid(inputString);


      Serial.println("Please enter the PSK");
      for (int i = 0; i < 50; i++) {
        delay(100);
        Serial.print(".");
        if (Serial.available()) {
          break;
        }
      }
      Serial.println();
      inputString = Serial.readStringUntil('\n');
      Serial.println("Writing PSK to flash");
      write_pass(inputString);
      connect_to_wifi();
      inputString = "";
      stringComplete = false;
      correct_message = true;
    }
    else if (inputString.equals(connect_wifi)) {
      connect_to_wifi();
      inputString = "";
      stringComplete = false;
      correct_message = true;
    }
    else if (inputString.equals(help)) {
      Serial.println("The following commands can be used");
      Serial.println();
      Serial.println("-change_ip to change the IP stored in flash memory");
      Serial.println("-show_ip to check the current IP stored in flash memory");
      Serial.println("-awake to send a OSC message to the set IP");
      Serial.println("-show_wifi to check the current SSID and PSK for the wifi connection");
      Serial.println("-change_wifi to change the SSID and PSK stored in flash memory");
      Serial.println("-connect_wifi to connect to wifi");
      Serial.println();
      inputString = "";
      stringComplete = false;
      correct_message = true;
    }
  }
  if (!correct_message) {
    Serial.print("Error: ");
    Serial.print(inputString);
    Serial.println(" was not a correct message");
    inputString = "";
    stringComplete = false;
  }
}


String read_ssid() {
  int length_of_string = EEPROM.read(10);
  char char_array[length_of_string];
  for (int i = 0; i < length_of_string; i++) {
    char_array[i] = EEPROM.read(11 + i);
  }
  String return_string(char_array);
  return_string.remove(length_of_string);
  return return_string;
}

String read_pass() {
  int length_of_string = EEPROM.read(100);
  char char_array[length_of_string];
  for (int i = 0; i < length_of_string; i++) {
    char_array[i] = EEPROM.read(101 + i);
  }
  String return_string(char_array);
  return_string.remove(length_of_string);
  return return_string;
}

void write_ssid(String ssid_to_write) {
  Serial.println(ssid_to_write);
  int length_of_ssid = ssid_to_write.length();
  EEPROM.write(10, length_of_ssid);
  for (int i = 0; i < length_of_ssid; i++) {
    EEPROM.write(11 + i, ssid_to_write[i]);
    Serial.print("WROTE: ");
    Serial.print(ssid_to_write[i]);
    Serial.print(" here: ");
    Serial.println(11 + i);
  }
  EEPROM.commit();
}

void write_pass(String pass_to_write) {
  Serial.println(pass_to_write);
  int length_of_pass = pass_to_write.length();
  EEPROM.write(100, length_of_pass);
  for (int i = 0; i < length_of_pass; i++) {
    EEPROM.write(101 + i, pass_to_write[i]);
    Serial.print("WROTE: ");
    Serial.print(pass_to_write[i]);
    Serial.print(" here: ");
    Serial.println(101 + i);
  }
  EEPROM.commit();
}


