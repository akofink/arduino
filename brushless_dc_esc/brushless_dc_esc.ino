int out1 = 3;
int out2 = 5;
int out3 = 6;
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
  pinMode(test, OUTPUT);
  Serial.begin(9600);
}

void loop() {
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
  
  // Calculate offset
  period = 1.0 / frequency;
  offset = 1.0 / (3 * frequency);
  
  // Step
  step_count++;
  x += period / points_per_period;
  
  // Sinusoidally Modulate Pulse
  analogWrite(out1, f(x));
  analogWrite(out2, f(x - offset));
  analogWrite(out3, f(x - 2 * offset));
  
  if (!(2 * step_count % round(points_per_period))) {
    digitalWrite(test, (test_val = !test_val)); // Frequency Check  
  }
  
  delay(1000 * period / points_per_period);
}

int f(float x) {
  return abs(255 * (pow(sin(2 * PI * frequency / 2 * x), 20)));
}
