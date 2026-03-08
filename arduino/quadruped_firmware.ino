#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define FREQUENCY_OSCILLATOR 25000000
#define SERVO_FREQ 50
#define SERVO_MIN 102
#define SERVO_MAX 500

// Servos that should convert negative angles to positive angles
// Add the channels that aren't working properly here
int negativeAngleServos[] = {1, 5, 8, 10}; // front_left_knee, front_right_ankle, rear_right_ankle, rear_left_knee
int numNegativeAngleServos = 4;

void setup() {
  Serial.begin(9600);
  // Identify the firmware version and feature in use
  Serial.println("R3 - Basic Servo Control w/ Negative Angle Handling");
  
  Wire.begin();
  pwm.begin();
  pwm.setOscillatorFrequency(FREQUENCY_OSCILLATOR);
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);
  
  // Notify that initialization is complete and commands can be sent
  Serial.println("System initialized. Awaiting commands.");
  Serial.println("Format: servo [channel] [angle]");
  Serial.println("Usage example: servo 0 90");
  
  // List all channels that apply the negative-to-positive angle conversion
  Serial.print("Channels w/ neg. angle fix applied: ");
  for(int i = 0; i < numNegativeAngleServos; i++) {
    Serial.print(negativeAngleServos[i]);
    if(i < numNegativeAngleServos - 1) Serial.print(", ");
  }
  Serial.println();
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    if (input.startsWith("servo ")) {
      // Parse: "servo 0 90" or "servo 1 -35"
      int space1 = input.indexOf(' ');
      int space2 = input.indexOf(' ', space1 + 1);
      
      if (space1 > 0 && space2 > 0) {
        int channel = input.substring(space1 + 1, space2).toInt();
        int angle = input.substring(space2 + 1).toInt();
        
        setServoAngle(channel, angle);
        // Confirm the command was executed
        Serial.print("Ch.");
        Serial.print(channel);
        Serial.print(" set to ");
        Serial.print(angle);
        Serial.println(" deg");
      }
    } else {
      // Echo back unrecognized input for debugging
      Serial.print("Echo: ");
      Serial.println(input);
    }
  }
}

bool isNegativeAngleServo(int channel) {
  // Check if this channel is in the negative angle correction list
  for(int i = 0; i < numNegativeAngleServos; i++) {
    if(negativeAngleServos[i] == channel) {
      return true;
    }
  }
  return false;
}

void setServoAngle(int channel, int angle) {
  int originalAngle = angle;
  
  // Handle negative angles for specific servos
  if (isNegativeAngleServo(channel) && angle < 0) {
    angle = abs(angle); // Convert negative to positive
    // Log the angle correction applied to this channel
    Serial.print("Ch.");
    Serial.print(channel);
    Serial.print(": angle adjusted ");
    Serial.print(originalAngle);
    Serial.print("deg -> ");
    Serial.print(angle);
    Serial.println("deg");
  }
  
  // Constrain to servo range
  angle = constrain(angle, 0, 180);
  
  // Convert angle to PWM pulse length
  uint16_t pulseLength = map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
  pwm.setPWM(channel, 0, pulseLength);
}