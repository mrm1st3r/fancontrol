# fancontrol
## Allgemein
Dieses Programm steuert einen Lüfter zur Senkung der Luftfeuchtigkeit
z.B. in Kellern.
Es nutzt zwei kombinierte Temperatur- und Luftfeuchtigkeitssensoren um jeweils
innen und außen die absolute Luftfeuchtigkeit zu berechnen.
Wenn die absolute Feuchtigkeit außen geringer ist, als innen und
die äußere Temperature die Mindesttemperatur überschreitet,
wird Lüfter eingeschaltet.

## Abhängigkeiten
* DHT Sensoren: https://github.com/adafruit/DHT-sensor-library
* Display: Für die Displayansteuerung wird die LiquidCrystal Bibliothek benutzt

## Hardware
Für die bestmögliche und vollständige Funktion wird folgende
zusätzliche Hardware benötigt:
* 1x Arduino Uno
* 2x DHT22 Temperatur- und Luftfeuchtesensor
* 1x Relais-Board um den/die Lüfter anzusteuern
* 1x ein beliebiges 16x2 LCD Display, das von der LiquidCrystal Bibliothek
unterstützt wird (getestet mit JHD162a)
    * Peripherie (z.B. Potentiometer für Kontrastspannung)

## Verbindung
Die aufgelistete Hardware sollte wie folgt an den Arduino angeschlossen werden
(alternativ können die Ports im Programm geändert werden):

2  => LCD DB7
3  => LCD DB6
4  => LCD DB5
5  => LCD DB4
6  => DHT data (innen)
7  => DHT data (außen)
8  => Relais
11 => LCD Enable
12 => LCD RS

Zusätzliche Verbindungen für das Display (JHD162a):

1  => GND
2  => VCC 5V
3  => Kontrastspannung
5  => GND
15 => VCC 4.2V (Hintergrundbeleuchtung)
16 => GND (Hintergrundbeleuchtung)

## Konfiguration
Sie können die Funktionalität durch anpassen der config.h auf ihre Bedürfnisse
anpassen.
