//this code is meant for testing the arduino mkr 1010 with a wired connection.
//In other words, only use this if you have a wired connection from a computer to the arduino as 
//this code prints information to a serial monitor.

//Author: Ryan Roberts
//Email: roberyan@oregonstate.edu

#include <SPI.h>
#include <WiFiNINA.h>

//imu things
#include <SparkFunMPU9250-DMP.h>
#define SerialPort SerialUSB
MPU9250_DMP imu;

#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiClient client;
int time;

WiFiServer server(80); //using port 80 for connection

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: "); Serial.println(ssid);
    status = WiFi.begin(ssid, pass); //if using non-encrypted wifi: remove pass parameter i.e. WiFi.begin(ssid)
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();

  //if IMU isn't responding properly, don't continue
  if (imu.begin() != INV_SUCCESS){
    while (1){
      SerialPort.println("Unable to communicate with MPU-9250");
      delay(2500);
    }
  }
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);
  imu.setGyroFSR(2000); // Set gyro to 2000 dps
  imu.setAccelFSR(2); // Set accel to +/-2g
  imu.setLPF(5); // Set LPF corner frequency to 5Hz
  imu.setSampleRate(10); // Set sample rate to 10Hz
  imu.setCompassSampleRate(10); // Set mag rate to 10Hz
}


void loop() {
  String time_val = String("");
  // listen for incoming clients
  client = server.available();
  //connection was successful.
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read(); //reads char from the established client. Used for entering length of data capture
        Serial.write(c); //prints value recieved from client to serial monitor for confirmation
        if (c == 10) { //ascii value of 10 is the 'enter' key
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
         // client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("This is where IMU data goes:");
          time = millis();
          int cap = time + time_val.toInt();
          while(time < cap){
          if (imu.dataReady()){
            imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
            printIMUData();
          }
          time = millis();
          }
          client.println("</html>");
          break;
        }
        else{
          time_val += String(c - 48); //converts char to int type
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address (use this for Telnet connection): ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void printIMUData(void){  
  float accelX = imu.calcAccel(imu.ax);
  float accelY = imu.calcAccel(imu.ay);
  float accelZ = imu.calcAccel(imu.az);
  float gyroX = imu.calcGyro(imu.gx);
  float gyroY = imu.calcGyro(imu.gy);
  float gyroZ = imu.calcGyro(imu.gz);
  float magX = imu.calcMag(imu.mx);
  float magY = imu.calcMag(imu.my);
  float magZ = imu.calcMag(imu.mz);

  client.println("Accel: " + String(accelX) + ", " + String(accelY) + ", " + String(accelZ) + " g,");
  client.println("Gyro: " + String(gyroX) + ", " + String(gyroY) + ", " + String(gyroZ) + " dps,");
  client.println("Mag: " + String(magX) + ", " + String(magY) + ", " + String(magZ) + " uT,");
  client.println("Time: " + String(imu.time) + " ms,");
  client.println("<br />");
}


