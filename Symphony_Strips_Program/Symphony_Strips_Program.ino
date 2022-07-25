/*
 * Symphony_Strips_Program
 * 
 * Shad Canada 2022 at Carleton University
 * Design Project 2
 * A program for an Arduino UNO R3 with 5 ultrasonic sensors and 5 piezo buzzers
 * Credits: Joshua Dierickse and Armand Lupien 
 */

int echo_pins[5] = {2, 4, 7, 8, 12};
int trig_pins[5] = {3, 5, 6, 9, 10};
int distance_values[5] = {0, 0, 0, 0, 0};
long duration = 0;
int music_mode = 0;
int temp_music_mode = 0;

int music_array[4][5] = 
  {
  {440, 523, 659, 784, 880},
  {440, 466, 494, 523, 554},
  {554, 659, 932, 880, 698},
  {262, 294, 311, 392, 415}
  };

int distance_min = 3;
int distance_max = 50;
boolean buzzer_trigger[5] = {false, false, false, false, false};
char buzzer_pins[5] = {A0, A1, A2, A3, A4};

void setup() {
  for (int i = 0; i < 5; i++) {
    pinMode(trig_pins[i], OUTPUT);
    pinMode(echo_pins[i], INPUT);
  }

  for (int i = 0; i < 5; i++) {
    pinMode(buzzer_pins[i], OUTPUT);
  }

  Serial.begin(9600);
}

void loop() {

  if (Serial.available() > 0) {
    temp_music_mode = Serial.parseInt();
    if (temp_music_mode > 0) {
      music_mode = (temp_music_mode - 1 + 1);
    }
  }
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(trig_pins[i], LOW);
  }
  
  delayMicroseconds(2);
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(trig_pins[i], HIGH);
  }
  
  delayMicroseconds(10);

  for (int i = 0; i < 5; i++) {
    digitalWrite(trig_pins[i], LOW);
    duration = pulseIn(echo_pins[i], HIGH);
    distance_values[i] = duration * 0.034 / 2;
  }

  for (int i = 0; i < 5; i++) {
    if (distance_values[i] > distance_min && distance_values[i] < distance_max) {
      buzzer_trigger[i] = true;
    }
    else {
      buzzer_trigger[i] = false;
    }
  }
  
  /*
  Serial.print("\nbuzzer trigger array: "); //optionally, print out sensor states
  for (int i = 0; i < 5; i++) {
    Serial.print(buzzer_trigger[i]);
    Serial.print(" ");
  }
  */
  
  for (int i = 0; i < 5; i++) {
    if (buzzer_trigger[i] == 1) {
      if (music_mode == 5) {
        tone(buzzer_pins[i], 500 + (distance_values[i] * 5));
      }
      else {
        tone(buzzer_pins[i], music_array[music_mode - 1][i]);
      }
    }
    else {
      noTone(buzzer_pins[i]);
    }
  }
}
