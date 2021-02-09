int led = 19;

void setup() {
  delay(1000); // allow crystal to stabilise
  pinMode(led, OUTPUT);

  TCCR2B = 0;  //stop Timer 2
  TIMSK2 = 0;  // disable Timer 2 interrupts
  ASSR = (1<<AS2);  // select asynchronous operation of Timer2
  TCNT2 = 0;      // clear Timer 2 counter
  TCCR2A = 0;     //normal count up mode, no port output
  TCCR2B = (1<<CS22) | (1<<CS20);   // select prescaler 128 => 1 sec between each overflow

  while (ASSR & ((1<<TCN2UB)|(1<<TCR2BUB))); // wait for TCN2UB and TCR2BUB to clear

  TIFR2 = 0xFF;     // clear all interrupt flags
  TIMSK2 = (1<<TOIE2);  // enable Timer2 overflow interrupt
  sei(); // enable interrupts
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);          //Selecting power save mode as the sleep mode to be used
  sleep_enable();                     //Enabling sleep mode
}

void loop() {
  while(1) {
    sleep_mode();
  }
}

ISR(TIMER2_OVF_vect)
{
  digitalWrite(led, !digitalRead(led));
}
