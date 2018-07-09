/*******************************************************************************
*
* File Name:   SelfTest.c
* Project  :   SelfTest
*
* Description:
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------
* 1.00	   14.08.2003   Jan Grewe		 build
* 2.00     22.10.2003   Jan Grewe        angepasst auf asuro.c Ver.2.10
*
* Copyright (c) 2003 DLR Robotics & Mechatronics
*****************************************************************************/
/*****************************************************************************/
/*!
  /file 	SelfTest.c
  /brief 	SelfTest Routinen fuer den Asuro
  /author Jan Grewe, m.a.r.v.i.n

  /version      V001 - 14.08.2003 - Jan Grewe\n
                Initial Release
  /version      V002 - 22.10.2003   Jan Grewe\n
                angepasst auf asuro.c Ver.2.10
  /version      V003 - 18.08.2007 - m.a.r.v.i.n\n
                SerPrint verwenden anstelle von SerWrite
*/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   any later version.                                                    *
 ***************************************************************************/
#include "asuro.h"
#include "test.h"

void OptionsMsgs(void);
void ActionFromMsgs(char msg[3]);
void LEDOptionsMsgs(void);

void Sleep_more(int i);//Sleep more than 1sec, i is in secs
void SelfTest (void)
{
  unsigned int i;
  char dataMsg[1] ;
  Init();
  {
    StatusLED(GREEN);

    SerPrint("\n\r-- ASURO Manual Testing --\n\r");


    while( dataMsg != '0')
    {
	  *dataMsg = 'ini';
      SerPrint("\n\r I'm done my selftest. Please give me some instructions!!\r\n");
      OptionsMsgs();
      SerRead(dataMsg,1,0);
      SerPrint("\n\r Received : ");
      SerPrint(dataMsg);
      ActionFromMsgs(dataMsg);
    }

  }
}

void Sleep_more(int i){
  int millis, j;
  millis = i*1000;
  for (j = 0; j <= (millis/3); j++ ){
    Sleep(216);
  }
}

void ActionFromMsgs(char msg[1]){
  char dataMsg[1];
  int switches;
  unsigned int index,i;
  int pollRef[7]= {0,32,16,8,4,2,1};


  if (msg[0] == '1'){
	SerPrint("\n\rRun motor: FWD, FWD at 120\r\n");
    MotorDir(FWD, FWD);
    MotorSpeed( 120	, 120);
    Sleep_more(3);
    MotorSpeed(0,0);
  }
  else if ( msg[0] == '2'){
    SerPrint("\n\r LED function \r\n");
    LEDOptionsMsgs();
    SerRead(dataMsg,1,0);
    if ( dataMsg[0] == '1' ){
      SerPrint("Red StatusLED turn on\r\n");
      StatusLED(RED);
      Sleep_more(3);
      StatusLED(OFF);
    }
    else if( dataMsg[0] == '2'){
      SerPrint("Green StatusLED turn on\r\n");
      StatusLED(GREEN);
      Sleep_more(3);
      StatusLED(OFF);
    }
    else if (dataMsg[0] == '3'){
      SerPrint("Back LEDs turn on\r\n");
      BackLED(ON, ON);
      Sleep_more(3);
      BackLED(OFF, OFF);
    }
  }
  else if (msg[0] == '3'){
    SerPrint("\n\rTesting buttons. Press '9' to escape.\r\n");
    Sleep(216);
    while (msg[0] != '9'){
      Sleep(216);
      switches = PollSwitch();
      if (switches >= 64){
        SerPrint("\n\rSomething is wrong!\r\n");
      }
      for (index = 1; index<= 6; index++){
        if (switches >= pollRef[index]){
          SerPrint("\n\r ----->  Switch ");
          SerPrint(index);
          SerPrint("was pressed!\r\n");
          switches -= pollRef[index];
        }
      }
	  SerRead(msg, 1, 0); 
    }
  }
  else if (msg[0]== '4'){
    SerPrint("\n\r-- ASURO Testing --\n\r");

    SerPrint("LED Test\n\r");
    LEDTest();

    SerPrint("Line Test\n\r");
    StatusLED(GREEN);
    for (i = 0; i < 0xFE00; i++)
      LineTest();
    StatusLED(OFF);

    SerPrint("Switch Test\n\r");
    for (i = 0; i < 0xFE00; i++)
      SwitchTest();
    Init();
    StatusLED(OFF);

    SerPrint("Odometrie Test\n\r");
    FrontLED(ON);
    for (i = 0; i < 0xFFF0; i++)
      OdometrieTest();
    StatusLED(OFF);
    FrontLED(OFF);

    SerPrint("Motor Test\n\r");
    BackLED(ON,ON);
    MotorTest();
    BackLED(OFF,OFF);

    SerPrint("Serial Test\n\r");
    SerialTest();
  }
}

void LEDOptionsMsgs( void){
  SerPrint("\n\r-- LED manual test --\r\n");
  SerPrint("1/ Red LED \r\n");
  SerPrint("2/ Green LED \r\n");
  SerPrint("3/ Back LEDs turn on \r\n");
}

void OptionsMsgs(void){
  SerPrint("1/ Run motor\r\n");
  SerPrint("2/ LEDs functions\r\n");
  SerPrint("3/ Buttons \r\n");
  SerPrint("4/ Run Selftest \r\n");
}

