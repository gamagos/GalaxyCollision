# Bearbeitete Konzepte des Informatik Projekts - Sebastian Fiault

## Vorraussetzungen

Diese "Dokumentation" der bearbeiten Konzepte in meinem Informatik Projekt setzt Java UND Python als Basis voraus.  
Das heißt das sämtliche Konzepte und Eigenheiten die in diesen Programmiersprachen gleich sind wie in C,  
werden höchstens kurz angedeutet und nicht weiter erläutert.  
Zum Beispiel wird was eine Variable ist, Grundlegene Datentypen oder die klassischen Schleifen/Verzweigungen nicht weit erklärt werden.  
Außerdem setzt diese Dokumentation gewisse Computer und Mathe Kentnisse voraus.  

## Vorwort

Die Dokumentation teilt bearbeitete Bereiche in mehrer Überthemen ein,  
die sich dann weiter zu Unterthemen verzweigen.  
Themen auf der selben "Stufe" sind dann alphabetisch sortiert.  

Außerdem möchte ich hinzufügen,  
dass das Format meiner Dokumentation,  
mit den Überschriften, Codeblöcken, Stickpunkten etc.  
KI bzw. LLM output womöglich ähnelt.  
Jedoch dieses Dokument, als auch ~80-90% meines Codes  
manuell ohne Hilfe von KI(direkt, als auch indirekt) geschrieben wurde.  
Mein Format ähnelt der Dokumentation ähnelt womöglich KI,  
da ich für Dokumentation meiner Projekte allgemein MarkDown verwende (.md Dateiformat),  
da es von dem was ich gesehen habe bei Developern allgemein, als auch in Foren wie z.B. StackOverflow die Standardform ist.  
MarkDown ist sehr einfach zu verwenden um schnell Dokumentation, Build Instructions als auch READMEs zu schreiben,  
weshalb ich es verwende, **als auch KIs es verwenden,**  
da es eben einfach ist und der Syntax gut kompatibel mit dem Tokensystem von LLMs ist.  
Diese MarkDown "rendere" ich dann zu HTML mithelfe einer Visual Studio Code Extension,  
weshalb sie diese Dokumentation wahrscheinlich als .html einsehen.  

Auf MarkDown Syntax werde ich in der Dokumentation nicht eingehen.  
Auf die Verwendung der tools zum Programmieren von C, GLSL und CMake werde ich nicht groß eingehen.  
Also wie man die CLIs von Compilern oder Visual Studio benutzt werde ich nicht erklären.  

## **C**

### Datentypen, Daten und "Objekte"

#### Arrays

##### Arrays wie in Jave und Python

In C kann man Arrays wie man es aus Python und Java kennt klassisch mit `type[size]` zuweisen.  
Doch wenn man in C `[]` benutzt muss man sich bewusst sein,  
dass **man sein Daten auf dem Stack ablegt.**  
Der Stack ist ein Bereich im RAM auf dem Daten in der Datenstruktur des Stacks angelegt sind.  
Das heißt wenn das Program den aktuellen Scope verlässt zu dem ihr Bereich auf dem Stack gehört werden dies wieder freigegeben, wie es üblich ist,  
wenn man sich von oben nach unten durch einen Stack durcharbeitet.  
In C sind die Daten im Stack im Call Stack (Konzept wird vorausgesetz, da es in Python und Jave ebenfalls call stacks gibt)  
neben Funktions Pointern in deren Stack Frames gespeichert.  
So macht es auch Sinn, dass wenn der Stack Frame einer Funktion aufgelöst wird dass auch der Platz,  
den sie für ihre Daten beansprucht hat freigegeben wird.  
Im Vergleich zum heap ist der Stack in Access Times,  
als auch in Lesegeschwindigkeiten schneller.  
Das kommt daher, dass der Stack ein einziger kontingenter Block an Speicher ist.  
Dem Entwickler bleibt das manuelle Freigeben von RAM erspart.  
Doch dafür sind Element auf dem Stack nicht dynamisch skalierbar,  
ist die Größe eine Elements einmal festgelegt kann sie nicht geändert werden.  
Die Größe muss sogar schon bei compile time bekannt sein,  
also muss der Entwickler sich schon während der Programmierung für eine Größe entscheiden und  
dann mit dieser bis zur Freigebung des Speichers arbeiten.  

