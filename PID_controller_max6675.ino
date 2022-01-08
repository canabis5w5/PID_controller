#include <Thermocouple.h>
#include <MAX6675_Thermocouple.h>
#include <EEPROM.h>


#define SCK_PIN 10
#define CS_PIN  9
#define SO_PIN  8

#define SCK_PIN2 6
#define CS_PIN2  5
#define SO_PIN2  4

Thermocouple* thermocouple;
Thermocouple* thermocouple2;


// pin RELAY
#define PIN_RELAY 7

// температура страбатывания реле
int T_State = 0;
int T_State1 = 80;


void setup() 
{
  Serial.begin(57600);

  thermocouple = new MAX6675_Thermocouple(SCK_PIN, CS_PIN, SO_PIN);
  thermocouple2 = new MAX6675_Thermocouple(SCK_PIN2, CS_PIN2, SO_PIN2);

  // Relay
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, HIGH);

  T_State = EEPROM.read(1);
}


void loop() 
{

  //const double Tc0 , Tc1 ;
  
  const double Tc0 = thermocouple->readCelsius();
  delay(100);
  const double Tc1 = thermocouple2->readCelsius();
  delay(100);

  String data;
  while (Serial.available())
  {
       char temp=Serial.read();
       data += temp;
   }
  data = " ";
  
  if ( data.indexOf ( "<State:>" ))  // For example: "State: 80"
    {
      int T_port = Serial.parseInt();
      if ( T_port )
      {
        T_State = T_port;
        EEPROM.write( 1, T_port );
      }
    }

  // Reaction relay
  if ((int)Tc0 < T_State) digitalWrite(PIN_RELAY, HIGH);
  else digitalWrite(PIN_RELAY, LOW);


  //push data in serial port
  Serial.print("State:      Hold: ");
  Serial.println(Tc0);
  Serial.println(T_State);
  Serial.print("State1:     Temp1: ");
  Serial.println(Tc1);
  Serial.println(T_State1);
  Serial.print("========================= \n");
  delay(500);
}
