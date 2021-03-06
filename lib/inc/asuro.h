/*!
 * \file asuro.h
 * \brief Definitionen und Funktionen der ASURO Bibliothek
 *
 * $Revision: 2.70 $
 * $Date: 07. Januar 2007 $
 * $Author: Jan Grewe, Robotrixer, Waste, Stochri, Andun, m.a.r.v.i.n $
 *

  \version  V001 - 10.02.2007 - m.a.r.v.i.n\n
            +++ my_t Neue Datenstruktur\n
            Datenstruktur fuer die Asuro-Hardwareabhaengigen Parameter die
            jeder User fuer seinen Asuro in der Datei myasuro.h selber
            einstellen kann um die Asuro-Typischen Eigenschaften zu definieren.
  \version  V002 - 18.02.2007 - Sternthaler\n
            +++ my_t\n
            - Neue Variable fuer MY_MOTOR_DIFF zum ausgleichen unterschiedlicher\n
              Motoren.
            - Aendern aller enthaltenen Variablen auf einen moeglichst kleinen\n
              Speicherbedarf. Notwendige Typumwandlungen muessen im Code durch\n
              casten erfolgen.
  \version  V003 - 20.02.2007 - m.a.r.v.i.n\n  
            +++ my_t Datenstruktur wieder entfernt\n
            Es werden direkt die Defines aus der myasuro.h verwendet.
  \version  V004 - 06.04.2007 - m.a.r.v.i.n\n
            Batterie und OdometrieData Funktionen umbenannt in 
            Battery und OdometryData.\n
            Alte Funktionsnamen ueber Defines beibehalten   
  \version  V005 - 07.06.2007 - Sternthaler\n
            Funktionen Go() und Turn() aus encoder.c zusammengefasst in
            GoTurn().\n
            Alte Funktionsnamen ueber Defines beibehalten   
  \version  V006 - 11.07.2007 - Sternthaler\n
            Externe Variable encoder als volatile angegeben, da sie im
            Interrupt SIGNAL (SIG_ADC) benutzt wird.
  \version  V007 - 15.11.2007 - m.a.r.v.i.n\n
            Variable switched als volatile definiert, da sie im Interrupt
            SIGNAL (SIG_INTERRUPT1) benutzt wird.
*****************************************************************************/
/*****************************************************************************
*                                                                            *
* it is not allowed to remove the nicknames of the contributers to this      *
* software from the function header                                          *
*                                                                            *
*****************************************************************************/
/*****************************************************************************
*                                                                            *
*   This program is free software; you can redistribute it and/or modify     *
*   it under the terms of the GNU General Public License as published by     *
*   the Free Software Foundation; either version 2 of the License, or        *
*   any later version.                                                       *
*                                                                            *
*****************************************************************************/



