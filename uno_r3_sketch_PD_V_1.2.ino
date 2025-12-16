const int TRIG_PIN = 12; 
const int ECHO_PIN = 11; 
const int BUZZER_PIN = 8; 
const int LED_PIN = 13; 

const int ALARM_THRESHOLD_CM = 100; 
const long READING_INTERVAL_MS = 250; 
const int ALARM_DURATION_MS = 5000; 
const int PULSE_RATE_MS = 100; 

long lastReadingTime = 0;   
long lastToggle = 0; 
bool alarmState = LOW; 

long alarmTriggerTime = 0; 

void setup() {
  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);  
  pinMode(BUZZER_PIN, OUTPUT); 
  pinMode(LED_PIN, OUTPUT); 
}

void loop() {
  
  if (millis() - lastReadingTime >= READING_INTERVAL_MS) {
    lastReadingTime = millis();
    
    int currentDistance = getDistanceCm();
    
    if (currentDistance > 0 && currentDistance < ALARM_THRESHOLD_CM) {
      if (alarmTriggerTime == 0 || millis() - alarmTriggerTime > ALARM_DURATION_MS) {
         alarmTriggerTime = millis();
      }
    }
  }

  if (alarmTriggerTime != 0 && millis() - alarmTriggerTime < ALARM_DURATION_MS) {
    handleAlarmOutput(true);
  } else {
    handleAlarmOutput(false);
    alarmTriggerTime = 0; 
  }
}

int getDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  return duration / 58; 
}

void handleAlarmOutput(bool isAlarmActive) {
  if (isAlarmActive) {
    if (millis() - lastToggle >= PULSE_RATE_MS) {
      lastToggle = millis(); 
      
      alarmState = !alarmState; 
      
      digitalWrite(BUZZER_PIN, alarmState);
      digitalWrite(LED_PIN, alarmState);
    }
  } else {
    if (alarmState == HIGH) {
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
      alarmState = LOW;
      lastToggle = 0; 
    }
  }
}