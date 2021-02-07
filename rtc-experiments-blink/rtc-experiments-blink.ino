typedef struct{
  unsigned char second;
  unsigned char minute;
  unsigned char hour;
  unsigned char date;
  unsigned char month;
  unsigned char year;
} time;
  
time t;
  
int led = 19;

void setup() {
  pinMode(led, OUTPUT);

  ASSR |= (1<<AS2); //set Timer/counter2 to be asynchronous from the CPU clock
  TCNT2 = 0; // reset Timer/counter2
  TCCR2B = (1<<CS22)|(1<<CS20); //Prescale the timer to be clock source/128
  TIMSK2 |= (1<<TOIE2); // enable overflow interrupt
  sei(); // enable interrupts
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);          //Selecting power save mode as the sleep mode to be used
  sleep_enable();                     //Enabling sleep mode
}

void loop() {
  while(1) {
    sleep_mode();
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
  }
}

ISR(TIMER2_OVF_vect)
{
  if (++t.second==60)        //keep track of time, date, month, and year
  {
    t.second=0;
    if (++t.minute==60)
    {
      t.minute=0;
      if (++t.hour==24)
      {
        t.hour=0;
        if (++t.date==32)
        {
          t.month++;
          t.date=1;
        }
        else if (t.date==31)
        {
          if ((t.month==4) || (t.month==6) || (t.month==9) || (t.month==11))
          {
            t.month++;
            t.date=1;
          }
        }
        else if (t.date==30)
        {
          if(t.month==2)
          {
            t.month++;
            t.date=1;
          }
        }
        else if (t.date==29)
        {
          if((t.month==2) && (not_leap()))
          {
            t.month++;
            t.date=1;
          }
        }
        if (t.month==13)
        {
          t.month=1;
          t.year++;
        }
      }
    }
  }

}

static char not_leap(void)      //check for leap year
{
  if (!(t.year%100))
  {
    return (char)(t.year%400);
  }
  else
  {
    return (char)(t.year%4);
  }
}
