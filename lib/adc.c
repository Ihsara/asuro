/****************************************************************************/
/*!
  \file     adc.c

  \brief    Funktionen zum Auslesen der ADC-Wandler.\n
            Gruppiert in die zusammenhaengenden Hardwareeinheiten.\n
            (Batterie, Liniensensoren, Radsensoren)\n
            Die Switches (Taster) werden in der Datei switches.c bearbeitet.

  \see      Defines fuer die Auswahl der ADC-Kanaele in asuro.h\n
            BATTERIE, IR_LEFT, IR_RIGHT, WHEEL_LEFT, WHEEL_RIGHT\n
            Der Kanal SWITCH wird hier \b nicht bearbeitet.

  \version  V--- - 10.11.2003 - Jan Grewe - DLR\n
            Original Version von der ASURO CD\n
  \version  V--- - bis zum 07.01.2007 - \n
            Bitte in Datei CHANGELOG nachsehen.\n
  \version  V001 - 13.01.2007 - m.a.r.v.i.n\n
            +++ Alle Funktionen\n
            Zerlegte Sourcen in einzelne Dateien fuer eine echte Library.
  \version  V002 - 22.01.2007 - Sternthaler\n
            +++ Alle Funktionen\n
            Kommentierte Version (KEINE Funktionsaenderung)
  \version  V003 - 20.02.2007 - m.a.r.v.i.n\n
            In Battery und OdometryData Funktion autoencode temporaer abschalten
  \version  V004 - 06.04.2007 - m.a.r.v.i.n\n
            Batterie und OdometrieData Funktionen umbenannt in 
            Battery und OdometryData.\n    
            Alte Funktionsnamen ueber Defines beibehalten   

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
  Liest die Batteriespannung und gibt sie zurueck.\n
  Es erfolgt keine Konvertierung in einen Spannungswert.

  \param
  keine

  \return
  10-Bit-Wert der Batteriespannung (Bereich 0..1023)

  \par  Die Spannung in Volt kann mit folgende Formel berechnet werden:
        Umess[V] = (Uref / 2 ^ ADC-Bitsanzahl) * Batterie ()\n
        Ubat[V]  = ((R1 + R2) * Umess) / R2

  Dabei sind:
  \par
        Uref = 2.56 Volt\n
        ADC-Bitsanzahl = 10 Bit\n
        R1 = 12000 Ohm auf der ASURO-Platine\n
        R2 = 10000 Ohm auf der ASURO-Platine

  Oder einfach:
  \par
        Ubat[V] = 0,0055 * Battery ()

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  // In Variable wert den 10-Bit-Bateriespannungswert lesen
  // und bei Unterschreitung von 810 eine alles_stop-Funktion
  // aufrufen.
  int wert;
  wert = Battery();
  if (wert < 810)             // 810 entsprechen ca. 4,455 Volt
    alles_stop ();            // Spannung zu klein, Akkus schonen
  \endcode
*****************************************************************************/
int   Battery (
  void)
{
  int   ec_bak = autoencode;            // Sichert aktuellen Zustand
  int   data;

  /*
     Autoencode-Betrieb vom ADC-Wandler unterbinden.
  */
  autoencode = FALSE;

  ADMUX = (1 << REFS0) | (1 << REFS1) | BATTERIE; // interne 2.56V Referenz
                                                  // Ref. mit ext. Kapazitaet
  ADCSRA |= (1 << ADSC);                // Starte AD-Wandlung
  while (!(ADCSRA & (1 << ADIF)))       // Ende der AD-Wandlung abwarten
    ;
  ADCSRA |= (1 << ADIF);                // AD-Interupt-Flag zuruecksetzen
  data = ADCL + (ADCH << 8);            // Ergebnis als 16-Bit-Wert
  /*
     Autoencode-Betrieb vom ADC-Wandler wiederherstellen.
  */
  autoencode = ec_bak;
  return data;
}



