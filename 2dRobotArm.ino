


#include <Servo.h>

Servo servo1;      // Joint at base
Servo servo2;      // Elbow between a and b

// Servo Angles
float servo1_Angle = 90;
float servo2_Angle = 90;

// Define arm Constants
const float a = 30;      // lower joint length (cm)
const float b = 30;      // upper joint length (cm)

// Correction factors to align servo values with their respective axis
const float S_1_CorrectionFactor = 0;     // Align arm "a" with the horizontal when at 0 degrees
const float S_2_CorrectionFactor = 180;     // Align arm "b" with arm "a" when at 0 degrees

// Correction factor to shift origin out to edge of the mount
const float X_CorrectionFactor = 0;       // X direction correction factor (cm)
const float Y_CorrectionFactor = 0;       // Y direction correction factor (cm)

// Angle Variables
float A;            //Angle oppposite side a (between b and c)
float B;            //Angle oppposite side b
float C;            //Angle oppposite side c
float theta;        //Angle formed between line from origin to (x,y) and the horizontal

// Distance variables
float x;            // x position (cm)
float y;            // y position (cm)
float c;            // Hypotenuse legngth in cm
const float pi = M_PI;  //Store pi in a less annoying format

// Calculate necessary servo angles to move arm to desired points
void MoveCoord(float x_input, float y_input)
{
  x = x_input + X_CorrectionFactor;
  y = y_input + Y_CorrectionFactor;
  if (x < 0) {
    c = -sqrt( sq(x) + sq(y) );
    B = (acos( (sq(b) - sq(a) - sq(c)) / (-2 * a * c) )) * (180 / pi);    // Law of cosines: Angle opposite upper arm section
    C = -(acos( (sq(c) - sq(b) - sq(a)) / (-2 * a * b) )) * (180 / pi); // Law of cosines: Angle opposite hypotenuse
  }
  else {
    c = sqrt( sq(x) + sq(y) );
    B = -(acos( (sq(b) - sq(a) - sq(c)) / (-2 * a * c) )) * (180 / pi);    // Law of cosines: Angle opposite upper arm section
    C = -(acos( (sq(c) - sq(b) - sq(a)) / (-2 * a * b) )) * (180 / pi); // Law of cosines: Angle opposite hypotenuse
  }
  theta = (asin( y / c )) * (180 / pi);                                 // Solve for theta to correct for lower joint's impact on upper joint's angle
  servo1_Angle = B + theta + S_1_CorrectionFactor;                    // Find necessary angle. Add Correction
  servo2_Angle = C + S_2_CorrectionFactor;                            // Find neceesary angle. Add Correction
  servo1.write(servo1_Angle);              // Move joint to desired position
  servo2.write(servo2_Angle);

}



int x_now = 0;
int y_now = 0;

void setup()
{
  servo1.attach(9);             // Attach servos
  servo2.attach(10);
  Serial.begin(9600);             // -For debugging

}
//--------------------------------------------------------------

void Debug(){
      Serial.print("x: ");
      Serial.println(x_now);
      Serial.print("y: ");
      Serial.println(y_now);
      Serial.print("Servo1 angle: ");
      Serial.println(servo1_Angle);
      Serial.print("Servo2 angle: ");
      Serial.println(servo2_Angle);
}
void loop()
{
  if (Serial.available() > 0) {
    char com_dc  = Serial.read();
    Serial.println(com_dc);
    if (com_dc == '1') {
      x_now++ ;
     Debug();

    }
    if (com_dc == '2') {
      x_now--;
      Debug();

    }
    if (com_dc == '3') {
      y_now++ ;
      Debug();

    }
    if (com_dc == '4') {
      y_now--;
      Debug();

    }

  }
  MoveCoord(x_now, y_now);
}
