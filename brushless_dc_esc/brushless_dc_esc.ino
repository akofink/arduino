int out1 = 3;
int out2 = 5;
int out3 = 6;
int out4 = 9;
int out5 = 10;
int out6 = 11;

int test = 13;
int test_val = 0;

int duty = 255;

int read_val = 0;
float read_frequency; // Calculated from read_val
float decimal_position;

float frequency = 1; // Default value of 1Hz

void setup() {
  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT);
  pinMode(out4, OUTPUT);
  pinMode(out5, OUTPUT);
  pinMode(out6, OUTPUT);
  pinMode(test, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  handleSerial();
  digitalWrite(test, test_val++ % 2);
  
  write(duty, 0, 0, duty, 0, duty);
  d();
  write(duty, 0, 0, duty, 0, duty);
  d();
  write(0, duty, duty, 0, 0, duty);
  d();
  write(0, duty, duty, 0, 0, duty);
  d();
  write(0, duty, 0, duty, duty, 0);
  d();
  write(0, duty, 0, duty, duty, 0);
  d();
}

void write(float a, float b, float c, float d, float e, float f) {
  analogWrite(out1, a);
  analogWrite(out2, b);
  analogWrite(out3, c);
  analogWrite(out4, d);
  analogWrite(out5, e);
  analogWrite(out6, f);
}

void d() {
  delay(1000.0 * 1.0 / (6.0 * frequency));
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
        if (read_val == 66 && frequency > 1) { // Down
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
