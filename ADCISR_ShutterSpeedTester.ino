/*
 * Written by: vegetableswim 2020/01/28
 * Inspired by CAMERADACTYL Cameras https://youtu.be/UwOh3da_Y8s
 * I used TEPT4400, any other phototransistor would work similarly.
 * Issue: Threshold/MaxReadout ratio will affect result.
 */

volatile int Fired = 0;
volatile int Done = 0;
const int Threshold = 130; //Adjust according to your light source and lens
long Start;
long Stop;

void setup()
{
  Serial.begin(115200);
  
  ADMUX  = ((0<<REFS1)| // AVcc
            (1<<REFS0)| 
            (1<<ADLAR)| // ADC Left Adjust Result, read 8-bit ADCH only
            (0<< MUX4)| // (0)00111 = ADC7 = Arduino A0
            (0<< MUX3)| 
            (1<< MUX2)|
            (1<< MUX1)|
            (1<< MUX0));

  ADCSRB = ((1<<ADHSM)| // High Speed mode
            (0<< MUX5)| 
            (0<<ADTS3)| // Free Running mode
            (0<<ADTS2)| 
            (0<<ADTS1)|
            (0<<ADTS0));

  ADCSRA = ((1<< ADEN)| // ADC Enable
            (1<< ADSC)| // ADC Start Conversion 
            (1<<ADATE)| // ADC Auto Trigger Enable
            (1<< ADIE)| // ADC Interrupt Enable
            (0<<ADPS2)| // 011 = 8 prescaler, sampling rate 153 KHz
            (1<<ADPS1)|  
            (1<<ADPS0));
}

ISR(ADC_vect)
{
  //Serial.println(ADCH); //Uncommand when finding threshold, shutter set to bulb
  if((Done == 0) && (Fired == 0) && (ADCH > Threshold)) {
    Start = micros();
    Fired = 1;
  }
  if((Fired == 1) && (ADCH < Threshold)) {
    Stop = micros();
    Fired = 0;
    Done = 1;
  }
}
  
void loop()
{
  if(Done == 1){
    long Speed = (Stop - Start);      // make a variable called speed, which is the total number of microseconds that the shutter is open for
    Serial.print("Microseconds: ");
    Serial.println(Speed);               //display total microseconds in shutter interval
  
    float SS = (float)Speed/1000000;    // make a variable SS, which is how many seconds that the shutter open for
    float SS2 = 1/SS;                   // make a variable SS2, which is the inverse of the SS, or 1/ the shutter speed
    Serial.print("shutter speed: 1/");
    Serial.println(SS2);                //display the shutter speed
    Serial.println();
    
    Fired = 0;
    Start = 0;
    Done = 0;
  }

}
