#include <Wire.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

unsigned long fix_age;

SoftwareSerial GSM(4,2);

TinyGPS gps;
void gpsdump(TinyGPS &gps);
bool feedgps();
void getGPS();
long lat, lon;
float LAT, LON;
long measurement;

char inchar; // Will hold the incoming character from the GSM shield


int buz = 9;
int stop = 0;
int ir = 3;
int alcohol = 7;
int eye=6;


const char *phone_no[] = {
  "+916305427492",
  "+916305427492"
  
  };



void setup(){   

pinMode(ir, INPUT);
pinMode(alcohol, INPUT);
pinMode(eye, INPUT);

Serial.begin(9600);

Wire.begin();


pinMode(buz, OUTPUT);
 
 GSM.begin(9600);
 Serial.begin(9600);
 Serial.println("Initializing....");
 initModule("AT","OK",1000);
 initModule("ATE1","OK",1000);
 initModule("AT+CPIN?","READY",1000);  
 initModule("AT+CMGF=1","OK",1000);     
 initModule("AT+CNMI=2,2,0,0,0","OK",1000);  
 Serial.println("Initialized Successfully");
   
  
 GSM.print("AT+CMGS=\"");GSM.print(phone_no[1]);GSM.println("\"\r\n");
 delay(1000);
 GSM.println("Welcome to Arduino Based Vehicle Accident Alert System using GPS, GSM, Shock Sensor.");
 delay(300);
 GSM.write(byte(26));
 delay(3000); 
 getGPS(); 
}

void loop(){
long measurement = digitalRead(ir);

long eye_status = digitalRead(eye);

long alcohol_status = digitalRead(alcohol);

delay(1000);
Serial.println(measurement);


  if (eye_status == 0) {
    Serial.println("Eyes r Close");
    digitalWrite(buz, HIGH); 
  } 
  else{
     digitalWrite(buz, LOW);
    }

 if (alcohol_status == 0) {
    Serial.println("Alcohol Detected");
              SendSMS("Alcohol Detected :: while driving !!! ");
 }

if(measurement == 0){ 
sms();
Serial.println("SMS SENT SUCCESSFULLY");
}


 if(GSM.available() >0){inchar=GSM.read();
 if(inchar=='R'){inchar=GSM.read(); 
 if(inchar=='I'){inchar=GSM.read();
 if(inchar=='N'){inchar=GSM.read();
 if(inchar=='G'){  
 GSM.print("ATH\r");
 delay(1000);
          
 getGPS();     
 GSM.print("AT+CMGS=\"");GSM.print(phone_no[1]);GSM.println("\"\r\n");
 delay(1000);
 GSM.println("RING Reply");
 GSM.print("http://maps.google.com/?q=loc:");
 GSM.print(LAT/1000000,7);
 GSM.print(",");
 GSM.println(LON/1000000,7);
 delay(300);
 GSM.write(byte(26));
 delay(5000);
   }
  }
 }
 }
 
}


  long lat, lon;
  unsigned long fix_age, time, date, speed, course;
  unsigned long chars;
  unsigned short sentences, failed_checksum;
  // retrieves +/- lat/long in 1000000ths of a degree
  gps.get_position(&lat, &lon, &fix_age);

}

long vibration(){
  long M = pulseIn(ir,HIGH);
  return M;
  
  
  }

 void SendSMS(String msg) {
  GSM.println("AT+CMGS=\"+916305427492\"\r"); 
  delay(1000);
  GSM.println(msg);
  delay(200);
  GSM.println((char)26);
  delay(1000); 
}


void sms(){
getGPS();     
GSM.print("AT+CMGS=\"");GSM.print(phone_no[1]);GSM.println("\"\r\n");
delay(1000);
GSM.println("Emergency.Please reach out to this location right away...");
GSM.print("http://maps.google.com/?q=loc:");
GSM.print(LAT/1000000,7);
GSM.print(",");
GSM.println(LON/1000000,7);
delay(300);
GSM.write(byte(26));
delay(5000);  
}



void getGPS(){
bool newdata = false;
unsigned long start = millis();
// Every 1 seconds we print an update
while (millis() - start < 1000){
    if (feedgps ()){
      newdata = true;
    }
  }
if (newdata){
    gpsdump(gps);
  }
}

bool feedgps(){
while (Serial.available()){
    if (gps.encode(Serial.read()))
      return true;
  }
  return 0;
}

void gpsdump(TinyGPS &gps){
  //byte month, day, hour, minute, second, hundredths;
  gps.get_position(&lat, &lon);
  LAT = lat;
  LON = lon;
  {
    feedgps(); // If we don't feed the gps during this long routine, we may drop characters and get checksum errors
  }
}

void initModule(String cmd, char *res, int t){
while(1){
    Serial.println(cmd);
    GSM.println(cmd);
    delay(100);
    while(GSM.available()>0){
       if(GSM.find(res)){
        Serial.println(res);
        delay(t);
        return;
       }else{Serial.println("Error");}}
    delay(t);
  }
}
