/*
============================================================================
Name :        chStepperDriver.ino
Author :      ChrisMicro
Version :
Copyright :   GPL license 3
Date :        June 2015
Description : Stepper motor driver with acceleration and deceleration using
              the DDS principle.
============================================================================
/*

  Stepper motor driver with acceleration and deceleration.
  The timing is created with the DDS principle ( fractional accumulator ).
  Acceleration an decelaration is done with a state machine.
  
  Hardware 
  MC:            Arduino Uno/Nano/Due
  Motor Driver:  Polulu A4988
  
 */
 
#define   DIRECTION_PIN      12 
#define   STEPPER_PULSE_PIN  13 

uint8_t   Direction=0;

void setup() {

  pinMode( DIRECTION_PIN,     OUTPUT );
  pinMode( STEPPER_PULSE_PIN, OUTPUT );
  
  digitalWrite(DIRECTION_PIN,Direction);

}

#define   MICROSTEPS       16

int32_t   SpeedNow         = 0;
int32_t   TargetSpeed      = 2000 * 65536*MICROSTEPS;
uint16_t  Acceleration     = 4000 * MICROSTEPS;
uint16_t  DDS_Accumulator  = 0;

#define   HOLDSPEED_STEPS   20000

#define   STATE_ACCELERATE  0
#define   STATE_HOLDSPEED   1
#define   STATE_DECELERATE  2

uint8_t   state             = STATE_ACCELERATE; // initial state

uint32_t  HoldSpeed_Counter = 0;


void loop() {

  DDS_Accumulator+=(SpeedNow>>16);
  digitalWrite(STEPPER_PULSE_PIN,DDS_Accumulator>>15);
  delayMicroseconds(10);

  switch(state)
  {
    case STATE_ACCELERATE:{
      SpeedNow+=Acceleration;
      if(SpeedNow>TargetSpeed){
        state=STATE_HOLDSPEED;
        HoldSpeed_Counter=0;
      }
    }break;

    case STATE_HOLDSPEED:{
      HoldSpeed_Counter++;
      if(HoldSpeed_Counter>HOLDSPEED_STEPS)
      {
        state=STATE_DECELERATE;
      }
    }break;

    case STATE_DECELERATE:{
       SpeedNow-=Acceleration;
       if(SpeedNow<0)
       {
         SpeedNow=0;
         state=STATE_ACCELERATE;
         Direction^=1; // reverse direction
         digitalWrite(DIRECTION_PIN,Direction);
       }
    }break;

    default:{
        state=STATE_ACCELERATE;
    }
  }

} 