**Außerdem verhindert C eine nicht daran out-of-bounds elemente in einem Array zu lesen.  
man kann also aus einem Array der Länge 5 das 6te, 7te, 8te ... Element lesen.**  
Meistens sind in den out-of-bounds Elementen Müllwerte,  
also zufällige Werte die das vorherige Programm zurückgelassen hat.  
Oder das Betriebsystem schmeißt das Program raus mit einer Access Violation,  
da das Programm Speicher, der womöglich einem anderen Programm zugewiesen wurde versucht zu benutzen.  
Da C einen nicht davon abhält falsche indexe zu benutzen muss man sich den maximalen Index entweder merken  
oder in eine "Variable" tun um auf den maximalen Index zurückgreifen zu können.  
*C mag es nicht wenn man für Stack Allocation Größen Speizifkation Variablen nimmt,  
weil diese nicht immer 100% sicher vor Änderungen sind z.B. durch andere Threads sind.  
Deswegen verwende ich dafür meisten unveränderbare Makros.  
Mehr zu Makros [hier](#preprocessor-makros)*  
**Beispiel:**
```C
#include <stdio.h>

int main()
{
	int myarr[5] = {1, 2, 3, 4, 5}; // Setz Größe automatisch auf 5
	printf_s("Out of bounds numbers: %d, %d", myarr[5], myarr[6]); // <== "Access Violation at 0x12345678" oder zufällige Zahlen
}
```

##### Heap Arrays

In C kann man allerdings auch Arrays auf dem Heap allocaten.  
Das geht jedoch dann mit Pointern und speziellen Methoden.  
Pointer werden [hier](#pointer) erklärt und für diesen Abschnitt vorausgesetzt.  
Arrays auf dem heap zuzuweisen ist relativ einfach.  
Man benutzt hierfür meist die Funktionen `malloc()`, `calloc()` und `realloc()` von stdlib.h Bibliothek.
Diese Funktion fragen beim Kernel Betriebsystems die vom Entwickler angegebene Menge an Speicherplatz an und  
wenn nichts schief läuft gibt die Methode einen Pointer zurück,  
welcher an den Anfang dieses Bereiches zeigt.  

**Beispiel:**
```C
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int _dataForHeap_[4] = {1,2,3,4}; // Daten die in den heap geladen werden sollen
	int* heap_ptr = malloc( sizeof(_dataForHeap_) );

	for 
}
```

#### Datentypen

In C sind die meisten Datentypen recht ähnlich zu Java.  
Es gibt die klassischen Datentypen `char`, `short`, `int`, `long`, `bool`(nur über stdbool.h), `float` und `double`.  
**`byte` und `String` gibt es nicht.**  
In C ist nämlich ein `char` gleichzeitig ein 1 Byte UTF-8 Zeichen,  
als auch das äquivalent des `byte` Typs in Java.  
Also ein 8 Bit Integer.  
Für Strings benutzt man Arrays von chars und dies sind am Ende mit dem "Null Terminator" `\0` char markiert, der signalisiert,  
dass der String bzw. in C Sprache `char` Array zu Ende ist.  

Außerdem sind in C die Größen der Standard Integer Datentypen nur als Minima definiert.  
So ist also ein `char` **mindestens** 8 Bit, 
ein `short` als auch ein `int` als mindestens 16 Bit (wobei shorts meisten 16 und nur ints meistens 32 sind)  
`long int` mindestens 32 Bit und ein `long long int` mindestens 64.  
Dasselbe gilt für ihre `unsigned` Varianten.  
Außerdem ist einem die Abkürzung dieser erlaubt:  
`long signed int` = `long`,  
`unsigned int` = `short unsigned`,  
usw.  
Es wird eigentlich immer vorausgesetzt das man,  
wenn man nur `long` oder `unsigned` schreibt einen `int` meint als auch,  
dass wenn man nicht `unsigned` schreibt man `signed meint`.

Und es gibt eine `long double` der entweder 80, 96 oder 128 Bit in verschiedenen Formate sein kann(weitgehend aber nicht unterstützt).

##### Spezielle Datentypen

##### Spezielle Integers von stdint.h

Es gibt in der Bibliothek stdint.h spezielle `int` Datentypen.  
Diese sehen meist so aus `intN_t`, `int_fastN_t` und `int_leastN_t`.  
Und ihre korrespondierend `unsigned` Version z.B. `uintN_t`.  
N kann hier entweder 8, 16, 32 oder 64 sein und gibt an,  
wie viel Bits der `int` groß sein soll.  
`intN_t` ist einfach nur ein `signed int` mit genau N Bits.  
`int_leastN_t` ist der kleinstmögliche `signed int` der mindestens N Bit groß ist.  
Doch dieser Datentyp wird nur sehr selten und eher auf komischen Architekturen bzw. eingebetten System verwendet.  

###### `size_t` von stdlib.h (in mehren Standardbibliotheken definiert)

`size_t` ist ein spezieller unsignierter Integer Datentyp,  
welcher die maximale Größe eines Objekts auf dem System in Bytes beinhalten kann.  
So ist er auf 32 Bit System zum Beispiel meist ein `unsigned int` oder `int32_t`,  
also ein 32 bit unsignierter Integer.  
Und auf 64 Bit System dann ein 64 Bit unsignierter Integer.  

#### Zusätchliches zu Daten

Wenn man in C eine Variable nicht definiert.  
Also sie einfach nur deklariert ohne ihr einen Wert zu geben und sie dann ausliest kann alles mögliche passieren.  
Den C stoppt eine nicht eine Variable zu lesen die keine wert zugewiesen bekommen hat (auch wenn die meisten Compiler Warnungen geben).  
Meistens befindet sich in dann in dem Speicher Mülldaten, die ein vorheriges Programm zurückgelassen hat.  
Also irgendwelche zufälligen Zahlen.  
**Beispiel hierfür**
``` C
#include <stdio.h>

int main() 
{
	int num;
	printf_s("Number: %d", num); // <== Gibt zufällige Zahl aus
}
```

### Header (.h) Dateien



### Preprocessor Makros



### Pointer

Pointer sind