/****************************************************************************/
/*!
  \brief
  Liest die Daten der beiden Linienverfolgungssensoren.\n
  Die Linien-Beleuchtungs-LED kann sowohl an- als auch ausgeschaltet sein.

  \param[out]
  data Zeiger auf die gelesenen Daten:\n
       data[0] linker Sensor  (Bereich 0..1023)\n
       data[1] rechter Sensor (Bereich 0..1023)

  \return
  nichts

  \see
  Die globale Variable autoencode wird temporaer auf FALSE gesetzt und am Ende\n
  der Funktion mit dem alten Wert restauriert.

  \par  Hinweis:
  Die Linien-Beleuchtungs-LED kann vorher mit der Funktion FrontLED()\n
  im aufrufenden Programmteil an- bzw. ausgeschaltet werden.

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  unsigned int data [2];
  FrontLED (ON);
  LineData (data);
  if (data [0] > 100)         // 0 ist der linke Sensor ...
    tu_diese ();              // linker Sensor > 100
  if (data [1] < 50)          // ... 1 ist der rechte Sensor
    tu_das ();                // rechter Sensor < 50
  \endcode
*****************************************************************************/
void  LineData (
  unsigned int  *data)
{
  int   ec_bak = autoencode;            // Sichert aktuellen Zustand

  /*
     Autoencode-Betrieb vom ADC-Wandler unterbinden.
  */
  autoencode = FALSE;

  /*
     Linken Linien-Sensor lesen
  */
  ADMUX = (1 << REFS0) | IR_LEFT;       // Referenz mit externer Kapazitaet
  Sleep (10);
  ADCSRA |= (1 << ADSC);                // Starte AD-Wandlung
  while (!(ADCSRA & (1 << ADIF)))       // Ende der AD-Wandlung abwarten
    ;
  ADCSRA |= (1 << ADIF);                // AD-Interupt-Flag zuruecksetzen
  data [0] = ADCL + (ADCH << 8);        // Ergebnis als 16-Bit-Wert

  /*
     Rechten Linien-Sensor lesen
  */
  ADMUX = (1 << REFS0) | IR_RIGHT;      // Referenz mit externer Kapazitaet
  Sleep (10);
  ADCSRA |= (1 << ADSC);                // Starte AD-Wandlung
  while (!(ADCSRA & (1 << ADIF)))       // Ende der AD-Wandlung abwarten
    ;
  ADCSRA |= (1 << ADIF);                // AD-Interupt-Flag zuruecksetzen
  data [1] = ADCL + (ADCH << 8);        // Ergebnis als 16-Bit-Wert

  /*
     Autoencode-Betrieb vom ADC-Wandler wiederherstellen.
  */
  autoencode = ec_bak;
}



/****************************************************************************/
/*!
  \brief
  Liest die Daten der beiden Odometriesensoren (Radsensoren).\n
  Diese Funktion schaltet die Odometrie-LED's immer an.\n
  Diese Funktion schaltet die Back-LED's immer aus.

  \param[out]
  data Zeiger auf die gelesenen Daten:\n
       data[0] linker Sensor,\n
       data[1] rechter Sensor. (Bereich 0..1023)

  \return
  nichts

  \par  Hinweis:
  Die Odometrie-Beleuchtungs-LED's muessen zur Messung der Odometrie-\n
  sensoren wegen der Hardware im ASURO immer eingeschaltet sein.\n
  Die Hardware ist so ausgelegt, dass dadurch allerdings die hinteren\n
  Back-LED's ausgeschaltet werden.\n
  Da die Odometrie-Beleuchtungs-LED's in dieser Funktion EIN-geschaltet\n
  werden, funktionieren dann die Back-LED's nicht mehr. Sie koennen im\n
  Programm nach dieser Funktion mit BackLED() bei Bedarf wieder\n
  eingeschaltet werden.

  \par  Beispiel:
  (Nur zur Demonstration der Parameter/Returnwerte)
  \code
  unsigned int data [2];
  OdometryData (data);
  if (data [0] > data [1])    // 0 linker Sensor; 1 ist rechter Sensor
    fahre_links ();           // Ein bisschen nach links fahren
  if (data [0] < data [1])
    fahre_rechts ();          // Ein bisschen nach rechts fahren
  BackLED (ON, OFF);          // linke Back-LED mal wieder anschalten
  \endcode
*****************************************************************************/
void  OdometryData (
  unsigned int  *data)
{
  int   ec_bak = autoencode;            // Sichert aktuellen Zustand

  /*
     Autoencode-Betrieb vom ADC-Wandler unterbinden.
  */
  autoencode = FALSE;
  /*
     Vorbereitung zum lesen der Odometrie-Sensoren.
  */
  DDRC &= ~((1 << PC0) | (1 << PC1));   // Port auf Input=>Back-LEDs gehen aus
  ODOMETRIE_LED_ON;                     // Odometrie-LED's einschalten

  /*
     Linken Odometrie-Sensor lesen
  */
  ADMUX = (1 << REFS0) | WHEEL_LEFT;    // Referenz mit externer Kapazitaet
  ADCSRA |= (1 << ADSC);                // Starte AD-Wandlung
  while (!(ADCSRA & (1 << ADIF)))       // Ende der AD-Wandlung abwarten
    ;
  ADCSRA |= (1 << ADIF);                // AD-Interupt-Flag zuruecksetzen
  data [0] = ADCL + (ADCH << 8);        // Ergebnis als 16-Bit-Wert

  /*
     Rechten Odometrie-Sensor lesen
  */
  ADMUX = (1 << REFS0) | WHEEL_RIGHT;   // Referenz mit externer Kapazitaet
  ADCSRA |= (1 << ADSC);                // Starte AD-Wandlung
  while (!(ADCSRA & (1 << ADIF)))       // Ende der AD-Wandlung abwarten
    ;
  ADCSRA |= (1 << ADIF);                // AD-Interupt-Flag zuruecksetzen
  data [1] = ADCL + (ADCH << 8);        // Ergebnis als 16-Bit-Wert

  /*
     Autoencode-Betrieb vom ADC-Wandler wiederherstellen.
  */
  autoencode = ec_bak;
}
