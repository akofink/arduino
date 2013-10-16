int potpin = 0;
int outpin = 9;
int output_value = 0;
int read_val = -1;

void setup()
{
  pinMode(outpin, OUTPUT);
  Serial.begin(9600);
  while (!Serial){
    ;
  }
}

void loop()
{
  if ((read_val = Serial.read()) > -1 && read_val != 10) {
    read_val -= 48;
    output_value = read_val;
    int place = 10;
    while ((read_val = Serial.read()) > -1 && read_val != 10) {
      read_val -= 48;
      output_value *= place;
      output_value += int(read_val);
    }
    Serial.print("Setting the output value to ");
    Serial.println(output_value % 256, DEC);
    analogWrite(outpin, output_value % 256);
  }
  delay(100);
}
