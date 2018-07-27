#include "SparkFun_Tlc5940.h"

//Globals

//Changing these could require major changes to code.
const int maxBrightness = 4095; //It's better to reduce brightness by lowering voltage, than to do it in code
const int NumLEDs = 16 * NUM_TLCS; //NUM_TLCS is defined in the tlc_config.h included in the appropriate TLC5940 library

//The led's have a subtle flicker. These values affect that
const int BlinkDeviation = 100; //Base Max PMW steps that we'll deviate from current brightness
const int BlinkDeviationEnhancer = 10; //This will be mathed with the current brightness to modify BlinkDeviation

//State Containers
unsigned long BlinkStartTime[NumLEDs];
unsigned long BlinkDuration[NumLEDs];
int BlinkStatus[NumLEDs];
int BlinkTarget[NumLEDs];

void setup() {
  randomSeed(analogRead(0));
  for ( int i=0; i < NumLEDs; i++) { //stuff the State Containers with starting values
    BlinkTarget[i] = random(0,maxBrightness);
    BlinkStartTime[i] = 0;
    BlinkDuration[i] = 0;
    BlinkStatus[i] = 0;
  }

  // Because the Duration = 0; as soon as it is checked it will set a new target,Duration,etc. etc.
  
  Tlc.init(maxBrightness); // Set LED all the way up for testing
  while (millis() < 10000){ // Maintain test state
    Tlc.update();
  }
}

// Cycle through each LED
// Push updates
void loop() {
  for (int i=0; i < NumLEDs; i++) {
    Twinkle(i);
  }
  Tlc.update();
}

//An irregular pulse with a minor irregular strobe that creates a flicker affect
// 1) Set Random Brightness Target. BlinkTarget[LED]
// 2) Set Random time to reach that target. TempDuration
// 3) Figure number of ms between each 1 PWM increment. BlinkDuraction[LED]
// 4) Wait that number of ms, change current step by 1
// 5) offset PWM from current step by small random value
// 6) repeat until Brightness Target is reached
void Twinkle(int LED)
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
      BlinkTarget[LED] = random((BlinkDeviation+1),maxBrightness-(BlinkDeviation+(maxBrightness/BlinkDeviationEnhancer))); //New Target.
      int TempDuration = random(500,3000); //New Time to get there
      int TempSteps = abs(BlinkTarget[LED] - BlinkStatus[LED]); // How many steps will it take?
      BlinkDuration[LED] = TempDuration/TempSteps; //How much time between each step?
    }

    //Add a minor random strobe to give it that twinkle
    if (random(2) == 0) //Heads we go up, tails we go down.
    {
      Tlc.set(LED, BlinkStatus[LED]-RandomDeviation(BlinkStatus[LED]));
    } else {
      Tlc.set(LED, BlinkStatus[LED]+RandomDeviation(BlinkStatus[LED]));
    }


    BlinkStartTime[LED] = millis();    //Reset the clock
  }
}

//To add a bit of sparkle, we wander around the actual status. This calculates that deviation
int RandomDeviation(int BaseBrightness) {
  return random(BlinkDeviation+(BaseBrightness/BlinkDeviationEnhancer));
}