/****************************************************************************
*
* File Name:   asuro.c
*
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------
* 1.00     14.08.2003   Jan Grewe    build
* 2.00     14.10.2003   Jan Grewe        RIGHT_VEL -> MotorSpeed(unsigned char left_speed, unsigned char right_speed);
*                                        LeftRwd(),LeftFwd(),RightRwd(), LEFT_VEL,
*                                        RigthFwd() -> MotorDir(unsigned char left_dir, unsigned char right_dir);
*                                        GREEN_ON,GREEN_OFF,RED_ON,RED_OFF -> StatusLED(unsigned char color);
*                                        LED_RED_ON, LED_RED_OFF -> FrontLED(unsigned char color);
*                                        Blink(unsigned char left, unsigned char right) ->
*                                         BackLED(unsigned char left, unsigned char right);
*                                        Alles in Funktionen gefasst => leichter verstšndlich ?!?!
* 2.10     17.10.2003   Jan Grewe        new Timer funktion void Sleep(unsigned char time36kHz)
*
*
* Copyright (c) 2003 DLR Robotics & Mechatronics
*****************************************************************************/
/****************************************************************************
*
* File Name:   asuro.c
* Project  :   asuro library "Robotrixer Buxtehude"
*
* Description: This file contains additional functions:
*
* signal (SIG_ADC)                 interrupt/signal routine for encoder-counter
* signal (SIG_INTERRUPT1)          signal for switches
* EncoderInit()                    initializing encoder-counter
* EncoderStart()                   start autoencoding
* EncoderStop()                    stop autoencoding
* EncoderSet(int,int)              set encodervalue
* Msleep(int delay)                wait for delay in milliseconds
* Gettime()                        get systemtime in milliseconds
* PrintInt(int)
*
* modifications in Sleep, SIG_OUTPUT_COMPARE2, PollSwitch, LineData
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------
* beta1    31.03.2005   Robotrixer   asuro library
* -------  ----------   --------------   ------------------------------
* the encoder source is based on RechteckDemo.c ver 2.0 by Jan Grewe 22.10.2003
* Copyright (c) 2003 DLR Robotics & Mechatronics

*****************************************************************************/
/****************************************************************************
*
* File Name:   asuro.c
* Project  :   asuro library modified for IR collision detector
*
* Description: modifications made in following functions:
*
* SIGNAL (SIG_OUTPUT_COMPARE2)  ->  SIGNAL (SIG_OVERFLOW2)
* Gettime()       counts now 36kHz
* Init()        timer2 modified for adjustable duty cycle
* Batterie()        bug fixed
* Sleep()       counts now 36kHz
* Msleep()        counts now 36kHz
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------
* beta2    11.06.2005   Waste      asuro library
* -------  ----------   --------------   ------------------------------
*****************************************************************************/
/****************************************************************************
*
* File Name:   asuro.c
* Project  :   asuro library
*
* Description: This file contains additional functions:
*
* motor control functions 29.7.2005 stochri
* void Go(int distance)
* void Turn(int degree)
*
* unsigned char Wheelspeed[2]   measured Wheelspeed by interupt
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------------------
* sto1     29.07.2005   stochri    asuro library with motor control functions
* -------  ----------   --------------   ------------------------------------------
*****************************************************************************/
/****************************************************************************
*
* File Name:   asuro.c
* Project  :   asuro library "Robotrixer Buxtehude"
*
* Description: modifications made in following function:
*
* Go (int distance, int speed)           Added Speed and Odometrie
* Turn (int degree, int speed)           Added Speed and Odometrie
*
* modifications in Sleep, SIG_OUTPUT_COMPARE2, PollSwitch, LineData
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------
* And1     31.07.2005   Andun    See above
* -------  ----------   --------------   ------------------------------
*
*****************************************************************************/
/****************************************************************************
*
* File Name:   asuro.c
* Project  :   asuro library
*
* Description: modifications made in following functions:
*
* void PrintInt(int wert)
*
* unsigned char Wheelspeed[2]    removed
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------------------
* 2.60     28.09.2005   m.a.r.v.i.n      doxygen comments
* -------  ----------   --------------   ------------------------------------------
*****************************************************************************/
/****************************************************************************
*
* File Name:   asuro.c
* Project  :   asuro library
*
* Description: modifications made in following functions:
*
* SIGNAL (SIG_ADC)
* void PrintInt(int wert)
*
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------------------
* 2.61     20.11.2006   m.a.r.v.i.n      SIGNAL (SIG_ADC): static Variable toggle initialisiert
*                                        auf False (Bug report von Rolf_Ebert)
*                                        PrintInt: Initialisierung text String kann zu Fehler
*                                        beim Flashen mit RS232/IR Adapter fuehren
*                                        (Bug report von francesco)
* -------  ----------   --------------   ------------------------------------------
*****************************************************************************/
/****************************************************************************
*
* File Name:   asuro.c
* Project  :   asuro library
*
* Description: new functions has been added:
*
* void uart_putc(unsigned char zeichen)
* void SerPrint(unsigned char *data)
* void SetMotorPower(int8_t left_speed, int8_t right_speed )
* void sound(uint16_t freq, uint16_t duration_msec, uint8_t amplitude)
*
* Description: modifications made in following functions:
*
* void Go(int distance, int power)
* void Turn(int degree, int speed)
* void PrintInt(int wert)
*
*
* Ver.     Date         Author           Comments
* -------  ----------   --------------   ------------------------------------------
* 2.70     07.01.2007   stochri          new functions:
*                                        uart_putc: send single character
*                                        SerPrint: send 0-terminated string
*                                        SetMotorPower: set Motor speed and direction
*                                        sound: Sound Ausgabe ueber Motor PWM
*                                        Go: distance in mm
*                                        Turn: comments
*                       m.a.r.v.i.n      PrintInt: SerWrite ersetzt durch SerPrint
* -------  ----------   --------------   ------------------------------------------
*****************************************************************************/


