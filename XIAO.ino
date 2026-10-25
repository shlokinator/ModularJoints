#include <ESP32Servo.h>
#include <WiFi.h>
#include <esp_now.h>


/* GENERAL THIGNS TO KNOW
myServo.write(position); is how you move servos around. myServo is the name of the servo variable I made.
To look at stuff being printed, click "Tools" and "Serial Monitor", and in the bottom of the Arduino IDE you should see one tab that says Output and another that says "Serial Monitor"
Click into the Serial Monitor tab after uploading your code. 
*/


Servo myServo;
const int servoPin = D5; // Choose an appropriate GPIO pin
const int button = D6;

int servoPos = 0;

// THIS IS THE CODE FOR THE GRAY MODULE WITH THE BLUE HINGE, THIS IS THE BOTTOM ONE




uint8_t broadcastAddress[] = {0x14, 0x63, 0x93, 0xBE, 0x2A, 0xA0}; //THIS IS A MAC ADDRESS, TO GET AN ESP32S MAC ACCRESS DO WiFi.macAddress()

//SETUP CODE FOR ESP32 NOW SENDING

//THIS IS THE PART OF THE CODE THAT SENDS A MESSAGE, IT'S KIND OF LIKE A DICTIONARY IN PYTHON. WE GIVE "A" A VALUE LATER
typedef struct struct_message {
  int a;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const wifi_tx_info_t* info, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


//ALL THE CODE IN SETUP RUNS ONE TIME WHEN THE ESP32 IS FIRST POWERED
void setup() {
  
  // Allow allocation of all timers, this helps with servo things I think
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);


  //You need to do this in setup to be able to print stuff. 
  Serial.begin(115200);

  // Attach hardware 
  pinMode(button, INPUT_PULLUP);
  myServo.setPeriodHertz(50);         // Standard 50Hz servo
  myServo.attach(servoPin, 500, 2400); // Attach pin with min/max pulse widths

  // Attach wifi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Failed to initialize");
    return;
  }

  esp_now_register_send_cb(OnDataSent);


  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("FAILED to add peer");
    return;
  }
}


//THIS LOOP RUNS INFINITELY AS LONG AS THE ESP32 IS RECEIVING POWER
void loop() {



  myData.a = 102; //THIS IS WHERE WE GIVE "A" A VALUE FROM EARLIER 
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData)); //THIS IS WERE WE SEND IT 

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);

  int pressed = (digitalRead(button) == LOW);

  myServo.write(90);

/*
  servoPos = 0;
  myServo.write(servoPos);
  Serial.println(servoPos);
  sleep(1);
  servoPos = 90;
  myServo.write(servoPos);
  Serial.println(servoPos);
  sleep(1);
  servoPos = 180;
  myServo.write(servoPos);
  Serial.println(servoPos);
  sleep(1);
*/
  Serial.println(WiFi.macAddress());


}