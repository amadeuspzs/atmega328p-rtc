typedef struct{
  unsigned char second;
  unsigned char minute;
  unsigned char hour;
  unsigned char date;
  unsigned char month;
  unsigned int year;
} time;
  
time t;

char timestamp[14]; // YYYYMMDDHHMMSS from serial input

int led = 19;

void setup() {
  pinMode(led, OUTPUT);

  ASSR |= (1<<AS2); //set Timer/counter2 to be asynchronous from the CPU clock
  TCNT2 = 0; // reset Timer/counter2
  TCCR2B = (1<<CS00)|(1<<CS02); //Prescale the timer to be clock source/128
//  TCCR2B |= (1<<CS22); //Prescale the timer to be clock source/128
  TIMSK2 |= (1<<TOIE2); // enable overflow interrupt
  sei(); // enable interrupts
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);          //Selecting power save mode as the sleep mode to be used
  sleep_enable();                     //Enabling sleep mode
  Serial.begin(9600);
}

void loop() {
//  while(1) {
//    sleep_mode();
//    digitalWrite(led, HIGH);
//    delay(100);
//    digitalWrite(led, LOW);
  if (Serial.available() > 0) {
    // read the incoming byte:
    char incomingByte = Serial.read();

    if (incomingByte == 115) { // s(et) mode
      Serial.println("Enter timestamp: YYYYMMDDHHSS");

      while (Serial.available() < 14); // wait for serial input
      for (int i=0; i<14; i++) {
        timestamp[i] = Serial.read();
      }
      t.year = 1000 * (timestamp[0] - '0') + 100 * (timestamp[1] - '0') + 10 * (timestamp[2] - '0') + (timestamp[3] - '0');
      t.month = 10 * (timestamp[4] - '0') + (timestamp[5] - '0');
      t.date = 10 * (timestamp[6] - '0') + (timestamp[7] - '0');
      t.hour = 10 * (timestamp[8] - '0') + (timestamp[9] - '0');
      t.minute = 10 * (timestamp[10] - '0') + (timestamp[11] - '0');
      t.second = 10 * (timestamp[12] - '0') + (timestamp[13] - '0');
      TCNT2 = 0; // reset Timer/counter2
      printTimestamp();
    } else if (incomingByte == 114) { // r(ead) mode
      printTimestamp();
    } else {
      Serial.println("Command not recognised.\n\ns(et) or r(read)?");
    }
  } // end if serial available
  
//  }
}

void printTimestamp() {
  Serial.print(t.date);
  Serial.print(" ");
  Serial.print(t.month);
  Serial.print(" ");
  Serial.print(t.year);
  Serial.print(" ");
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.minute);
  Serial.print(":");
  Serial.println(t.second);  
} // end printTimestamp

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
