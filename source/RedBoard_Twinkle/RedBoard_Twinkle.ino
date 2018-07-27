//Global Blink Variables
const int maxBrightness = 255;
const int LEDpin[6] = {3,5,6,9,10,11}; //This need to be PWM pins.
unsigned long BlinkStartTime[6] = {0,0,0,0,0,0};
unsigned long BlinkDuration[6] = {0,0,0,0,0,0};
byte BlinkStatus[6] = {0,0,0,0,0,0};
byte BlinkTarget[6] = {0,0,0,0,0,0};
byte BlinkDeviation = 10;

void setup() {
  randomSeed(analogRead(0));
  for ( int i=0; i < 6; i++) {
    pinMode(LEDpin[i], OUTPUT);
    BlinkTarget[i] = random(0,maxBrightness);
    analogWrite(LEDpin[i], BlinkTarget[i]);
  }
//  Serial.begin(9600);
//  Serial.println("---Start Program---");
}

void loop() {
  for (int i=0; i < 6; i++) {
    Twinkle3(i);
  }
}


//An irregular strobe look
// 1) Set Random Brightness
// 2) Wait a random time

void Twinkle1(int LED)
{
  if (millis() > (BlinkStartTime[LED] + BlinkDuration[LED])) {
    analogWrite(LEDpin[LED], random(0,maxBrightness));
    BlinkStartTime[LED] = millis();
    BlinkDuration[LED] = random(50,750);
  }
}

//A irregular pulse
// 1) Set Random Brightness Target. BlinkTarget[LED]
// 2) Set Random time to reach that target. TempDuration
// 3) Figure number of ms between each 1 PWM increment. BlinkDuraction[LED]
// 4) Wait that number of ms, change PMW by 1, repeat until Brightness Target is reached
void Twinkle2(int LED)
{
  if (millis() > (BlinkStartTime[LED] + BlinkDuration[LED])) {  
    if (BlinkStatus[LED] > BlinkTarget[LED])
    {
      BlinkStatus[LED] = BlinkStatus[LED] - 1;
    }
    
    if (BlinkStatus[LED] < BlinkTarget[LED])
    {
      BlinkStatus[LED] = BlinkStatus[LED] + 1;
    }
 
    if (BlinkStatus[LED] == BlinkTarget[LED])
    {
      BlinkTarget[LED] = random(0,maxBrightness);
      int TempDuration = random(0,3000);
      int TempSteps = abs(BlinkTarget[LED] - BlinkStatus[LED]);
      BlinkDuration[LED] = TempDuration/TempSteps;
    }

    analogWrite(LEDpin[LED], BlinkStatus[LED]);
    BlinkStartTime[LED] = millis();
  }
}

//An irregular pulse with a minor irregular strobe that creates a flicker affect
// 1) Set Random Brightness Target. BlinkTarget[LED]
// 2) Set Random time to reach that target. TempDuration
// 3) Figure number of ms between each 1 PWM increment. BlinkDuraction[LED]
// 4) Wait that number of ms, change current step by 1
// 5) offset PWM from current step by small random value
// 6) repeat until Brightness Target is reached
void Twinkle3(int LED)
{
  if (millis() > (BlinkStartTime[LED] + BlinkDuration[LED])) { //Time to make a step?
    if (BlinkStatus[LED] > BlinkTarget[LED]) //Going Down?
    {
      BlinkStatus[LED] = BlinkStatus[LED] - 1;
    }
    
    else if (BlinkStatus[LED] < BlinkTarget[LED]) //Going Up?
    {
      BlinkStatus[LED] = BlinkStatus[LED] + 1;
    }
 
    else if (BlinkStatus[LED] == BlinkTarget[LED]) //Destination reached?
    {
      BlinkTarget[LED] = random(BlinkDeviation,maxBrightness-BlinkDeviation); //New Target
      int TempDuration = random(0,3000); //New Time to get there
      int TempSteps = abs(BlinkTarget[LED] - BlinkStatus[LED]); // How many steps will it take?
      BlinkDuration[LED] = TempDuration/TempSteps; //How much time between each step?
    }

    //Add a minor random strobe to give it that twinkle
    if (random(2) == 0) //Heads we go up, tails we go down.
    {
      analogWrite(LEDpin[LED], BlinkStatus[LED]-random(BlinkDeviation));
    } else {
      analogWrite(LEDpin[LED], BlinkStatus[LED]+random(BlinkDeviation));
    }


    BlinkStartTime[LED] = millis();    //Reset the clock
  }
}

