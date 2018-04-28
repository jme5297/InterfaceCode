#include <NewPing.h>

const int max_dist = 200;

// right sensor pins
const int trig_right = 12;
const int echo_right = 11;
const int signal_right = 10;
int distance_right = 0;
NewPing sonar_right(trig_right,echo_right,max_dist);

// center sensor pins
const int trig_center = 9;
const int echo_center = 8;
const int signal_center = 7;
int distance_center = 0;
NewPing sonar_center(trig_center,echo_center,max_dist);

// left sensor pins
const int trig_left = 6;
const int echo_left = 5;
const int signal_left = 4;
int distance_left = 0;
NewPing sonar_left(trig_left,echo_left,max_dist);

void setup() {
  Serial.begin(9600);
  pinMode(signal_left, OUTPUT);
  pinMode(signal_right, OUTPUT);
  pinMode(signal_center, OUTPUT);
}

void loop() {
  // Read distances
  distance_left = sonar_left.ping_cm();
  if (distance_left == 0) {
    distance_left = 200;
  }
  distance_right = sonar_right.ping_cm();
  if (distance_right == 0) {
    distance_right = 200;
  }
  distance_center = sonar_center.ping_cm();
  if (distance_center == 0) {
    distance_center = 200;
  }

  // cut signal if one of distances are less than 60 cm
  if (distance_left < 60){
    digitalWrite(signal_left,LOW);
    Serial.println("sensor tripped!\n");
  }
  if (distance_left >= 60){
    digitalWrite(signal_left,HIGH);
    Serial.println("sensor NOT tripped!\n");
  }
  if (distance_center < 60){
    digitalWrite(signal_center,LOW);
  }
  if (distance_center >= 60){
    digitalWrite(signal_center,HIGH);
  }
  if (distance_right < 60){
    digitalWrite(signal_right,LOW);
  }
  if (distance_right >= 60) {
    digitalWrite(signal_right,HIGH);
  }
  delay(4);
}

