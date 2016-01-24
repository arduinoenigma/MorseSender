#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <avr/eeprom.h>

#define PIN 13
#define PINON 1
#define PINOFF 0

void LightOn()
{
  digitalWrite(PIN, PINON);
}

void LightOff()
{
  digitalWrite(PIN, PINOFF);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(PIN, OUTPUT);
  LightOff();

  //TestMorse();
}

void loop() {
  // put your main code here, to run repeatedly:

  // is something comes in, put it through the state machine
  if (Serial.available())
  {
    char inKey = Serial.read();
    // the state machine will queue out the last character in this particular string:
    // Q>Stecker>Q>ETW>Q>R1>V>R2>Y>R3>C>R4>Y>UKW>G>R4>U>R3>R>R2>G>R1>C>ETW>C>Stecker>C
    // The minimal test string to flash X is er>er>X
    Analyze(inKey);
  }

  // if there is anything queued to send out, send it, call often
  SendMorse();
}

/*
state machine
detect new line, wait for er> twice, then output next character, new line at any time returns to initial state:
this state machine will call QueueMorse(inKey) to put the desired character in a queue of morse to be sent out
the minimal test string to flash X is er>er>X

Q>Stecker>Q>ETW>Q>R1>V>R2>Y>R3>C>R4>Y>UKW>G>R4>U>R3>R>R2>G>R1>C>ETW>C>Stecker>C
W>Stecker>W>ETW>W>R1>O>R2>M>R3>O>R4>D>UKW>Q>R4>M>R3>C>R2>P>R1>G>ETW>G>Stecker>G
E>Stecker>E>ETW>E>R1>M>R2>W>R3>B>R4>E>UKW>A>R4>R>R3>X>R2>I>R1>C>ETW>C>Stecker>C
A>Stecker>A>ETW>A>R1>N>R2>T>R3>P>UKW>U>R3>R>R2>G>R1>O>ETW>O>Stecker>O
S>Stecker>S>ETW>S>R1>W>R2>F>R3>G>UKW>Y>R3>O>R2>Y>R1>T>ETW>T>Stecker>T
D>Stecker>D>ETW>D>R1>S>R2>Z>R3>J>UKW>B>R3>W>R2>M>R1>C>ETW>C>Stecker>C

A>Stecker>A>ETW>A>R1>A>R2>A>R3>A>R4>A>UKW>A>R4>A>R3>A>R2>A>R1>A>ETW>A>Stecker>B
A>Stecker>A>ETW>A>R1>A>R2>A>R3>A>UKW>A>R3>A>R2>A>R1>A>ETW>A>Stecker>Z
*/

enum verboseStateMachine {
  NL,
  E,
  R,
  GT,
  DONE
};

void Analyze(char inKey)
{
  static verboseStateMachine VerboseSM = NL;
  static byte SteckerCounter = 0;
  bool reset = false;

  if (inKey == 13)
  {
    SteckerCounter = 0;
    reset = true;
  }

  switch (VerboseSM)
  {
    case NL:
      if (inKey == 'e')
      {
        VerboseSM = E;
      }
      break;

    case E:
      if (inKey == 'r')
      {
        VerboseSM = R;
      }
      else
      {
        reset = true;
      }
      break;

    case R:
      if (inKey == '>')
      {
        if (SteckerCounter == 0)
        {
          SteckerCounter++;
          reset = true; // found first er>, go back and look for it again
        }
        else
        {
          // found er> for a second time, output next character if not newline
          VerboseSM = GT;
        }
      }
      else
      {
        reset = true;
      }
      break;

    case GT:
      QueueMorse(inKey);
      VerboseSM = DONE;
      break;

    case DONE:
      // stay here until new line is detected
      break;
  }

  if (reset)
  {
    VerboseSM = NL;
  }
}