#ifndef ASURO_H
#define ASURO_H

#include <avr/io.h>
#include <avr/interrupt.h>
#ifndef SIGNAL
  #include <avr/signal.h>   // header file obsolete in actual avr-libc
  #include <inttypes.h>
#endif
#include <stdlib.h>

#define  FALSE  0
#define  TRUE   1

#define  OFF    0
#define  ON     1

#define GREEN   1
#define RED     2
#define YELLOW  3

/* neue Funktionen und Variablen*/
#define LEFT    0
#define RIGHT   1
#define CENTER  2

/* Definitionen fuer Taster 
 * (Zaehlweise der Taster erfolgt von libks nachs rechts, 
 * wenn der Asuro mit den Tastern nach oben zeigt) */
#define K1 (1<<5)
#define K2 (1<<4)
#define K3 (1<<3)
#define K4 (1<<2)
#define K5 (1<<1)
#define K6 (1<<0)

/* --- Globale Variablen -----------------------------------*/
/*!
 * Asuro Lib Versions String
 */
extern const char version[5];

/*
 * Tastsensor Wert bei Interrupt Betrieb. 0=keine Taste, 1= Taste gedrueckt
 */
/*!
 * \~english
 * \brief any switch pressed?
 *
 * is being set to TRUE by interrupt if a switch is pressed\n
 * remember to set it to false again, since it is not reset automatically
 * \see StartSwitch
 */
extern volatile int switched;

/*
 * Odometriesensor Zaehler bei Interrupt Betrieb.
 * encoder[0] links, encoder[1] = rechts.
 */
/*!
 * \~english
 * \brief odometry sensors tick count
 * access: encoder[LEFT], encoder[RIGHT]\n
 * range: 0..255
 * \see EncoderInit
 */
extern volatile int encoder[2];

/*
 * Counter fuer 36kHz.
 * Wird in der Interrupt Funktion SIG_OVERFLOW2 hochgezaehlt\n
 * und in der Sleep() Funktion abgefragt.
 */
extern volatile unsigned char count36kHz;

/*
 * Sytemzeit in ms.
 * Wird in der Interrupt Funktion SIG_OVERFLOW2 hochgezaehlt
 * und in der Gettime() Funktion verwendet.
 */
extern volatile unsigned long timebase;

/*
 * Odometrie Sensor Abfrage im Interrupt Betrieb.
 * Wird in der Interrupt Funktion SIG_ADC abgefragt,
 * in der EncoderInit() und EncoderStart() Funktion gesetzt
 * und in der EncoderStop() Funktion geloescht.
 */
extern volatile int autoencode;

/* --- Funktions Prototypen -----------------------------------*/

/*!
 * \~english
 * initialises the hardware (ports, ADC, PWM)
 * this function has to be called by every program first
 */
void Init(void);

/**************** Zeit- und Delay Funktionen time.c *********/
/**
 * @brief return time since system start in ms
 * @return current system time in ms
 */
unsigned long Gettime(void);
/*!
 * \~english
 * \brief sleep function (ms)
 * \param ms time in ms
 */
void Msleep(int ms);
/*!
 * \~english
 * \brief sleep function (36Khz)
 * \param time36kHz time in 1/36k (sec)
 */
void Sleep(unsigned char time36kHz);

