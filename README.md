fancontrol
==========
This program controls a fan to lower air humidity e.g. in basements.
It uses two combined temperature and humidity sensors to calculate
the approximate absolute humidity inside and outside.
If the absolute humidity outside is lower than inside and the outside
temperature is above the minimum temperature, the fan pin gets powered.

To read the sensors, Adafruits DHT sensor library is used and
has to be downloaded seperately.
https://github.com/adafruit/DHT-sensor-library

----------

Dieses Programm steuert einen Lüfter zur Senkung der Luftfeuchtigkeit
z.B. in Kellern.
Es nutzt zwei kombinierte Temperatur- und Luftfeuchtigkeitssensoren um jeweils
innen und außen die absolute Luftfeuchtigkeit zu berechnen.
Wenn die absolute Feuchtigkeit außen geringer ist, als innen und
die äußere Temperature die Mindesttemperatur überschreitet,
wird Lüfter eingeschaltet.

Um die Sensoren auszulesen wird die DHT sensor library von Adafruit benutzt.
Diese muss seperat herunter geladen werden.
https://github.com/adafruit/DHT-sensor-library
