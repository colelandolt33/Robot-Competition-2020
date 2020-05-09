#include <Wire.h>
#include <ZumoShield.h>

ZumoMotors motors;
ZumoReflectanceSensorArray linesensors(QTR_NO_EMITTER_PIN);

void setup() {
  int i;
  int spin_direction = 1;
  motors.setSpeeds(80*spin_direction, -80*spin_direction);
  for(i = 0; i<100; i++){
    linesensors.calibrate();
    if(i%50 == 25){ // every 50 loops, starting on loop 25...
      spin_direction = -spin_direction;
      motors.setSpeeds(80*spin_direction, -80*spin_direction);
    }
    delay(20);
  }
  motors.setSpeeds(0,0);
  delay(500);
}

unsigned int sensor_vals[6];
int LEFT_SPEED = 300;
int RIGHT_SPEED = 0.96 * LEFT_SPEED;
double PROPORTION_GAIN = 0.2;
double DERIVATIVE_GAIN = 3;
double INTEGRAL_GAIN = 0;
int last_error = 0;
int total_error = 0;
void loop() {
  int line_position = linesensors.readLine(sensor_vals);
  int error = line_position - 2500;
  int error_change = error - last_error;
  int left_speed = LEFT_SPEED + PROPORTION_GAIN * error + DERIVATIVE_GAIN * error_change + INTEGRAL_GAIN * total_error;
  int right_speed = RIGHT_SPEED + -PROPORTION_GAIN * error + -DERIVATIVE_GAIN * error_change + INTEGRAL_GAIN * total_error;
  last_error = error;
  total_error = total_error + error;
  motors.setSpeeds(left_speed, right_speed);
}
