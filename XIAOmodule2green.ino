#include <ESP32Servo.h>
#include <WiFi.h>
#include <esp_now.h>

//THIS IS A RECEIVER FILE, READ THE SENDER FILE FIRST IF YOU CAN
//THIS IS THE RECEIVER MESSAGE STRUCTURE, IT NEEDS TO MATCH THE ONE IN THE SENDER FILE. 
typedef struct struct_message {
    int a;
} struct_message;

struct_message message;


//THIS FUNCTION AUTOMATICALLY RUNS WHEN IT RECEIVES DATA FROM THE OTHER ESP32, IT DOESN'T NEED TO BE CALLED LIKE MOST FUNCTIONS
//THIS IS WHERE YOU SHOULD MAKE THE SERVOS MOVE, HAVE EACH ESP32 MOVE IN RESPONSE TO GETTING A SIGNAL FROM THE OTHER SERVO
//FOR EXAMPLE, THE BOTTOM SERVO CAN MOVE AND THEN SEND IT'S POSITION VALUE TO THE TOP SERVO, WHICH CAN MOVE TO THAT POSITION AS WELL OR THAT POSITION PLUS ANOTHER NUMBER
void onDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&message, incomingData, sizeof(message));
  Serial.println("SUCCESS");
  Serial.println(message.a);
}

Servo myServo;
const int servoPin = D5; // Choose an appropriate GPIO pin
const int button = D6;

int servoPos = 0;

// THIS IS THE CODE FOR THE GRAY MODULE WITH THE BLUE HINGE

void setup() {
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);


  Serial.begin(115200);
  pinMode(button, INPUT_PULLUP);
  myServo.setPeriodHertz(50);         // Standard 50Hz servo
  myServo.attach(servoPin, 500, 2400); // Attach pin with min/max pulse widths

  //WIFI SETUP FOR RECEIVING
  WiFi.mode(WIFI_STA);

  if(esp_now_init() != ESP_OK){
    Serial.println("ERROR");
    return;
  }
  esp_now_register_recv_cb(onDataRecv);

}

void loop() {

  int pressed = (digitalRead(button) == LOW);

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
  sleep(1);*/


}