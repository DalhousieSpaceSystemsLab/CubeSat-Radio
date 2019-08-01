//This code sets up an arduino UNO to act as a dummy device.
//The purpose is to test the data link layer code including serial communications

int incomingByte = 0;    // for incoming serial data

void setup() {
    Serial.begin(9600);    // opens serial port, sets data rate to 9600 bps
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
  
    // read the incoming byte:
    incomingByte = Serial.read();
    if(incomingByte == 49){
      Serial.print("got 1\n");
    }
    // say what you got:
    Serial.print((char)incomingByte);
  }
  
}
