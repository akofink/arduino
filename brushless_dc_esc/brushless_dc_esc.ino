int out1 = 3;
int enable1 = 5;
int out2 = 6;
int enable2 = 9;
int out3 = 10;
int enable3 = 11;

int test = 13;

int test_val = 0;

int read_val = 0;
float read_frequency; // Calculated from read_val
float decimal_position;

float frequency = 1; // Default value of 1Hz
float points_per_period = 12; // Data Points per Period
float offset;
float period;

int step_count = 0;

float x; // Our independant variable

void setup() {
  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT);
  pinMode(enable1, OUTPUT);
  pinMode(enable2, OUTPUT);
  pinMode(enable3, OUTPUT);
  pinMode(test, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  handleSerial();
  
  period = 1.0 / frequency;
  
  write(255, 255, 0, 255, 0, 0);
  d();
  write(255, 255, 0, 0, 0, 255);
  d();
  write(0, 0, 255, 255, 0, 255);
  d();
  write(0, 255, 255, 255, 0, 0);
  d();
  write(0, 255, 0, 0, 255, 255);
  d();
  write(0, 0, 0, 255, 255, 255);
  d();
}

void write(float x, float ex, float y, float ey, float z, float ez) {
  analogWrite(out1, x);
  analogWrite(out2, y);
  analogWrite(out3, z);
  analogWrite(enable1, ex);
  analogWrite(enable2, ey);
  analogWrite(enable3, ez);
}

void d() {
  delay(1000.0 * period / 6.0);
}

void handleSerial() {
  if (Serial.available() > 0) {
    while ((read_val = Serial.read()) > -1) {
      Serial.println(read_val);
      if (read_val == 46) { // Found a decimal point
        if (decimal_position == 1) {
          read_frequency = 0;
        }
        
        decimal_position = 0.1;
      } else if (read_val == 27 && Serial.read() == 91) { // Arrow
        read_val = Serial.read();
        if (read_val == 66) { // Down
          frequency--;
        } else if (read_val == 65) { // Up
          frequency++;
        }
        
        Serial.print("Setting the frequency to ");
        Serial.println(frequency, DEC);
      } else if (read_val == 13 || read_val == 10) { // Carriage Return
        if (read_frequency > 0) {
          frequency = read_frequency;
        }

        decimal_position = 1; // Reset numerical computation

        Serial.print("Setting the frequency to ");
        Serial.println(frequency, DEC);
      } else if (read_val > 47 && read_val < 58) { // Numerical
        read_val -= 48;
        
        if (decimal_position == 1) { // First numerical value read
          read_frequency = 0;
        }
        
        if (decimal_position > 0.1) {
          read_frequency *= 10;
          read_frequency += float(read_val);
          decimal_position *= 10;
        } else {
          read_frequency += float(read_val * decimal_position);
          decimal_position /= 10;
        }
      }
    }
  }
}
