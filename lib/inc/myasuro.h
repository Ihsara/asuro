/****************************************************************************/
/*!
  \file  myasuro.h

  \brief Benutzerspezische Definitionen der ASURO Bibliothek


  \par Tasterabfrage:

  Korrekturwert fuer Tasterabfrage in der PollSwitch Funktion.
  Der Wert wird durch Probieren ernmittelt.
  Die korrekte Funktion kann dann mit dem dem Tastertest Programm ermittelt werden.
  Als Wertebereich sollte man 61..65 durchprobieren, bis alle Taster korrekte Werte liefern.
  K1=32, K2=16, K3=8, K4=4, K5=2, K6=1

  \par Odometrie/Encoder:

  Korrekturwerte fuer Odometrie/Encoder in der SIG_ADC Interrupt Funktion.
  Die Werte k�nnen mit dem OdometrieTest Programm ermittelt werden.
  Dabei wird eine Messreihe f�r den linken und rechten Encoder angelegt.
  In einem Tabbelverarbeitungsprogramm (Excel o.�.) kann die Messreihe eingelesen
  und grafisch dargestellt werden.
  Die Schwellwerte f�r hell und dunkel k�nnen dann

  Hinweis:

  Beim Messen der Werte ist das ADLAR Bit des A/D Wandler auf 1 gesetzt.
  Das bedeutet, der A/D Wert wird rechts aligned in das ADH/ADL Register abgelegt.
  ADH enth�lt die obersten 8 Bit. ADL die untersten 8 Bit.
  F�r die Messung werden vom A/D Wandler Wert nur die 8 obersten Bits (ADH) ber�cksichtigt.
  Die beiden niederwertigsten Bits (ADL) entfallen.

  \par Go und Turn Funktion

  Korrekturwerte fuer die Go und Turn Funtion.
  Die Werte sind abhaengig von den verwendeten Encoder Scheiben (8 oder 12 Segmente)

  \version  V001 - 09.02.2007 - m.a.r.v.i.n\n
            Initiale Version
  \version  V002 - 18.02.2007 - Sternthaler\n
            Neuer Define\n
            MY_MOTOR_DIFF zum ausgleichen unterschiedlicher Motoren.
            V003 - 20.02.2007 - m.a.r.v.i.n\n
            Kommentare aus my struktur uebernommen
*****************************************************************************/
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   any later version.                                                    *
***************************************************************************/

#ifndef MYASURO_H
#define MYASURO_H

/* Tastaturabfrage */
/*! Faktor zur Berechnung der gedrueckten Tasten.\n
      Der Originalwert ist \b 61L und koennten im Bereich zwischen ca. 58L und
      65L schwanken. Dieser Wert gleicht Toleranzen der Wiederstaende an den
      Tastern aus.
*/
#define MY_SWITCH_VALUE           61L   /*!< Multiplikator fuer Tasterwerte */

/* Odometrie / Encoder */
/*! Wert, der in der Odometrie ueberschritten werden muss, um zum
    weiterzaehlen der Ticks in encoder[] zu fuehren bei aktivierter
    Automatik\n
    Die Originalwerte (links, rechts) sind \b 160.
    Diese Werte sind sehr stark vom Umgebungslicht abhaengig.
    Sie MUESSEN GROESSER als die Werte fuer MY_ODO_DARK_VALUE_L sein.
*/
#define MY_ODO_LIGHT_VALUE_L     160    /*!< Encoderschwellwert fuer Hell (linke Seite) */
/*! Wert, der in der Odometrie unterschritten werden muss, um zum
    weiterzaehlen der Ticks in encoder[] zu fuehren bei aktivierter
    Automatik\n
    Die Originalwerte (links, rechts) sind \b 140.
    Diese Werte sind sehr stark vom Umgebungslicht abhaengig.
    Sie MUESSEN KLEINER als die Werte fuer MY_ODO_LIGHT_VALUE_L sein.
*/
#define MY_ODO_DARK_VALUE_L      140    /*!< Encoderschwellwert fuer Dunkel (linke Seite) */
/*! Wert, der in der Odometrie ueberschritten werden muss, um zum
    weiterzaehlen der Ticks in encoder[] zu fuehren bei aktivierter
    Automatik\n
    Die Originalwerte (links, rechts) sind \b 160.
    Diese Werte sind sehr stark vom Umgebungslicht abhaengig.
    Sie MUESSEN GROESSER als die Werte fuer MY_ODO_DARK_VALUE_R sein.
*/
#define MY_ODO_LIGHT_VALUE_R     160    /*!< Encoderschwellwert fuer Hell (rechte Seite) */
/*! Wert, der in der Odometrie unterschritten werden muss, um zum
    weiterzaehlen der Ticks in encoder[] zu fuehren bei aktivierter
    Automatik\n
    Die Originalwerte (links, rechts) sind \b 140.
    Diese Werte sind sehr stark vom Umgebungslicht abhaengig.
    Sie MUESSEN KLEINER als die Werte fuer MY_ODO_LIGHT_VALUE_R sein.
*/
#define MY_ODO_DARK_VALUE_R      140    /*!< Encoderschwellwert fuer Dunkel (rechte Seite) */

/* Werte f�r 12 Segmente Encoder */
/*! Faktor zur Berechnung von Ticks um aus den in mm angegebenen Parameter
    umzurechnen.\n
    Der Originalwert ist \b 19363L und ist von der Anzahl der schwarz/weiss
    Teilstuecke auf den Odometriescheiben abhaengig.\n
    Der Originalwert wurde durch stochri ermittelt.
*/
#define MY_GO_ENC_COUNT_VALUE  19363L   /*!< GO Funktion, Divisor fuer Entfernung   */
/*! Faktor zur Berechnung von Ticks um aus den in Grad angegebenen Parameter
    umzurechnen.\n
    Der Originalwert ist \b 177L und ist von der Anzahl der schwarz/weiss
    Teilstuecke auf den Odometriescheiben abhaengig.\n
    Der Originalwert wurde durch stochri ermittelt.
*/
#define MY_TURN_ENC_COUNT_VALUE  177L   /*!< Turn Funktion, Mutiplikator fuer Winkel */

/* Werte zum ausgleichen unterschiedlicher Motoren */
/*! Differenzangabe zwischen den beiden Motoren.\n
    Der angegeben Wert verteilt sich je zur Haelte auf die Vorgaben fuer die\n
    Motorgeschwindigkeit.\n
    Bei einem \n positiven Wert, wird der \n rechte Motor \b verstaerkt.\n
    Bei einem \n negativen Wert, wird der \n linke Motor \b verstaerkt.
  */
#define MY_MOTOR_DIFF              0    /*!< 1/2 PLUS fuer Rechts, 1/2 MINUS fuer Links */

#endif /* MYASURO_H */
