#define batteryCutoffVoltage 11
#define solarCutoffVoltageMin 45
#define solarCutoffVoltageMax 65

#define solarOKLed 9
#define batteryOKLed 8
#define outputOKLed 7
#define chargeLed 6
#define outputEnabledLed 5
#define errorLed 4

#define outputSwitch 3
#define chargeRelay 11
#define outputRelay 13
#define solarRelay 12
//vorderstes


//TODO
//Spannungsteiler für Regler bauen

struct Status {
  bool solarAvailable;
  bool batteryAvailable;
  double batteryVoltage;
  double solarVoltage;
  bool regVoltageOK;
  bool error;
};

Status systemStatus={false,false,0,0,true,false};
void setup() {
  // put your setup code here, to run once:
  pinMode(chargeRelay, OUTPUT);
  pinMode(outputRelay, OUTPUT);
  pinMode(solarRelay, OUTPUT);

  pinMode(outputSwitch, INPUT_PULLUP);
  pinMode(solarOKLed, OUTPUT);
  pinMode(batteryOKLed, OUTPUT);
  pinMode(outputOKLed, OUTPUT);
  pinMode(chargeLed, OUTPUT);
  pinMode(outputEnabledLed, OUTPUT);
  pinMode(errorLed, OUTPUT);

  Serial.begin(9600);
  systemStatus.batteryVoltage=0.0;
  systemStatus.solarVoltage=0.0;
  
}

void loop() {
  if(systemStatus.error!=true){
    checkStatesAndSetRelays();
  }
}

void getVoltage(){

  //TODO: Enter real data
  double voltageRead;
  voltageRead=analogRead(0)*0.0049;
  Serial.print("Gelesen: ");
  Serial.print(voltageRead);
  systemStatus.batteryVoltage=voltageRead/0.19;
  Serial.print("Gerechnet: ");
  Serial.print(systemStatus.batteryVoltage);
  delay(1000);
  

  if(systemStatus.solarVoltage>solarCutoffVoltageMin && systemStatus.solarVoltage<solarCutoffVoltageMax){
    systemStatus.solarAvailable=true;
  }else{
    systemStatus.solarAvailable=false;
  }

  if(systemStatus.batteryVoltage>batteryCutoffVoltage){
    systemStatus.batteryAvailable=true;
  }else{
    systemStatus.batteryAvailable=false;
  }

  //TODO: Hier muss noch code für die Reg Spannung hin..
 
}

void checkStatesAndSetRelays(){

  getVoltage();
  if(systemStatus.solarAvailable==true){
    digitalWrite(solarOKLed,HIGH);
    digitalWrite(solarRelay,HIGH);
  }else{
    digitalWrite(solarOKLed,LOW);
    digitalWrite(solarRelay,LOW);
  }
  getVoltage();
  if(systemStatus.regVoltageOK==true && systemStatus.solarAvailable==true ){
    digitalWrite(chargeLed,HIGH);
    digitalWrite(chargeRelay,HIGH);
  }else{
    digitalWrite(chargeLed,LOW);
    digitalWrite(chargeRelay,LOW);
  }
  getVoltage();
  if(systemStatus.batteryAvailable==true){
    digitalWrite(batteryOKLed,HIGH);
  }else{
    digitalWrite(batteryOKLed,LOW);
  }
  if(systemStatus.regVoltageOK==true){
    digitalWrite(outputOKLed,HIGH);
  }else{
    digitalWrite(outputOKLed,LOW);
  }
  if((systemStatus.batteryAvailable==true || systemStatus.regVoltageOK==true) && digitalRead(outputSwitch)==LOW){
    digitalWrite(outputEnabledLed,HIGH);
    digitalWrite(outputRelay,HIGH);
  }else{
    digitalWrite(outputEnabledLed,LOW);
    digitalWrite(outputRelay,LOW);
  }
  getVoltage();

  //TODO: activate
  /*
  if((systemStatus.regVoltageOK==false && systemStatus.solarAvailable==true) || (systemStatus.batteryAvailable==false && systemStatus.solarAvailable==false)){
    digitalWrite(solarRelay,LOW);
    digitalWrite(chargeRelay,LOW);
    digitalWrite(outputRelay,LOW);
    digitalWrite(errorLed,HIGH);
    systemStatus.error=true;
  }
  */
}
