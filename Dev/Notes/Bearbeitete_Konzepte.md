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
dass das Format meiner Dokumentation mit den Überschriften, Codeblöcken, Stickpunkten etc.  
KI bzw. LLM output womöglich ähnelt.  
Jedoch dieses Dokument, als auch ~80-90% meines Codes  
manuell ohne Hilfe von KI(direkt, als auch indirekt) geschrieben wurde.  
Mein Format der Dokumentation ähnelt womöglich KI,  
da ich für Dokumentation meiner Projekte allgemein MarkDown verwende (.md Dateiformat),  
da es von dem was ich gesehen habe bei Developern allgemein, als auch in Foren wie z.B. StackOverflow die Standardform ist.  
MarkDown ist sehr einfach zu verwenden um schnell Dokumentation, Build Instructions als auch READMEs zu schreiben,  
weshalb ich es verwende, **als auch KIs es verwenden,**  
da es eben einfach ist und der Syntax gut kompatibel mit dem Tokensystem von LLMs ist.  
Diese MarkDown "rendere" ich dann zu HTML mithelfe einer Visual Studio Code Extension,  
weshalb sie diese Dokumentation wahrscheinlich als .html einsehen.  

Auf MarkDown Syntax werde ich in der Dokumentation nicht eingehen.  
Auf die Verwendung der Tools zum Programmieren von C, GLSL und CMake werde ich nicht groß eingehen.  
Auch wie man die CLIs von Compilern oder Visual Studio benutzt werde ich nicht erklären,  
da diese Dokumentation darum geht wie diese Dinge funktionieren und nicht wie man sie erstellt.  

## C

