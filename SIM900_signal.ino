#include <SoftwareSerial.h>
  SoftwareSerial SIM900 (6,7); // SIM900 connected to pins 6 & 7 (pin 6 to SIMT and pin 7 to SIMR)
  char Reply[200]; // Serial buffer length
  int number = 29; // as above -1
  int Signal; // Signal strength as reported
  int temp1; // 4 temporary integers, used to extract the data from the incoming serial.
  int temp2;
  int temp3;
  int temp4;
  int BER; // Bit error rate
  int SignaldBm; //Signal in dBm

void setup() 
{
  SIM900.begin(9600); // Start serial comms with module
  Serial.begin(57600); // Start serial comms with PC

}

void GetStatus() {

    SIM900.write("AT+CMGF=1\r");           //set GSM to text mode
delay (150);

  SIM900.write("AT+CSQ\r");             //Send command for signal report
  delay (200);

  while(SIM900.available() >0 )  { //wait for responce
    for(int i=0; i<44; i++) //read character into array from serial port
    Reply[i] = SIM900.read();
  }
 Reply[199] = '/0';

 
 
  temp1=Reply[31]-'0'; //convert relevant characters from array into integers
  temp2=Reply[32]-'0';
  temp3=Reply[33]-'0';
  temp4=Reply[34]-'0';
  
  if (temp1 == -48) { //if temp1 is -48 then do it again as the data is not valid (yet)
    GetStatus();
  }
    
  if (temp3 == -4) { // use temp3 to determine where in the array the relevent integers are (-4 is a ",")
    Signal= temp2+ temp1*10; // calculate signal if the first digit is a multiple of 10
    BER = temp4;
  }
  
  else{
   Signal= temp1; //calculate signal if the first digit is not a multiple of 10
   BER = temp3;
 }
 
  if ( Signal == 99) { // if our signal is 99, IE no signal condition , the return a signal of -1
    Signal = -1;
  }
   
 
  SignaldBm = Signal*2 -113; // calculate dBm for geeks like me.
  Serial.print ("Signal: "); //output stats to serial interface.
  Serial.println (Signal);
  Serial.print (SignaldBm);
  Serial.println ("dBm");
  Serial.print ("BER: ");
  Serial.println (BER);  
  
}


void loop() 
{
GetStatus (); 
}