/**************** Low Level Encoder Funktionen encoder_low.c */
/*!
 * \~english
 * \brief initialise and starts odometry mesurement
 * uses interrupt(ADC_vect) 
 */
void EncoderInit(void);
/*!
 * \~english
 * \brief set odometry tick counter
 * \param setl left tick counter
 * \param setr left tick counter
 */
void EncoderSet(int setl,int setr);
/*!
 * \~english
 * \brief stop counting odometry sensor ticks
 */
void EncoderStop(void);
/*!
 * \~english
 * \brief stop counting odometry sensor ticks
 */
void EncoderStart(void);

/**************** Encoder Funktionen encoder.c **************/
/*!
 * \~english
 * \brief moves the robot forward/backward  OR  turns the robot using the
 *        odometry sensors.
 *        You can use the old function-names Go() and Turn()
 * \param distance travalling distance in cm. positive = forward, negative = backward
 *                 if distanse is zero, then the function will work as Turn.
 * \param degree degrees to turn. positive = turn right, negative = turn left. range: -360..360
 * \param speed motor speed. range: 0..255
 */
void GoTurn(int distance, int degree, int speed);
// aus Nostalgiegruenden Defines fuer alte Funktionsnamen 
#define Go(distance,speed) GoTurn(distance,0,speed)
#define Turn(degree,speed) GoTurn(0,degree,speed)

/**************** A/D Wandler Funktionen adc.c **************/
/*!
 * \~english
 * \brief returns the battery voltage
 * \return ADC value. range: 0..1023
 */
int Battery(void);
/*!
 * \~english
 * \brief reads out the phototransistors
 * \param data pointer to the data destination. access: data[LEFT], data[RIGHT]
 */
void LineData(unsigned int *data);
/*!
 * \~english
 * \brief reads out the odometry phototransistors
 * \param data pointer to the data destination. access: data[LEFT], data[RIGHT]
 */
void OdometryData(unsigned int *data);

// aus Nostalgiegruenden Defines fuer alte Funktionsnamen 
#define Batterie Battery 
#define OdometrieData OdometryData

/**************** LED Funktionen led.c **********************/
/*!
 * \~english
 * \brief controls the StatusLED
 * \param color values: OFF, GREEN, RED, YELLOW 
 */
inline void StatusLED(unsigned char color);
/*!
 * \~english
 * \brief controls the FrontLED
 * \param status values: ON, OFF
 */
inline void FrontLED(unsigned char status);
/*!
 * \~english
 * \brief values: ON, OFF
 * \param left leftLED status
 * \param right rightLED status
 */
void BackLED(unsigned char left, unsigned char right);

/**************** Low Level Motorsteuerungs Funktionen motor_low.c */
/*!
 * \~english 
 * \brief Motor configuration. values: FWD, RWD, BREAK, FREE
 * \param left left motor
 * \param right right motor
 */

inline void MotorDir(unsigned char left_dir, unsigned char right_dir);
/*!
 * \~english
 * \brief sets motor speed. range: 0..255
 * \param left_speed left motor
 * \param right_speed right motor
 */

inline void MotorSpeed(unsigned char left_speed, unsigned char right_speed);

/**************** Motorsteuerungs Funktionen motor.c **************/
void SetMotorPower(int8_t leftpwm, int8_t rightpwm);

/******************** Low Level UART Funktionen uart.c ************/
/*!
 * \~english
 * \brief Send Data to UART
 * \param data pointer to data
 * \param length data count
 */
void SerWrite(unsigned char *data,unsigned char length);
/*!
 * \~english
 * \brief Receive Data from UART
 * \param data pointer to data
 * \param length data count
 * \timeout timeout count, 0 meens blocking mode 
 */
void SerRead(unsigned char *data, unsigned char length, unsigned int timeout);

/**************** Print Funktionen serielle Ausgabe print.c ********/
void UartPutc(unsigned char zeichen);
void SerPrint(char *data);
void PrintInt(int wert);
void PrintLong(long wert);
/**************** Print Funktionen Floting Point printf.c ********/
void PrintFloat (float wert, char vorkomma, char nachkomma);

