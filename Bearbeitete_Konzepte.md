# Bearbeitete Konzepte des Informatik Projekts - Sebastian Fiault

## Vorraussetzungen

Diese "Dokumentation" der bearbeiten Konzepte in meinem Informatik Projekt setzt Java UND Python als Basis voraus.  
Das heißt das sämtliche Konzepte und Eigenheiten die in diesen Programmiersprachen gleich sind wie in C,  
werden höchstens kurz angedeutet und nicht weiter erläutert.  
Zum Beispiel wird was eine Variable ist, Grundlegene Datentypen oder die klassischen Schleifen/Verzweigungen nicht weit erklärt werden.  
Außerdem setzt diese Dokumentation gewisse Computer und Mathe Kentnisse voraus.  

## Struktur der Projektdokumentation

Die Dokumentation teilt bearbeitete Bereiche in mehrer Überthemen ein,  
die sich dann weiter zu Unterthemen verzweigen.  
Themen auf der selben "Stufe" sind dann alphabetisch sortiert.

## **C**

### Datentypen, Daten und "Objekte"

#### Arrays

##### Arrays wie in Jave und Python

In C kann man Arrays wie man es aus Python und Java kennt klassisch mit `type[size]` zuweisen.  
Doch wenn man in C `[]` benutzt muss man sich bewusst sein,  
dass **man sein Daten auf dem Stack ablegt.**  

##### Heap Arrays

todo!

#### Datentypen

In C sind die meisten Datentypen recht ähnlich zu Java.  
Es gibt die klassischen Datentypen `char`, `short`, `int`, `long`, `bool`(nur über stdbool.h), `float` und `double`.  
**`byte` und `String` gibt es nicht.**  
In C ist nämlich ein `char` gleichzeitig ein 1 Byte UTF-8 Zeichen,  
als auch das äquivalent des `byte` Typs in Java.  
Also ein 8 Bit Integer.  
Für Strings benutzt man Arrays von chars und dies sind am Ende mit dem "Null Terminator" `\0` char markiert, der signalisiert,  
dass der String bzw. in C Sprache `char` Array zu Ende ist.  
