#include <avr/sleep.h>
#define vibrationsstufen  6 //Stufen plus NULL
//EMW
const int pwmArray[vibrationsstufen+1] = {0,40,55,70,85,100,115};//Für Motor 775 mit 80 Watt = 3500-9000RPM bei 18V
//MESSWERTE: 0,2340,3252,3948,4386,4680,4956 RPM
//UMW
//const int pwmArray[vibrationsstufen+1] = {0,80,115,150,185,220,255};//Für UMW Motor mit 30 Watt  bei 12V

volatile byte vibrationsstufe = 0;

volatile unsigned long lastInterrupt;
#define debug   false
#define debug2   false
#define motor 1
#define led   0
//EMW
#define onOf 3
#define up  4
#define dwn 2
//UWM
//#define onOf 3
//#define up  2
//#define dwn 4

void Interrupt_config(){
  //Löschen des Global Interrupt Enable Bits (I) im Status Register (SREG)
  cli();
   //Setze des Pin Change Interrupt Enable Bit
  GIMSK |= (1 << PCIE);
  //Setzen des Pin Change Enable Mask für PB2 - PB4
  PCMSK |= (1 << PCINT2) | (1 << PCINT3) | (1 << PCINT4);
  //Setzen des Global Interrupt Enable Bits (I) im Status Register (SREG)
  sei();
  }
  
void setup()
{
  if( debug || debug2) {
  Serial.begin(9600);
  delay(500);
  Serial.println("Starte debugging....PWM ist aus");
  Serial.println(DDRB);
  Serial.println(PORTB);
  }
  if(! debug && ! debug2){//nicht während des debuggens
  DDRB =(1<<motor)|(1<<led);//Ausgabepins für PWM 0 und 1 
  digitalWrite(motor, LOW);
  digitalWrite(led, LOW);
  }
  
  //PB2 ist Eingang
  DDRB &= ~(1 << DDB2);
  //PB3 ist Eingang
  DDRB &= ~(1 << DDB3);
  //PB4 ist Eingang
  DDRB &= ~(1 << DDB4);
  //PB2-PB4 Pullup ist eingeschaltet
  PORTB |= (1 << PORTB2) | (1 << PORTB3) | (1 << PORTB4);
  
  Interrupt_config();
  
  if( debug ) {
  Serial.print("DDRB: ");
  Serial.println(DDRB, BIN);
  Serial.print("PORTB: ");
  Serial.println(PORTB, BIN);
  }
}


//Aufruf der Interrupt Serviceroutine
ISR(PCINT0_vect)
{ 
  cli(); // keine weiteren interrupts zulassen
  unsigned long prellzeit = lastInterrupt+250;//1/4 sekunde entprellen
  lastInterrupt = millis();//der aktuelle ist nun der letzte
  if(lastInterrupt < prellzeit) return;//entprellen
  
  if ( debug ) Serial.println("Interrupt");
  
  byte tastWertUp;
  byte tastWertDwn;
  byte tastWertOnOf;
  byte pwmFreq;
  
  tastWertUp = (PINB & (1 << up)) >> up; // 
  tastWertOnOf = (PINB & (1 << onOf)) >> onOf; // 
  tastWertDwn = (PINB & (1 << dwn)) >> dwn; // 
  
  if (tastWertOnOf == 0) //wenn schalter zu gound geschlossen
    {
      if ( debug ) Serial.println("Interrupt onOf");
      if(vibrationsstufe > 0) vibrationsstufe=0;
      else vibrationsstufe = 1;  
     }
    
  if (tastWertUp == 0) //wenn schalter zu gound geschlossen
    {
      // mehr wenn möglich
      if ( debug ) Serial.println("Interrupt up");
      if (vibrationsstufe < vibrationsstufen && vibrationsstufe > 0 ) 
                  vibrationsstufe++;
    }
  if (tastWertDwn == 0) //wenn schalter zu gound geschlossen
    {
      // weniger wenn möglich
      if ( debug ) Serial.println("Interrupt dwn");
      if (vibrationsstufe > 1) //
                  vibrationsstufe--;
    }
    
    if(! debug){
      if (vibrationsstufe == 0) {
        if(! debug2 ) digitalWrite(motor,LOW);
        if(! debug2 ) digitalWrite(led,LOW);
        if( debug2 ) Serial.println("Aus");
        }
     else if (vibrationsstufe == vibrationsstufen){
        if(! debug2 ) analogWrite(motor, pwmFreq);
        if(! debug2 ) digitalWrite(led,HIGH);
        if( debug2 ) Serial.println("Volle Fahrt");
        }    
     else {
        pwmFreq = pwmArray[vibrationsstufe];
        if(! debug2 ) analogWrite(motor, pwmFreq);
        if(! debug2 ) analogWrite(led, pwmFreq);
        if( debug2 ) Serial.print("PWM");
        if( debug2 ) Serial.println(pwmFreq);
        } 
  } 
  if ( debug ){
      Serial.print("Vibrationsstufe: ");
      Serial.print(vibrationsstufe);
      Serial.print(" pwm: ");
      Serial.println(pwmArray[vibrationsstufe]);
      } 
  sei();//Interrupts wieder freigeben
}

void goToSleep()
{
    byte adcsra;
    adcsra = ADCSRA; // save ADC control and status register A
    ADCSRA &= ~(1 << ADEN); // disable ADC
    MCUCR |= (1 << SM1) & ~(1 << SM0); // Sleep-Modus = Power Down
    MCUCR |= (1 << SE); // set sleep enable
    sei();//sicherheitshalber soll interrupt immer aktiv sein
    sleep_cpu(); // sleep
    MCUCR &= ~(1 << SE); // reset sleep enable
    ADCSRA = adcsra; // restore ADC control and status register A
}

void loop() {
  // put your main code here, to run repeatedly:
  if ( debug ) {
    Serial.print("Stufe: ");
    Serial.println(vibrationsstufe);
    delay(1000);
  }
 if(0 == vibrationsstufe) {
  delay(1000);
  goToSleep();
  }
 }