/**************** Taster Funktionen switches.c ******************/
/*!
 * \~english
 * \brief function to read out switches 
 * \return bitfield of switch value bit0 = K6, ... , bit5 = K1 
 */
unsigned char PollSwitch (void);
/*!
 * \~english
 * \brief binds switch press to external Interrupt 1
 * catch it with ISR(INT1_vect)
 * see avr-libc documentation for details
 */
void StartSwitch(void);
/*!
 * \~english
 * \brief stop sending switch interrupt
 * clears INT1_vect
 * \see StartSwitch
 */
void StopSwitch(void);

/**************** Soundausgabe sound.c **************************/
void Sound(uint16_t freq, uint16_t duration_msec, uint8_t amplitude);

/* ----------- END ------------ */


/* --------------- INTERNAL ------------- */
#define GREEN_LED_ON  PORTB |=  GREEN_LED /*!< Gruene Status LED an */
#define GREEN_LED_OFF PORTB &= ~GREEN_LED /*!< Gruene Status LED aus */
#define RED_LED_ON    PORTD |=  RED_LED   /*!< Rote Status LED an */
#define RED_LED_OFF   PORTD &= ~RED_LED   /*!< Rote Status LED aus */

#define FWD (1 << PB5)                    /*!< Motor vorwaerts */
#define RWD (1 << PB4)                    /*!< Motor rueckwaerts */
#define BREAK 0x00                        /*!< Motor bremsen */
#define FREE  (1 << PB4) | (1 << PB5)     /*!< Motor freilaufend */

#define IRTX        (1 << PB3)            /*!< PB3 Port fuer Infrarot Transmitter LED */
#define GREEN_LED   (1 << PB0)            /*!< PB0 Port fuer Gruene Status LED */
#define RED_LED     (1 << PD2)            /*!< PD2 Port fuer Rote Status LED */

#define PWM       (1 << PB1) | (1 << PB2) /*!< PB1, PB2 Ports fuer Pulsweitenmodulation der Motor Geschwindigkeit */
#define RIGHT_DIR (1 << PB4) | (1 << PB5) /*!< PB4, PB5 Ports fuer Drehrichtung rechter Motor */
#define LEFT_DIR  (1 << PD4) | (1 << PD5) /*!< PD4, PD5 Ports fuer Drehrichtung linker Motor */

#define SWITCHES   (1 << PD3)             /*!< PD3 Port fuer Tastsensor */
#define SWITCH_ON  PORTD |= SWITCHES      /*!< Tastsensor an  */
#define SWITCH_OFF PORTD &= ~SWITCHES     /*!< Tastsensor aus */

#define BATTERIE  (1 << MUX0) | (1 << MUX2)   /*!< ADC5 A/D Wandler Port fuer Batterie Abfrage */
#define SWITCH    (1 << MUX2)                 /*!< ADC4 A/D Wandler Port fuer Tastsensor */
#define IR_LEFT   (1 << MUX0) | (1 << MUX1)   /*!< ADC3 A/D Wandler Port fuer Linienfolger Fototransistor links */
#define IR_RIGHT  (1 << MUX1)                 /*!< ADC2  A/D Wandler Port fuer Linienfolger Fototransistor rechts */
#define FRONT_LED (1 << PD6)                  /*!< PD6 Port fuer Front LED */

#define ODOMETRIE_LED     (1 << PD7)              /*!< PD7 Port fuer  Odometrie LED */
#define ODOMETRIE_LED_ON  PORTD |= ODOMETRIE_LED  /*!< Odometrie LED an  */
#define ODOMETRIE_LED_OFF PORTD &= ~ODOMETRIE_LED /*!< Odometrie LED aus */

#define WHEEL_LEFT   (1 << MUX0)              /*!< ADC1 A/D Wandler Port fuer Odometrie Sensor links*/
#define WHEEL_RIGHT  0                        /*!< ADC0 A/D Wandler Port fuer Odometrie Sensor rechts */

#endif /* ASURO_H */
