#include "Wire.h"
#include "PowerfulBLDCdriver.h"

PowerfulBLDCdriver motor1;
PowerfulBLDCdriver motor2;
PowerfulBLDCdriver motor3;
PowerfulBLDCdriver motor4;

void setup() {
  Wire.setSCL(9);
  Wire.setSDA(8);
  Serial.begin(115200); // initialise serial
  Wire.begin(); // initialise i2c0, make sure to look up the i2c pins of your microcontroller.
  Wire.setClock(1000000); // set i2c speed to 1MHz
  motor1.begin(25, &Wire); // motor1 has i2c address 25 and is using i2c0.
  motor1.setCurrentLimitFOC(65536*2); // set current limit to 1 amp (only works in FOC mode)
  motor1.setIdPidConstants(1500, 200); 
  motor1.setIqPidConstants(1500, 200);
  motor1.setSpeedPidConstants(4e-2, 4e-4, 3e-2); // Constants valid for FOC and Robomaster M2006 P36 motor only, see tuning constants document for more details
  motor2.begin(26, &Wire); // motor1 has i2c address 25 and is using i2c0.
  motor2.setCurrentLimitFOC(65536*2); // set current limit to 1 amp (only works in FOC mode)
  motor2.setIdPidConstants(1500, 200); 
  motor2.setIqPidConstants(1500, 200);
  motor2.setSpeedPidConstants(4e-2, 4e-4, 3e-2); // Constants valid for FOC and Robomaster M2006 P36 motor only, see tuning constants document for more details
  motor3.begin(27, &Wire); // motor1 has i2c address 25 and is using i2c0.
  motor3.setCurrentLimitFOC(65536*2); // set current limit to 1 amp (only works in FOC mode)
  motor3.setIdPidConstants(1500, 200); 
  motor3.setIqPidConstants(1500, 200);
  motor3.setSpeedPidConstants(4e-2, 4e-4, 3e-2); // Constants valid for FOC and Robomaster M2006 P36 motor only, see tuning constants document for more details
  motor4.begin(28, &Wire); // motor1 has i2c address 25 and is using i2c0.
  motor4.setCurrentLimitFOC(65536*2); // set current limit to 1 amp (only works in FOC mode)
  motor4.setIdPidConstants(1500, 200); 
  motor4.setIqPidConstants(1500, 200);
  motor4.setSpeedPidConstants(4e-2, 4e-4, 3e-2); // Constants valid for FOC and Robomaster M2006 P36 motor only, see tuning constants document for more details
  motor1.setELECANGLEOFFSET(1464903936); // set the ELECANGLEOFFSET calibration value. Each motor needs its own calibration value.
  motor1.setSINCOSCENTRE(1250); // set the SINCOSCENTRE calibration value. Each motor needs its own calibration value.
  motor1.configureOperatingModeAndSensor(3, 1); // configure FOC mode and sin/cos encoder
  motor1.configureCommandMode(12); // configure speed command mode
  motor2.setELECANGLEOFFSET(1656027136); // set the ELECANGLEOFFSET calibration value. Each motor needs its own calibration value.
  motor2.setSINCOSCENTRE(1242); // set the SINCOSCENTRE calibration value. Each motor needs its own calibration value.
  motor2.configureOperatingModeAndSensor(3, 1); // configure FOC mode and sin/cos encoder
  motor2.configureCommandMode(12); // configure speed command mode
  motor3.setELECANGLEOFFSET(1392448256); // set the ELECANGLEOFFSET calibration value. Each motor needs its own calibration value.
  motor3.setSINCOSCENTRE(1236); // set the SINCOSCENTRE calibration value. Each motor needs its own calibration value.
  motor3.configureOperatingModeAndSensor(3, 1); // configure FOC mode and sin/cos encoder
  motor3.configureCommandMode(12); // configure speed command mode
  motor4.setELECANGLEOFFSET(1275949824); // set the ELECANGLEOFFSET calibration value. Each motor needs its own calibration value.
  motor4.setSINCOSCENTRE(1259); // set the SINCOSCENTRE calibration value. Each motor needs its own calibration value.
  motor4.configureOperatingModeAndSensor(3, 1); // configure FOC mode and sin/cos encoder
  motor4.configureCommandMode(12); // configure speed command mode

  delay(500);
}

void setMotorSpeed(float angle) {
  // If the motors are at a special angle in design
  float motorAngle = 60 * (PI / 180.0);

  // Convert angle to radians
  float radian = (angle) * (PI / 180.0);

  // Calculate motor speeds
  float speed1 = cos(radian + PI/6);
  float speed2 = sin(radian + PI/3);
  float speed3 = -cos(radian + PI/6);
  float speed4 = -sin(radian + PI/3);

  // Calculate a multiplier to ensure motors are at maximum speed while maintaining angle ratio

  float speedMultiplier = 1;

  //Serial.println(speed1);

  if ( abs(speed1) > abs(speed2) ) {
    speedMultiplier = 1.0/abs(speed1);
  } 
  else( abs(speed2) > abs(speed1) ); {
    speedMultiplier = 1.0/abs(speed2);
  }
  Serial.println(speedMultiplier);

  // Scale speeds to motor speed range (max is 90000000)
  float maxSpeed = 45000000;

  float scaledSpeed1 = speed1 * maxSpeed * speedMultiplier;
  float scaledSpeed2 = speed2 * maxSpeed * speedMultiplier;
  float scaledSpeed3 = speed3 * maxSpeed * speedMultiplier;
  float scaledSpeed4 = speed4 * maxSpeed * speedMultiplier;

  // Set the motor speeds
  motor1.setSpeed(scaledSpeed1);
  motor2.setSpeed(scaledSpeed2);
  motor3.setSpeed(scaledSpeed3);
  motor4.setSpeed(scaledSpeed4);

  // Testing purposes
  Serial.println(scaledSpeed1);
  Serial.println(scaledSpeed2);
}

void loop() {
  // Set the desired angle here
  float angle = 90; // Change this value to set a different angle
  setMotorSpeed(angle);
  delay(1000); // Adjust delay as needed
}
