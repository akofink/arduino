int current_value = 0;

void setup() { 
 //Initialize serial and wait for port to open:
  Serial.begin(9600);
} 

void loop() {
  Serial.print("Test ");
  Serial.println(current_value++);
  delay(1000);
} 