C ist eine 50+ Jahre alte funktionale Programmiersprache.  
Sie wird von vielen als der Vater der OOP angesehen und obwohl die Sprache 50 Jahre alt ist,  
ist sie immer noch eine der meist verwendeten Programmiersprachen der Welt([laut der StackOverflow Developer Survey 2025](https://survey.stackoverflow.co/2025/technology#most-popular-technologies)).  
C wird vor allem in eingebetteten System, Betriebsystemem, bzw. deren Kernels  
und Treibern verwendet.  
Und sonst allem anderen was low-level sein muss.  

### Datentypen, Daten und "Objekte"

#### Arrays

##### Arrays wie in Java und Python

In C kann man Arrays wie man es aus Python und Java kennt klassisch mit `type[size]` zuweisen.  
Doch wenn man in C `[]` benutzt muss man sich bewusst sein,  
dass **man sein Daten auf dem Stack ablegt.**  

Der Stack ist ein Bereich im RAM auf dem Daten in der Datenstruktur des Stacks angelegt sind.  
Das heißt wenn das Program den aktuellen Scope verlässt wird der zum Programm gehörende Bereich auf dem Stack wieder freigegeben, wie es üblich ist,  
wenn man sich von oben nach unten durch einen Stack durcharbeitet.  
In C sind die Daten im Stack im Call Stack (Konzept wird vorausgesetz, da es in Python und Java ebenfalls Call Stacks gibt)  
neben Funktions Pointern in deren Stack Frames gespeichert.  
So macht es auch Sinn, dass wenn der Stack Frame einer Funktion aufgelöst wird dass auch der Platz,  
den sie für ihre Daten beansprucht hat freigegeben wird.  

Im Vergleich zum heap ist der Stack in Access Times,  
als auch in Lesegeschwindigkeiten schneller.  
Das kommt daher, dass der Stack ein einziger kontingenter Block an Speicher ist.  

Dem Entwickler bleibt das manuelle Freigeben von RAM erspart.  
Doch dafür sind Element auf dem Stack nicht dynamisch skalierbar,  
ist die Größe eines Elements einmal festgelegt kann sie nicht geändert werden.  
Die Größe muss sogar schon bei compile time bekannt sein,  
also muss der Entwickler sich schon während der Programmierung für eine Größe entscheiden und  
dann mit dieser bis zur Freigebung des Speichers arbeiten.  

**Außerdem verhindert C einen nicht daran out-of-bounds elemente in einem Array zu lesen.  
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
Mehr zu Makros [hier](#preprocessor-macros-und-preprocessor-directives)*  
**Beispiel:**

```C
#include <stdio.h>

int main()
{
    int myarr[5] = {1, 2, 3, 4, 5}; // Setz Größe automatisch auf 5
    printf_s("Out of bounds numbers: %d, %d", myarr[5], myarr[6]); // <== "Access Violation at 0x12345678" oder zufällige Zahlen
}
```

Man *kann* Stack Arrays auch mit [Pointern](#pointer) indexen:

```C
#include <stdio.h>

int main()
{
    int myarr[5] = {1, 2, 3, 4, 5}; // Setz Größe automatisch auf 5
    int* tmp_myarr_ptr = myarr;

    for (size_t i = 0; i < 4; i++) // Da es sich um Pointer handelt kann man auch anders indexen als nur mit []
    {
        printf_s("Number: %d\n", *tmp_myarr_ptr); // <== "Access Violation at 0x12345678" oder zufällige Zahlen nach fünftem Durchgang
        tmp_myarr_ptr ++;
    }
}
```

##### Heap Arrays

In C kann man allerdings auch Arrays auf dem Heap allocaten.  
Das geht jedoch dann mit Pointern und speziellen Methoden.  
Pointer werden [hier](#pointer) erklärt und für diesen Abschnitt vorausgesetzt.  
Arrays auf dem heap zuzuweisen ist relativ einfach.  
Man benutzt hierfür meist die Funktionen `malloc()`, `calloc()` und `realloc()` von der stdlib.h Bibliothek.
Diese Funktionen fragen beim Kernel Betriebsystems die vom Entwickler angegebene Menge an Speicherplatz an und  
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

    int* tmp_heap_ptr = heap_ptr;
    for (size_t i = 0; i < 4; i++) // Da es sich um Pointer handelt kann man auch anders indexen als nur mit []
    {
        printf_s("Number: %d\n", *tmp_heap_ptr);
        tmp_heap_ptr ++; // Auf pointern sind mathematische Operatoren mit der Größe der Pointer gescaled.
                         // Dass heißt ++(+ 1) wird zu + (1 * sizeof(int))
    }
    free(heap_ptr); // Speicherplatz wieder freigeben
    heap_ptr = 0;     // Pointer zu 0 setzen um sogenannte "Dangling Pointers" zu vermeiden, "Dangling Pointers" sind Pointers die zu nicht zugewiesenem  
    tmp_heap_ptr = 0; // Speicher zeigen und können Probleme bereiten, da man nicht wissen kann ob ein Pointer RAM zugewiesen hat oder nicht
}
```

Wie man am Beispiel sehen kann, muss man den RAM für Heap Arrays nicht nur manuell anfragen,  
sondern auch wieder freigeben.  
Und dieses Freigeben ist auch sehr wichtig, denn sonst frisst das Program mehr und mehr RAM.  
Außerdem müssen bzw. **sollten** Pointer immer nach dem sie zu keinem validen Speicher mehr zeigen zu null gesetzt werden,  
da sonst andere stellen des Codes nicht wissen ob der RAM zu dem sie zeigen noch dem Program gehört oder nicht.  
Außerdem kann man heap arrays nicht nur mit `[]` indexen sondern auch mit pointern,  
welche unterschiedliche Stellen des Arrays indexen.  
Strengenommen kann man auch Stack Arrays mit Pointern indexen aber das wird seltener implementiert.

Der Heap ist neben dem Stack der andere Hauptbereich im RAM den C benutzt.  
Der Heap ist viel mehr ein grober, manchmal zerstückelter **Haufen** im RAM.  
Die Access Times des Heaps sind langsamer,  
da man mehr oder weniger wild durch zerstückelten RAM durchliest (vergleichbar zu slow-downs wenn HDDs fragmentiert sind auch wenn nicht soo extrem).  
Dafür dass der Heap aber eben ein etwas zerstückelter Bereich separat vom Stack ist kann man auf ihm  
wesentlich größere Datenmengen ablegen und auch seine Größe während Runtime dynamisch anpassen,  
was mit dem Stack nicht geht.  
Deshalb wird immer mit großen Datenmengen oder Datenmengen mit variabler Größe der Heap verwendet.
Mit dem Nachteil das man sehr gut auf seine Pointer aufpassen muss und nicht vergessen darf,  
unbenuzte Bereiche freizugeben.  

*Eigentlich sind der Stack und der Heap auch nur Konstrukte von C zur Abstraktion, das tätsächliche Layout des RAMs  
ist eben einfach nur so der RAM ohne Bereiche.  
Außer eben ein Program reserviert sich einen Bereich des RAMs.  
Stack und Heap sind dann einfach Teil des sogenannten Memory Models von C.*  

#### Datentypen

In C sind die meisten Datentypen recht ähnlich zu Java.  
Es gibt die klassischen Datentypen `char`, `short`, `int`, `long`, `bool`(nur über stdbool.h), `float` und `double`.  
**`byte` und `String` gibt es nicht.**  
In C ist nämlich ein `char` gleichzeitig ein 1 Byte UTF-8 Zeichen,  
als auch das äquivalent des `byte` Typs in Java.  
Also ein 8 Bit Integer.  

Für Strings benutzt man Arrays von chars und diese sind am Ende mit dem "Null Terminator" `\0` char markiert, der signalisiert,  
dass der String bzw. in C Sprache `char` Array zu Ende ist.  
Und die klassischen Control Character funktioniern in C "Strings" genau wie in den meisten anderen Programmiersprachen  
wie z.B. `'\t'`, `'\n'`, `'\r'` usw.  
Einzelne chars werden in '' geschrieben und char arrays sind in "" geschrieben,  
wie in Java und Python.  
Wenn man einen char array so definiert: `char myarr[] = "Informatik ist super"`,  
hängt C automatisch einen Null Terminator an.  

Außerdem sind in C die Größen der Standard Integer Datentypen nur als Minima definiert.  
So ist also ein `char` **mindestens** 8 Bit(in echt eigentlich immer 8 Bit),  
ein `short` als auch ein `int` als mindestens 16 Bit (wobei shorts meisten 16 und ints meistens 32 sind)  
`long int` mindestens 32 Bit und ein `long long int` mindestens 64.  
Dasselbe gilt für ihre `unsigned` Varianten.  
Also die Varianten,  
die nur den positiven Zahlenbereich und 0 beinhalten.  

Außerdem ist einem die Abkürzung/Mischung dieser so erlaubt:  
`long signed int` = `long`,  
`unsigned short int` = `short unsigned`,  
usw.  
Es wird eigentlich immer vorausgesetzt das man,  
wenn man nur `long` oder `unsigned` schreibt einen `int` meint als auch,  
dass wenn man nicht `unsigned` schreibt man `signed meint`.

Und es gibt eine `long double` der entweder 80, 96 oder 128 Bit in verschiedenen Formate sein kann(weitgehend aber nicht unterstützt wird).  

##### Spezielle Datentypen

##### Spezielle Integers von stdint.h

Es gibt in der Bibliothek stdint.h spezielle `int` Datentypen.  
Diese sehen meist so aus `intN_t`, `int_fastN_t` und `int_leastN_t`.  
Und ihre korrespondierend `unsigned` Version z.B. `uintN_t`.  

N kann hier entweder 8, 16, 32 oder 64 sein und gibt an,  
wie viel Bits der `int` groß sein soll.  

`intN_t` ist einfach nur ein `signed int` mit genau N Bits.  

`int_fastN_t` ist der schnellsetmögliche `int` Typ für N Bits und ist mindestens N Bits groß.  
Schnellstmöglich heißt in diesem Fall meistens,  
dass die Größe aufgerundent wird um die Größe eines CPU Registers zu sein,  
da meist Operationen mit Daten die so groß sind wie die Register für Integer weniger Compute brauchen um  
durchgeführt zu werden.  
Denn manchmal müssen CPUs für Daten, welche kleiner sind als die Register spezielle Konversionen oder Allignements machen.  
Welcher `int` hierbei am Ende rauskommt ist abhängig von Compiler und dem Ziel System bzw. der Ziel Prozessorarchitektur.  

`int_leastN_t` ist der kleinstmögliche `signed int` der mindestens N Bit groß ist.  
Doch dieser Datentyp wird nur sehr selten und eher auf komischen Architekturen bzw. eingebetten System verwendet.  

###### `errno_t` von stdlib.h (in mehren Standardbibliotheken definiert)

`errno_t` ist ein spezieller signierter Integer Datentyp,  
der dafür gedacht ist Fehlercodes zu speichern.  
Er signalisiert Entwicklern, dass es sich hier um einen Fehlercode handelt.  
Seine tatsächliche Größe ist platform- und compilerabhängig.  

###### `size_t` von stdlib.h (in mehren Standardbibliotheken definiert)

`size_t` ist ein spezieller unsignierter Integer Datentyp,  
welcher die maximale Größe eines Objekts auf dem System in Bytes beinhalten kann.  

So ist er auf 32 Bit System zum Beispiel meist ein `unsigned int` oder `int32_t`,  
also ein 32 bit unsignierter Integer.  
Und auf 64 Bit System dann ein 64 Bit unsignierter Integer.  

#### Structs

Structs sind ein sehr C eigenes Konzept.  
Einfach gesagt sind sie sozusagen Klassen,  
welche weder Funktionen beinhalten,  
noch irgendwelche Erbschaftsfunktionalität haben.
Sie beinhalten nur Daten und werden zum organisierten Management von Daten verwendet.  
**Beispiel:**  

```C
#include <stdbool.h>
#include <stdio.h>

struct Car 
{
    char* manufacturer;
    char* model;
    unsigned short int releaseYear;
    float horsePower;
    bool isConvertible;
};

int main()
{
    struct Car VW_Golf = {
        .manufacturer = "Volkswagen",
        .model = "Golf 7",
        .releaseYear = 2012,
        .horesPower = 150.0f,
        .isConvertible = false
    };
    // Um auf Wert zuzugreifen
    printf_s("%s\n", VW_Golf.model);

    // Um auf Wert zuzugreifen, wenn man den Pointer zum struct hat
    struct Car* VW_Golf_ptr = &VW_Golf;
    printf_s("%s\n", VW_Golf_ptr->manufacturer);
}
```

Besonders an structs ist,  
dass man bei der Benutzung trotzdem immer das `struct` keyword davor schreiben muss.  
Und das man direkt von einem Poiner zu einem Struct auf seine Werte mit dem `->` Operator zugreifen kann.  

Da immer `struct` vor seine benannten structs zu schreiben macht man sich meistens das leben so leichter:

```C
#include <stdbool.h>
#include <stdio.h>

typedef struct Car 
{
    char* manufacturer;
    char* model;
    unsigned short int releaseYear;
    float horsePower;
    bool isConvertible;
} Car;

int main()
{
    // Jetzt kann man sich hier das struct keyword sparen
    Car VW_Golf = {
        .manufacturer = "Volkswagen",
        .model = "Golf 7",
        .releaseYear = 2012,
        .horesPower = 150.0f,
        .isConvertible = false
    };
}
```

Mehr zu dem typedef keyword [hier](#typedefs).  

#### Typedefs

Typedefs erstellen Aliase für Datentypen um deren Benutzung zu vereinfachen.  
Einen typedef Alias definiert man mit dem `typedef` keyword und diesem Syntax.  

```C
typedef int ganze_zahl;

ganze_zahl num = 10;
```

Wie man sehen kann gibt das einem bestehenden Datentyp einfach einen neuen Name.  
Am nützlichsten ist diese Funktion mit structs als auch mit spezifischen Datentypen.  
So sind z.B. `size_t` von stdlib.h oder `int32_t` von [stdint.h](#spezielle-integers-von-stdinth) einfach nur typedef Aliase für bestimmte Integer.  

#### Zusätchliches zu Daten

Wenn man in C eine Variable nicht definiert.  
Also sie einfach nur deklariert ohne ihr einen Wert zu geben und sie dann ausliest kann alles mögliche passieren.  
Den C stoppt eine nicht eine Variable zu lesen die keine wert zugewiesen bekommen hat (auch wenn die meisten Compiler Warnungen geben).  
Meistens befindet sich in dann in dem Speicher Mülldaten, die ein vorheriges Programm zurückgelassen hat.  
Also irgendwelche zufälligen Zahlen.  
**Beispiel hierfür:**

``` C
#include <stdio.h>

int main() 
{
    int num;
    printf_s("Number: %d", num); // <== Gibt zufällige Zahl aus
}
```

### Compiler

Compiler sind die Programme die den C code zu Maschinencode (Assembly) umkonvertieren und das Programm als funktionierende .exe  
mit zugehörigen .dll Dateinen auspuckt. (Oder .lib, .dylib etc. je nach Platform)

### Header (.h) Dateien

Header Dateien in C sind Datein,  
welche Deklarationen und manchmal Definition beinhalten.  
Man kann sie mit Java Interfaces oder abstrakten Klassen am besten vergleichen,  
da sie meist auflisten was die Implementierung beinhalten muss,  
diese aber nicht selber definieren(meistens).  

*Strenggenommen kann man ein komplettes C Program in einer .h Datei schreiben aber das ist in sehr nichigen Verwednungszwecken und nicht Standard.*  

Header files werden verwendet um die Funktionalität
einer C Datei anderen C Datein mitzuteilen ohne dass diese die Implementierung kennen müssen.

Header Dateien beinhalten meist nur struct Definition,  
Konstanten Definitionen und Methoden Prototypen.  

Methoden Prototypen sind einfach nur Methoden ohne die Implementierung.  
Also nur Methodensignatur ohne den Teil in `{}`.  

Die Hauptverwendung dieser ist dann von anderen Dateien mit `#include`  
inkludiert zu werden um der Logik in der nächsten Datein zu sagen was die andere Datei bereithält.  

Diese Funktionalität macht sehr viel Sinn,  
wenn die Implementierungsdatein z.B. eine Bereits kompilierte binäre Bibliothek ist,  
und man trotzdem wissen muss was diese beinhaltet.  

Zusätzlich zu dem verhilfen sie dem Linker die Dateien richtig zu linken,  
da wenn man .c Datein in mehr als einer Datei inkludiert ist es möglich das Dinge mehrmals definiert werden was zu Link Fehlern führt.  
Mit .h Datein weiß der Linker was wohin muss (.h und .c Datein müssen hierfür immer gleich bennant sein)  
und kompiliert erst die C Datein einzeln und linkt sie dann mithilfe der .h Dateien die dem Linker sagen wo diese hin müssen.  

Und sie sind nützlich um anderen Entwicklern schnell mitzuteilen welche Funtionalität ein Datei beinhaltet.  
Das Python Äquivalent wären .pyi Datein und in Java die Interfaces/abstrakten Klassen.  

### Linker

Linker sind die Programme die die kompilierten Dateien so zusammenfügen **(linken)**,  
dass das Program funktionert.  

### Preprocessor Macros und Preprocessor Directives

#### Macros

Preprocessor Macros verhalten sich wie der Name schon sagt wie Makros.  
In Java, als auch Python werden Makros kaum verwendet bzw. existieren erst gar nicht.  
Ein Makro ist wie eine Art Funktion,  
jedoch hat ein Makro einen fundementalen unterschied.  
Während in Funktion bzw. Methoden man in einer Funktion in einen anderen Scope geht,  
bleibt man mit einer Makro im aktuellen scope.  
Effektive ist eine Makro dann einfach nur ein Kürzel für einen vorher definierten Block an Code bzw. Wert,  
der bei der Kompilierung einfach nur anstelle der Makro eingefügt wird.  
**Ein Beispiel hilft hier etwas mit der Erklärung:**  

```C
#include <stdio.h>

#define myMacroValue 17
#define myMacroCode printf_s("Bin in einer Makro\n"); \
                    printf_s("Ja ganz toll\n") // Wenn Makros über mehrere Zeilen Spannen muss man das mit einem \ anzeigen, sonst versteht der Compiler nicht, dass die Makro mehrere Zeilen beinhalten soll. Hinter dem \ dürfen keine anderen Zeichen stehen
#define myFunctionLikeMacro(x, y) printf_s("x: %s\n", x); \
                                  printf_s("y: %s\n", y)

int main()
{
    printf_s("myMacroValue: %d\n", myMacroValue);
    myMacroCode;
    myFunctionLikeMacro("Text1", "Text2");
}
```

**Während des Comipilens sieht der Code intermediär, wenn die Makros aufgelöst werde, so aus:**

```C
int main()
{
    printf_s("myMacroValue: %d\n", 17);
    printf_s("Bin in einer Makro\n"); 
    printf_s("Ja ganz toll\n");
    printf_s("x: %s\n", "Text1");
    printf_s("y: %s\n", "Text2");
}
```

Wie man sehen kann werden in den ersten zwei Fällen einfach nur der identifier der Makro mit dem Text der nach dem Name der Makro  
im `#define` statement steht ersetzt.  
Einmal ist das ein Wert das andere mal ein Block an Code.  
Doch der 3. Fall der sogenannten "Function-like Macro" ist etwas interessanter.  
Die Definition der Makro ähnelt etwas der einer Funktion,  
jedoch fällt auf,  
dass ihre Parameter keine Datentypen spezifiziert haben.  
Beim aufrufen so einer funktionsartigen Makro werden zuerst die "Parameter" mit dem was man angegeben hat
ersetzt und dann die Makro im Code ersetzt.  
Die "Parameter" verhalten sich also selber nochmal wie Makros,  
nur das sie eben nicht beim `#define` definiert werden sondern im  
Aufruf der Makro.

Man kann definierte Makros auch mit `#undef` undefinieren,  
doch in der Realität wird das nur selten angewendet.

#### Preprocessor Directives

##### If Verzweigungen im Preprocessor

Man kann Verzweigungen mit Preprocessor directives schreiben.  
Doch dies Funktionieren leicht anders von If Verzweigungen wie man sie gewohnt ist.  
Hier einmal eine Tabelle aller Verzweigungs-Directives:  

|     Directive       |                               Was es macht                                                                                                                                                          |
| ------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `#if <Ausdruck>`    | Checkt ob einen Ausdruck hinter dem if für wahr oder falsch. Funktioniert nur mit Elementen die in der Preprocessor Stufe des Compilers bereits existieren(andere mit #define definierte Elemente). |
| `#ifdef <Makro>`    | Überprüft ob eine Makro definiert ist                                                                                                                                                               |
| `#ifndef <Makro>`   | Überprüft ob eine Makro nicht definiert ist                                                                                                                                                         |
| `#else`             | Was auszuführen ist wenn die vorherige `#if` Makro false ist(wie normales else)                                                                                                                     |
| `#elif <Ausdruck>`  | Wie `#else` + `#if`                                                                                                                                                                                 |
| `#elifdef <Makro>`  | Wie `#else` + `#ifdef`                                                                                                                                                                              |
| `#elifndef <Makro>` | Wie `#else` + `#ifndef`                                                                                                                                                                             |
| `#endif`            | Schließt den `#if` oder `#elif` Block da man keine `{}` für Preprocessor Directives verwendet                                                                                                       |

Insgesamt verhalten dies sich recht ähnlich wie man if, elseif und else von Java und Python gewohnt ist,  
nur das sie eben nur mit anderen Makros bzw. Directives zusammen funktionieren und das if defined() zu ifdef z.B. wird.  
Und eben dass man `if` Blöcke mit `#endif` schließt statt `}`.  

Diese Verzweigungen werden einmal als Schutz benutzt um nichts zu überschreiben,  
was woanders defienrt wurde.  

```C
#define Lieblingsfach "Informatik"

// ...
// Ganz weit oben in der Datei, vielleicht eine andere Datei sogar
// ...

// Jetzt Vorsicht dass man den Wert nicht ausversehen ungewollt unwissend überschreibt
#ifndef Lieblingsfach
#define Lieblingsfach "Nicht Informatik"
#endif
// Lieblingsfach wird nicht überschrieben. Zum Glück!
```

Sonst werden diese Directives auch des öfteren verwendet um Programme für Multiplatfrom Kompatibilität anzupassen.  
**Beispiel:**

```C
// ...

int main()
{
    #ifdef OS_IS_WINDOWS
        funktion_wo_nur_in_windows_geht(variable);
    #elif OS_IS_LINUX
        funktion_wo_nur_in_linux_geht(variable);
    #elif OS_IS_MAC
        funktion_wo_nur_auf_mac_geht(variable);
    #else
        perror("Benutzt du OpenBSD oder was?!");
    #endif
}
```

Das geht, da alles, was in Blöcken steht, welche nicht true sind weggelassen wird.  

##### Include (`#include`)

Die `#include` directive ist so ziemlich das import von C.  
Effektive tut sie einfach nur den Inhalt der spezifizierten .c oder .h Datein den man angegeben hat  
an der Stelle wo sie stand einfügen.  
Meistens tun C Entwickler Header Dateien inkludieren anstatt C source code Dateien,  
da es ein Standard ist und es unnötig ist die ganze Quellcodedatei zu inkludieren,  
da der Linker es dann einfacher hat zu wissen, was wo hin muss.

### Pointer

Pointer sind einfach nur rohe RAM Adressen als integer abgespeichter.  
Was RAM Adressen sind und wie der RAM funktioniert wird hier vorausgesetzt und nicht erklärt.  

In C wird ein Pointer definiert indem man hinter den Datentyp ein `*` schreibt.  

```C
int data = 5347;
int* data_ptr = &data;
```

Pointer sind meist unsignierte Integer,  
die so groß sind wie die RAM Adressen der Architektur.  
x86_64 Systeme haben also 64 Bit unsignierte Integer als Pointer,  
x86_32 System 32 Bit unsignierte Integer usw.  

**Wenn man Mathematik mit Pointern macht, wie z.B. + 1 sind diese mit der Größe des Datentyps gescaled,  
sodass + 1 z.B. zu + (1 * sizeof(datentyp)) wird.  
Somit kann einfach von Element zu Element gesprungen werden ohne die Datenauslese  
komplett zu korrumpieren weil der Pointer in die Mitte eines Elementes zeigt.**

#### Funktions Pointer

Funktions pointer funktionieren sehr ähnlich wie reguläre Pointer,  
nur dass diese eben zu Funktionen zeigen.  
So werden sie definiert:

```C
int add(int a, int b)
{
    return a + b;
}

int (*func_ptr)(int, int);
func_ptr = &add; // An sich kann man das & weglassen, da Funktion in C so bereits Pointer sind, aber mit & ist was man vorhat explizieter
```

Interessant am Syntax ist, dass man den Namen des Pointers so schreiben muss `(*Name)` um zu signalisieren,  
dass es sich um einen Funktionspointer handelt.  
Und dass man die Datentypen der Parameter spezifizieren muss.  

**Um die Funktion über den Pointer zu benutzen ruft man die Funktion wie gewohnt auf:**

```C
int add(int a, int b)
{
    return a + b;
}

int (*func_ptr)(int, int);
func_ptr = &add;

int main()
{
    printf_s("1 + 2 is: %d\n", func_ptr(1, 2))
}
```

Das ist vor allem nützlich wenn man callbacks schreiben möchte.

#### Pointer Operatoren

|    Operator                       |                                             Was er macht                                                   |
| --------------------------------- | ---------------------------------------------------------------------------------------------------------- |
| `&<Element>`(Adress-of)           | Gibt die Adresse eines Elementes zurück. **Kann sogar die Adressen von Funktionen besorgen**               |
| `*<Pointer>`(Dereference)         | Folgt dem Pointer und gibt die Daten zurück die dort vorzufinden sind                                      |
| `<Pointer>[<index>]`(Dereference) | Folgt dem Pointer und gibt die Daten zurück, die index * Größe des Datentyps Bytes weiter im Speicher sind |

### Sonstiges zu C

- Es gibt zwar keine Klassen aber theoretisch könnte man ein mit einem Struct, welches Funktionspointer hat ähnliches verhalten immitieren.
- Da es in C keine Klassen gibt ist jede Methode die man importiert direkt verfügbar, also statt `class.method()` einfach nur `function()`
- In C gibt es kein `try`, `catch` bzw. `except` wie in Java und Python. In C arbeitet man roh und das heißt wenn es einen Fehler gibt, gibt es entweder eine SEGMENTATION FAULT, ACCESS VIOLATION oder einfach nur falsche Zahlen. Um Fehler im Code zu finden muss man entweder ein schlaues IDE haben oder eine guten Compiler der einem Fehler und Warnungen anzeigt

----

## OpenGL

Ist eine Grafik API.  
Das heißt sie ermöglicht dem Entwickler Kontrolle über die Grafikkarte um dort,  
vor allem 3D Grafik Berechnungen zu tätigen.  
OpenGL ist eine weit verbreitete Grafik API welche in sehr vielen Bereichen angewendet wird.  
Sie ist 30+ Jahre alt und hat ihr letztes Update 2017 bekommen, wird aber trotzdem noch von vielen verwendet.  
Im Vergleich zu Vulkan und DirectX ist OpenGL das langstamste Grafik API,  
dafür aber das einfachste zu verwenden.  
Es ist sozusagen das Python von Grafik APIs.  

### Extra Info

Außerdem ist OpenGL eine State Machine,  
dass heißt alles was wir im Code machen tut nur den aktuellen Zustand von OpenGL ändern anstatt direkt etwas zu tun.  

### Buffer Objects

OpenGL tut größtenteils zur Datenkommunikation mit der GPU  
verschiedene sogenannte Buffer Objects benutzen.

#### Element Buffer Objects

Sind Buffer Objects,  
die die Reihenfolge abspeichern in der Vertexe gemalt(der Fachbegriff auf English ist **draw**) werden müssen.  

#### Shader Storage Buffer Objects (SSBO)

Sind Buffer Objects,  
welche dazu verwendet werden große Mengen an Daten zwischen GPU und CPU  
unabhängig der Vertexe zu senden.  

#### Vertex Array Objects (VAO)

Sind spezielle Objekte.  
Ihre ganze Funktion ist den aktuellen State des Renderes zu speichern.  
Wenn man einen VAO an den aktuellen Kontext bindet  
tut dieser von allen Konfigurationen für die aktuelle verwendeten Buffer Objects den Überblick behalten.  
So kann man zum Beispiel für ander Szenarien ander VAOs verwenden die für dieselben Buffer verschieden Konfigurationen,  
abspeichern und aufrufbar machen.  

#### Vertex Buffer Objects (VBO)

Sind die wichtigsten Buffer Objects.  
Sie beinhalten die Vertexe welche zu rendern sind und ihre Eigenschaften.  
Ein Vertex ist ein Punkt im 3D Raum.  
Aus mehreren Vertex lassen sich Formen z.B. auch Formen zusammenbauen,  
wenn man Flächen zwischen ihnen malt.  
Eine VBO kann man so definieren:  

```C
// ...
GLuint VBO; // Um einen Zahlenwert zu haben um, der den VBO identifiziert
glGenBuffers(1, VBO); // Macht das Setup des Buffers(generiert ihn)
glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bindet den Buffer zu GL_ARRAY_BUFFER(der Buffer, der mit glDrawArrays gemalt wird)
glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0); // Gibt der GPU das Layout der Daten an
glEnableVertexAttribArray(0); // Macht die Punkte sichtbar/schaltet die Attribute für den VBO an
// ...
```  

Die `glVertexAttribPointer` Methode lohnt sich nochmals genauer anzuschauen.  
Sie hat die folgenden Parameter(in Reihenfolge gelistet):  

- `GLuint index`: Spezifiziert den Index der Daten für die Shader, mehr dazu später  
- `GLint size`: Gibt die Menge an Elementen an, die an diesem Index zu finden sind (zwischen 1-4)  
- `GLEnum type`: Datentyp der hier vorhandenen Daten als GLEnume (Enums funktionieren in C wie in JAVA so ziemlich)  
- `GLBoolean normalize`: Ob die Daten, die hier eingefügt werden, normalisiert werden sollen (Im Intervak\[-1;1\])  
- `GLsizei stride`: Wie viele Bytes man vor muss, bis man beim nächsten Vertex ist  
- `const void* offset`: Das Offset vom Start des Vertexes, ab dem man anfangen muss zu lesen um die richtigen Daten zu bekommen(Wenn der Vertexes mehr als nur ein Datenelement hat)  

**Beim Beispiel von oben könnte man dann so im Vertex Shader auf die Daten zugreifen:**  

```GLSL
// ...
// Um auf die Variablen zuzugreifen (mit dem in und layout keywords)
layout(location = 0) in <datentyp> <name>;
// ...
```

So kann man dann mit `<name>` auf die zugewiesenen Daten im aktuellen Vertex zugreifen.  
Die `location` ist der vorher spezifizierte Index,  
der eigentlich Nutzen dieses ist wirklich nur der,  
die Daten eben in der Grafik Pipeline aufrufbar zu machen.    

### Contexte

Ein OpenGL Kontext ist soziemlich die Aktuelle Instanz bzw. das aktuelle Fenster welches offen ist,  
an welches so ziemlich alle Daten die OpenGL benutzt gebunden werden.  

### OpenGL Shading Language (GLSL)

GLSL ist eine Shadersprache.  
Shader sind Programme, welche auf der Grafikkarte laufen.  
Somit wird GLSL für zum Programmieren auf der Grafikkarte verwendet.  
Es ist in großen Teilen C sehr ähnlich,  
hat aber doch ein paar Unterschiede.  

#### Datentypen

Neben den bekannten Datentypen gibt es noch ein paar extra Datentypen.  
Vor allem für Vektoren und Matrixen,  
da 3D Grafik Berechnungen sehr viele Vektoren und Matrixen beiinhalten.  

##### Für Matrixen

`matNxM`: Eine N mal M Matrix an floats  

**N und M können zwischen 2 und 4 sein**  

##### Für Vektoren

`vecN`: Ein Vektor mit N floats  
`dvecN`: Ein Vektor mit N doubles  
`ivecN`: Ein Vektor mit N integers  
`uvecN`: Ein Vektor mit N unsigned integers  
`bvecN`: Ein Vektor mit N booleans  

**N kann zwischen 1 und 4 sein**  

Mann kann auf die verschiedenen Komponenten mit vec.x/y/z/w zugreifen, je nachdem wie groß der Vektor ist.  

##### Besonderheiten

###### Extra Operator Schreibweisen für Vektoren und Matrizen

In GLSL können Matrix Datentypen als auch Vektordatentypen direkt nativ miteinander mit den standard operationen miteinander verechnet werden.  
Das heißt man kann z.B. dass hier einfach ohne Probleme machen:

```GLSL
mat4 matrix1;
mat4 matrix2;
vec4 vector1;
mat3 incompatible;
// ...
Matrixen werden zugewiesen
// ...

// Man kann so Matrixen und Vektoren zusammenrechnen:
mat4 newMatrix1 = matrix1 * matrix2;
vec4 newVector1 = matrix1 * vector1;
// etc.

// Mathematisch nicht definierte Dinge kann man aber trotzdem nicht machen
mat4 doesNotWork = matrix1 * incompatible; // Geht nicht aufgrund von Matrixrechenregeln
// ...
```

In C währenddessen müsste man nachdem man sich ein `struct` welches einen Vektor darsetellt und diese mit `typedef` als  
Datentyp definiert müsste man stattdessen immer noch manuell so rechnen.  

```C
typedef struct vec4 
{
    float x;
    float y;
    float z;
    float w;
} vec4;

typedef struct mat4
{
    vec4 a;
    vec4 b;
    vec4 c;
    vec4 d;
}

mat4 bsp1;
mat4 bsp2;
vec4 bspv1;
// ...
Vektoren und Matrixen werden zugewiesen
// ...

// Matrix Vektor Multiplikation (bsp1 * bspv1):
vec4 newVec = {
    .x = bsp1.a.x * bspv1.x + bsp1.b.x * bspv1.y + bsp1.c.x * bspv1.z + bsp1.d.x * bspv1.w,
    .y = bsp1.a.x * bspv1.x + bsp1.b.x * bspv1.y + bsp1.c.x * bspv1.z + bsp1.d.x * bspv1.w,
    .x = bsp1.a.x * bspv1.x + bsp1.b.x * bspv1.y + bsp1.c.x * bspv1.z + bsp1.d.x * bspv1.w,
    .x = bsp1.a.x * bspv1.x + bsp1.b.x * bspv1.y + bsp1.c.x * bspv1.z + bsp1.d.x * bspv1.w,
}
```

#### Extensions

GLSL hat sogenannte Extensions.  
Diese kann man sich wie Bibliotheken oder Importe für GLSL vorstellen.  
Den sie erweitern die bereits vorhandene Funktionalität von GLSL Shadern wie  
Bibliotheken es für Programmiersprachen tun.  
Diese wurden allerding nicht in meinem Projekt verwendet.  

#### Qualifier für Variablen

##### in

Der `in` Qualifier deklariert, dass eine Variable vom vorherigen Shader definiert wird.  
Wird verwendet um Variablen um eine Variable vom vorherigen Shader zu "empfangen".  

##### out

Der `out` Qualifier deklariert, dass eine Variable zum nächsten Shader in der Pipiline geschickt werden soll.  
Wird verwendet um Variablen von einem Shader zum nächsten zu schicken.  

#### layout

Das `layout` keyword wird dazu verwendet um den Ort im Shader zu spezifizieren,  
auf dem die CPU davor die Daten für die Grafikkarte abgespeichert hat.  
So werden zum Beispiel Daten, die vom VBO kommen mit `layout(location=<index>)` "gefunden" und  
Daten aus einem SSBO mit `layout(std430, binding=<index>)` "gefunden".

##### uniform

`uniform` Deklariert eine Uniform Variable.  
Uniforms sind Variablen, die von der CPU aus gesetzt werden können und dann in der GPU als read-only verwendbar sind.

### Pipeline

Die Grafik Pipeline kann man sich ein bisschen wie ein Fließband,  
oder eben eine Pipeline vorstellen,  
wo eins nach dem anderen unterschiedlicher Operationen auf den Daten in Reihenfolge gemacht werden.  

Ein solche Pipeline kann viele wie auch wenige Schritte habe,  
jenachdem wie kompliziert die zu rendernde Szene ist.  

Ich werde nur auf die Stufen eingehen die es in meinem Projekt hat.  

#### Vertex Shader

Der Vertex Shader ist der erste welcher nach dem Aufruf eines draw calls ausgeführt wird(wie in meinem Program z.B `glDrawArrays()`).  
Er bekommt seine Daten direkt von der CPU über die Buffer Objects.  
Der Vertex Shader läuft einmal für jeden Vertex der zu malen ist, also sehr, da die meisten Szenen hunderte, tausende oder noch mehr Vertecies haben.  
Da er so oft ausgeführt wird und extrem parallelisierbar ist,  
wird er und alle anderen Shader auf der GPU ausgeführt.  
Die Hauptaufgabe des Vertex Shaders ist es jedem Vertex eine Position im sogenannten OpenGL Viewport zu geben.  
Dieser Viewport ist einfach nur ein Bereich im Fenster(meist das ganze Fenster) für ein \[1;-1\] normalisiertes Koordinatensystem auf welchem die gerenderten Pixel dann angezeigt werden.  
Dieser Bereich wird oft auch als NDC(Normalised Device Coordinates) bezeichnet,  
und alle Pixel welche außerhalb dieses Bereiches fallen werden nicht gemalt.  
Daher findet im Vertex Shader auch die perspektivische Projektion der Szene, als auch die Transformation der Objekte in der Szene statt.  
Um die Position eines Vertexes anzugeben muss die spezielle Variable `glPosition` für diesen Vertex als `vec4` zugewiesen werden.  
Der Shader gibt nachdem er gelaufen ist 

*Transformationen sind hier wirklich nur die Transformationen, welche man aus der Matrixrechnung kennt um Koordinaten durch den Raum zu schieben und zusammenhängende Strukturen zu drehen oder skalierent.  
In meinem Projekt gibt es nur Transformatien der Position von Objekten doch aufgrund deren Simplizität  
findet dies nicht in Matrixen statt sonderen direkt beim Assignement von `gl_Position`*

##### Perspektivische Projektion und wie man eine Matrix baut die diese anwendet


