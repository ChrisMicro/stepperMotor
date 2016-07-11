/*
============================================================================
Name :        chStepperDriver.ino
Author :      ChrisMicro
Version :
Copyright :   GPL license 3
Date :        July 2016
Description : Stepper Motor Control with serial interface
============================================================================
/*
  Stepper motor driver with acceleration and deceleration.
  The timing is created with the DDS principle ( fractional accumulator ).
  Acceleration an decelaration is done with a state machine.

  Hardware
  MC:            Arduino Uno/Nano/Due
  Motor Driver:  Polulu A4988

 */

#define   DIRECTION_PIN      6
#define   STEPPER_PULSE_PIN  3

uint8_t   Direction = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("f: forward");
  Serial.println("b: backward");
  pinMode( DIRECTION_PIN,     OUTPUT );
  pinMode( STEPPER_PULSE_PIN, OUTPUT );

  digitalWrite(DIRECTION_PIN, Direction);
}

uint8_t Arduino_getchar()
{
  uint8_t c;

  while (!Serial.available())
  {

  }
  c = Serial.read();
  return c;
}

uint32_t NumberOfSteps = 100;

void loop()
{
  uint8_t command;

  command = Arduino_getchar();
  Serial.write(command);
  switch (command)
  {
    case 'f':
      {
        Direction = 0;
        digitalWrite(DIRECTION_PIN, Direction);

        steploop();

      } break;
    case 'b':
      {
        Direction = 1;
        digitalWrite(DIRECTION_PIN, Direction);

        steploop();
      } break;
    case 'n': // set the number of steps ( 8 bit )
      {
        NumberOfSteps = Arduino_getchar();

      } break;
    case 'm': // set the number of steps ( 16 bit )
      {
        uint8_t lowValue, highValue;
        lowValue = Arduino_getchar();
        highValue = Arduino_getchar();
        NumberOfSteps = (((uint32_t) highValue) << 8 ) + lowValue;

      } break;

  }
}

void steploop()
{
  uint32_t n;
  for (n = 0; n < NumberOfSteps; n++)
  {
    delay(10);
    digitalWrite(STEPPER_PULSE_PIN, HIGH);
    digitalWrite(STEPPER_PULSE_PIN, LOW);
  }
}

/* Matlab Ocatave script to control the motor
pkg load instrument-control

if exist("acm")
	SERIALPORTNAMEMOTOR     = ["/dev/ttyACM" int2str(acm)];
end
if exist("usb")
	SERIALPORTNAMEMOTOR     = ["/dev/ttyUSB" int2str(usb)];
end
if exist("com");
	SERIALPORTNAMEMOTOR=["\\\\.\\COM" int2str(com)];
end

BAUDRATE=115200 
if (exist("serial") == 3)
	printf("open %s defined in init.m\n\r",SERIALPORTNAMEMOTOR);
	printf("( if not working: try to open the serial port with a terminal and type space )\n\r\n\r");
else
    disp("Serial in octave driver: Unsupported")
endif

% Opens serial port  (config defaults to 8-N-1)
timeoutInSeconds=0.2;
SerialPortMotor = serial(SERIALPORTNAMEMOTOR, BAUDRATE,timeoutInSeconds*10);
SERIALPORTNAMEMOTOR

pause(0.1)

% Flush input and output buffers
srl_flush(SerialPortMotor); 

pause(2)

srl_write(SerialPortMotor,char(' '));
srl_write(SerialPortMotor,char(' '));

NumberOfSteps=50;
lowValue  = uint8(NumberOfSteps)
highValue = uint8(NumberOfSteps/256)
srl_write(SerialPortMotor,char('m'));
srl_write(SerialPortMotor,char(lowValue));
srl_write(SerialPortMotor,char(highValue));


totalNumberOfMeasurements=4;

if 1,
	for n=1:totalNumberOfMeasurements,
		value='b';

		srl_write(SerialPortMotor,char(value));
		pause(5);
	end
end
srl_close(SerialPortMotor);

*/
