#include "Wire.h"
#include "PowerfulBLDCdriver.h"

PowerfulBLDCdriver motor1;
PowerfulBLDCdriver motor2;
PowerfulBLDCdriver motor3;
PowerfulBLDCdriver motor4;

//yaw is the direction that we care abt
#include <Adafruit_BNO08x.h>
#include <Wire.h>

float headingCorrection = 0;

#define BNO08X_RESET -1

Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

void setup() {
  
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit BNO08x test!");

  Wire.setSCL(9);
  Wire.setSDA(8);
  Wire.begin();
  
  // Try to initialize!
  if (!bno08x.begin_I2C(74, &Wire)) {
    Serial.println("Failed to find BNO08x chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("BNO08x Found!");

  setReports();

  Serial.println("Reading events");
  delay(100);
  
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


void setReports(void) {
  Serial.println("Setting desired reports");
  if (!bno08x.enableReport(SH2_GAME_ROTATION_VECTOR)) {
    Serial.println("Could not enable game rotation vector");
  }
}

void quaternionToEuler(float w, float x, float y, float z, float &roll, float &pitch, float &yaw) {
    // Roll (x-axis rotation)
    float sinr_cosp = 2 * (w * x + y * z);
    float cosr_cosp = 1 - 2 * (x * x + y * y);
    roll = atan2(sinr_cosp, cosr_cosp);

    // Pitch (y-axis rotation)
    float sinp = 2 * (w * y - z * x);
    pitch = fabs(sinp) >= 1 ? copysign(M_PI / 2, sinp) : asin(sinp);

    // Yaw (z-axis rotation)
    float siny_cosp = 2 * (w * z + x * y);
    float cosy_cosp = 1 - 2 * (y * y + z * z);
    yaw = atan2(siny_cosp, cosy_cosp);

    // Convert radians to degrees
    roll *= 180 / M_PI;
    pitch *= 180 / M_PI;
    yaw *= 180 / M_PI;

    // yaw is reversed (back of robot is 0) so convert
    if (yaw <= 0) {
      yaw += 180;
    } else {
      yaw -= 180;
    }
    
}

void spinAround(float speed) {
  // Speed can be between +1 and -1, +1 spins clockwise, -1 spins counterclockwise
  
  float speed1 = speed;
  float speed2 = speed;
  float speed3 = speed;
  float speed4 = speed;

  // Scale speeds to motor speed range (max is 90000000)
  float maxSpeed = 45000000;

  float scaledSpeed1 = speed1 * maxSpeed;
  float scaledSpeed2 = speed2 * maxSpeed;
  float scaledSpeed3 = speed3 * maxSpeed;
  float scaledSpeed4 = speed4 * maxSpeed;
  
  motor1.setSpeed(scaledSpeed1);
  motor2.setSpeed(scaledSpeed2);
  motor3.setSpeed(scaledSpeed3);
  motor4.setSpeed(scaledSpeed4);
}

void stopMoving() {
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor3.setSpeed(0);
  motor4.setSpeed(0);
}

void setMotorSpeed(float angle, float rotation) {
  // If the motors are at a special angle in design
  float motorAngle = 60 * (PI / 180.0);

  // Convert angle to radians
  float radian = (angle) * (PI / 180.0);

  // Get heading correction from IMU
  //headingCorrection = correctHeading();
  
  // Calculate motor speeds
  float speed1 = cos(radian + PI/6);
  float speed2 = sin(radian + PI/3);
  float speed3 = -cos(radian + PI/6);
  float speed4 = -sin(radian + PI/3);

  // Calculate a multiplier to ensure motors are at maximum speed while maintaining angle ratio
  float speedMultiplier = 1;

  //Serial.println(speed1);
  speedMultiplier = 1.0/max(abs(speed1), abs(speed2), abs(speed3), abs(speed4))
  /*if ( abs(speed1) > abs(speed2) ) {
    speedMultiplier = 1.0/abs(speed1);
  } 
  else( abs(speed2) > abs(speed1) ); {
    speedMultiplier = 1.0/abs(speed2);
  }*/
  Serial.println(speedMultiplier);

  // Scale speeds to motor speed range (max is 90000000)
  float maxSpeed = 60000000;
  // Scale speeds to motor speed range (max is 90000000)
  float maxSpinSpeed = 30000000;

  float scaledSpeed1 = speed1 * maxSpeed * speedMultiplier + rotation * maxSpinSpeed;
  float scaledSpeed2 = speed2 * maxSpeed * speedMultiplier + rotation * maxSpinSpeed;
  float scaledSpeed3 = speed3 * maxSpeed * speedMultiplier + rotation * maxSpinSpeed;
  float scaledSpeed4 = speed4 * maxSpeed * speedMultiplier + rotation * maxSpinSpeed;

  // Set the motor speeds
  motor1.setSpeed(scaledSpeed1);
  motor2.setSpeed(scaledSpeed2);
  motor3.setSpeed(scaledSpeed3);
  motor4.setSpeed(scaledSpeed4);

  // Testing purposes
  Serial.println(scaledSpeed1);
  Serial.println(scaledSpeed2);
}

void correctHeading(float heading, float desiredHeading) {
  // assuming heading is received from imu as 180 to -180, with 0 being straight (opposing wall)
  // if not, then make it work !!!
  //float headingCorrection = heading/PI;

  // Scale speeds to motor speed range (max is 90000000)
  float maxSpinSpeed = 30000000;

  rotation = abs((((desiredHeading - heading) % 360)-180)/180);
  if (desiredHeading - heading < 0) {
    rotation *= -1
  }

  //headingCorrection = rotation * maxSpinSpeed;
  //return scaledHeadingCorrection;
}

void loop() {
  // Set the desired angle here (in degrees, 0-360)
  float angle = 0; // Change this value to set a different angle
  //spinAround(1); // Spin anticlockwise
  //setMotorSpeed(angle);
  delay(1); // Adjust delay as needed

  if (bno08x.wasReset()) {
    Serial.print("sensor was reset ");
    setReports();
  }

  if (!bno08x.getSensorEvent(&sensorValue)) {
    return;
  }

  if (sensorValue.sensorId == SH2_GAME_ROTATION_VECTOR) {
    float w = sensorValue.un.gameRotationVector.real;
    float x = sensorValue.un.gameRotationVector.i;
    float y = sensorValue.un.gameRotationVector.j;
    float z = sensorValue.un.gameRotationVector.k;

    float roll, pitch, yaw;
    quaternionToEuler(w, x, y, z, roll, pitch, yaw);

    Serial.print("Direction - Roll: ");
    Serial.print(roll);
    Serial.print(" degrees, Pitch: ");
    Serial.print(pitch);
    Serial.print(" degrees, Yaw: ");
    Serial.println(yaw); // yaw is here!

    correctHeading(yaw, 0);
    //spinAround(-yaw/180);
  }
  setMotorSpeed(angle, );
}
