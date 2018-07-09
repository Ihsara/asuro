/****************************************************************************/
/*!
  \file     encoder_low.c

  \brief    Low Level Funktionen für Radencoder und Odometrie.

  \see      Defines zum setzen von Port's und Konfigurationen in asuro.h\n
            TRUE, FALSE, LEFT, RIGHT

  \version  V--- -
            KEINE original Version von der ASURO CD vom DLR\n
  \version  V--- - bis zum 07.01.2007 - \n
            Bitte in Datei CHANGELOG nachsehen.\n
            Das Grundgeruest dieser Funktionen wurde von stochri erstellt.\n
  \version  V001 - 13.01.2007 - m.a.r.v.i.n\n
            +++ Alle Funktionen\n
            Zerlegte Sourcen in einzelne Dateien fuer eine echte Library.
  \version  V002 - 27.01.2007 - Sternthaler\n
            +++ Alle Funktionen\n
            Kommentierte Version (KEINE Funktionsaenderung)
  \version  V003 - 21.02.2007 - Sternthaler\n
            +++ EncoderStart()\n
            KEINE Funktionsaenderung\n
            Die Fehlerbeschreibung von Sternthaler ersatzlos gestrichen, da\n
            die Funktionalitaet von stochri durch das Starten des AD-Wandlers\n
            in EncoderInit() im sogenannten 'free running'-Mode gegeben ist.
*****************************************************************************/
/*****************************************************************************
*                                                                            *
*   This program is free software; you can redistribute it and/or modify     *
*   it under the terms of the GNU General Public License as published by     *
*   the Free Software Foundation; either version 2 of the License, or        *
*   any later version.                                                       *
*                                                                            *
*****************************************************************************/
#include "asuro.h"



/****************************************************************************/
/*!
  \brief
  Den Interrupt Betrieb der Odometriesensoren-Messung initialisieren und starten.

  \param
  keine

  \return
  nichts

  \see
  autoencode, encoder

  \par  Funktionsweise:
  Schaltet die Back-LED's aus und die Odometrie-LED's ein.\n
  Initialisiert den AD-Wandler und startet ihn fuer eine Messung des linken\n
  Rad-Sensors.\n
  Wichtig ist nun das setzen der globalen Variablen autoencode auf TRUE.\n
  Diese Funktion wird nun verlassen und das aufrufende Hauptprogramm arbeit\n
  weiter. In der Zwischenzeit ist der AD-Wandler beschaeftigt um das Mess-\n
  ergebniss zu ermitteln.\n
  Ist der Wandler fertig, wird der Interrupt zum AD-Wandler aufgerufen und in\n
  der dazu vorhandene Interrupt-Funktion aus asuro.c berabeitet.\n
  Dort wird nun AUTOMATISCH das Messergebnis ausgewertet, ein erkannter\n
  Hell- Dunkel-Wechsel an der Encoderscheibe erkannt und dadurch der Zaehler\n
  in der globalen Variablen encoder[] weitergezaehlt.\n
  Ausserdem wird dort dann der AD-Wandler fuer die andere Seite gestartet.\n
  Da dies dann ab nun immer Wechsel laeuft, kann das Hauptprogramm, ohne\n
  weiters Zutun von nun ab auf die Zaehlerwerte in encoder[] zugreifen.

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  int main (void)
  {
    Init ();

    EncoderInit ();

    MotorDir (FWD, FWD);
    MotorSpeed (150, 150);
    while (1)
    (
      // Dein Programm

      if (encoder [0] > 500)
      {
        EncoderStop ();
        MotorSpeed (0, 0);
      }
    }
    return 0;
  }
  \endcode
*****************************************************************************/
void EncoderInit (void)
{
  /*
    Alle definierten Interrupts im Asuro sperren.
  */
  cli();

  /*
    Port C als Input => dadurch gehen die Back-LED aus ...
  */
  DDRC &= ~ ((1<<PC0) | (1<<PC1));
  /*
    ... aber nun koennen die LED's am Rad eingeschaltet werden, und die
        Sensoren koennen gemessen werden.
  */
  ODOMETRIE_LED_ON;

  /*
    AD-Wandler einschalten, Parameter einstellen und Starten. (clk/128)
    Startet den ADC im 'free running'-Mode (ADFR). Das heisst, der Wandler
    nach einer Messung automatisch wieder ne startet.
  */
  ADCSRA = (1<<ADEN) | (1<<ADFR) | (1<<ADIE) | (1<<ADSC) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);

  /*
    Linken Odometrie-Sensor auswaehlen. (AVCC ref. with external capacitor)
  */
  ADMUX = (1<<ADLAR) | (1<<REFS0) | WHEEL_LEFT;

  /*
    Odometrie im Interruptbetrieb weiter bearbeiten.
  */
  autoencode = TRUE;

  /*
    Alle definierten Interrupts im Asuro wieder zulassen.
  */
  sei();

  /*
    Die Odometrie Hell-/Dunkel-Zaehler zuruecksetzen/initialisieren.
  */
  EncoderSet (0, 0);
}



/****************************************************************************/
/*!
  \brief
  Den Interrupt Betrieb der Odometriesensoren-Messung anhalten.

  \param
  keine

  \return
  nichts

  \see 
  Die globale Variable autoencode hier auf FALSE gesetzt.

  \par  Funktionsweise:
  Durch das setzen der globalen Variablen autoencode auf FALSE wird in\n
  der AD-Wandler-Interruptfunktion der Teil zur Bearbeitung uebersprungen.\n
  Dadurch wird der Wandler nicht mehr neu gestartet und somit stopp die\n
  Automatik.

  \par  Beispiel:
  (siehe unter EncoderInit bzw. in den examples)
*****************************************************************************/
void EncoderStop (void)
{
  autoencode = FALSE;
}



/****************************************************************************/
/*!
  \brief
  Den Interrupt Betrieb der Odometriesensoren-Messung starten.

  \param
  keine

  \return
  nichts

  \see 
  Die globale Variable autoencode hier auf TRUE gesetzt.

  \par Beispiel:
  (siehe unter EncoderInit bzw. in den examples)
*****************************************************************************/
void EncoderStart (void)
{
  autoencode = TRUE;
}



/****************************************************************************/
/*!
  \brief
  Interruptbetriebene Odometriesensoren Werte vorbelegen.

  \param[in]
  setl Wert fuer links

  \param[in]
  setr Wert fuer rechts

  \return
  nichts

  \par  Hinweis:
  Initialisiert die beiden Werte in der globalen Variable encoder.\n
  Normalerweise werden die Zaehlerwerte mit 0 initialisiert. Fuer einige\n
  Anwendungen kann es sinnvoll sein auch schon bestimmte Werte vorzubelegen.

  \see
  Die globale Variable encoder wird hier initialisiert.

  \par Beispiel:
  (siehe unter den examples)
*****************************************************************************/
void EncoderSet (
  int setl,
  int setr)
{
  encoder [LEFT]  = setl;
  encoder [RIGHT] = setr;
}
