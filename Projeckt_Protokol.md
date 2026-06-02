# Galaxienkollision - Sebastian Fiault

## Thema meines Projekts

Ich werde in C eine Simulation von 2 Galaxien,  
die miteinander kollidieren erstellen.  
C ist sehr geeignet dafür das es eine native kompilierte Sprache ist und somit sehr schnell (sogar schneller als C++).  
Des weiteren ist es das Ziel dies Simulation bestmöglich zu optimierent um beindruckendere Ergebnisse zu erzielen.  
Was auch heißt für 3D und physikalische Berechnungen die Grafikkarte durch das OpenGL- oder evtl. später Vulkan API für schnellere massive paralelle Berechnungen zu verwenden.  
Das evtl. wird auch heißen eine stark C ähnliche Shadersprache zu lernen  um Code für die Grafikkarte zu schreiben.  
Falls das Projekt soweit fortschreiten sollte das es stark optimiert auf der Grafikkarte läuft und das Vulkan API verwendet (Vulkan ist schneller als OpenGL aber wesentlich komplexer) wäre es das finale Ziel das Projekt so zu erweitern, das für die Berechnungen Clustercomputing verwendet werden kann.  
In diesem Fall das eine Host Machine auf der das Program läuft die Rechenarbeit auf mehreren Client Machinen aufteilt für mehr Leistung.  
Außerdem soll dieses Projekt in als auch out source gut dokumentiert und klar sein das etwas ist was ich lernen möchte.  

## Soweit bearbeitete Inhalte

### Astrophysik Verständis für Physik Simulation

Ich habe viel Zeit mit Recherche verbracht um mir der Größenordnungen die ich verwenden muss bewusst zu werden und mir viel Verständis über Astrophyisk im allgemeinen angeeignet.  
Und somit auch viele Formeln mir angeeignet.
Viele Notizen die ich mir hierzu gemacht habe sind in der Datei Notes\.md zu finden.

### C Grundlagen

Ich habe mir die Grundlagen der C Programmiersprache angeeignet und kann diese nun selbstbewusst und auch teils über die Grundlagen hinaus verwenden.  

### Low Level Verständis von Computern und Code

Ich habe mein bestehendes Verständis von den Low Level Abläufen von Computern erweitert und kann somit gut ermitteln wie ich Dinge programmieren muss damit sie möglichst effizient sind.

#### Beispiele hierfür

- Klares verständis wie Daten im RAM abgespeicher sind, wie die verschiedenen Bereiche im RAM aussehen und grob wie die Daten vom RAM über die Datenbusse zur CPU oder GPU kommen
- Ich habe Verständiss von der physikalischen Architektur von CPUs und wie diese welche Operation ausführen und wie aufwendig welche Operationen sind. Z.B: weiß ich das selbst mit extra FPUs Divisionen immer noch teurer sind oder das Wurzeln kein leichtes für einen Computer sind
- Ich bin mir der fundamentalen Unterschiede von GPU und CPU Microarchitektur grob bewusst und habe ein grobes Gefühl wie man für beide jeweils optimiert programmieren muss
- Ich habe grobes Verständnis wie der Prozess des compilens und linkens abläuft

### Mathematik für 3D engines

Work in progress

### *Verständis Kommunikation zwischen PCs*

Ich kenne die unterschiede zwischen verschiedenen Internetprotokollen und weiß wie Daten über das Internet fließen

### Resourcen die zum lernen verwendet wurden:

- [Geeks For Geeks Artikel über C, als auch für manche Gleichungen](https://www.geeksforgeeks.org)
- [W3Schools Artikel über C, vor allem Datei Interaktionen](https://www.w3schools.com)
- [Offizielles CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- [learnopengl.com](https://learnopengl.com)
- Das Buch "Modern C" von Jens Guestedt

## Fortschritt soweit

*Siehe Progress_Log.md*  

Dieses Protokoll ist nicht vollständig präzise und teils lückenhaft aufgrund Vergessens des Protokolierens.  
Jedoch habe ich mich meist für mehrere Wochen grob mit dem selben Thema auseinandergesetz also ist nicht allzu viel verloren.  

### Vor dem 3.3

Ich habe mir bereits die Grundlagen der C Programmierung ca. 2/3 Wochen vor Projektbeginn grob zuhause angeschaut und bin somit halbwegs vorbereitet eingestiegen.

### Um den 3.5

Habe ich mich mit den Standard Bibliotheken von C auseinandergesetz und auch über Represäntationen von Daten im RAM gelernt z.b. IEEE 754 für 32 bit floats

### Um den 3.9

Habe mehr C features gelernt und begonnen die Matrix/Vektor Mathematik die ich benötigen werde zu lernen.

### 3.10

Habe mir mehr Standardbibliotheken und mehr Daten in RAM Verständnis angeeignet.

### Um den 3.14

Habe gelernt wie man mit der Standardbibliothek stdlib Daten in den Heap ablegen kann, habe gelernt Pointer zu verwended und habe gelernt das Daten standardmäßig im Stack sind.  
Habe über Heap und Stack gelernt, dass der Stack keine dynamische Allokation zulässt und das Stack Frames aufgelöst werden wenn die Methode auf dem Call Stack einen Frame runter geht.  
Habe gelernt das der Stack schneller ist als der Heap aber nicht für große und dynamisch große Datenmengen ist.

### Undprotokolierte Lücke 3.14 - 4.01

Habe hauptsächlich die Logik ausgetüftelt um elliptische Galaxien zu generieren.  
Das hat Heap Allokation und Wisen über Wahrscheinlichkeitsverteilung, Exponentailfunktion, Astrophyisk und Vector/3D Mathematik erfordert.  

### 4.01

Habe mich mehr mit Formeln auseinandergesetz die ich verwenden werde

### 4.02 - ~4.09

Habe viel über 3D engine Mathematik recherchiert

### 4.10 - 4.17

Habe begonnen mich mit CMake auseinanderzusetzen, vor allem um automatisierte Tests mit CTest benutzen zu können.
Habe CMake Dokumentation und Tutorials gelesen/bearbeitet.

### 4.18

Habe ~3.5h damit verbracht portable installation von CMake, C compiler (gcc - Gnu C Compiler) und build tool (Ninja) zu erstellen und testen, da Versionen auf Schul PCs veraltet und schwer benutzbar sind.  
Habe kleines PowerShell Script das diese portablen installationen verwendet um CMake projekte zu compilen erstellt.  
Habe Installationen und script auf USB-Stick geladen.

### 4.19

Habe migration auf CMake finalisiert und weiter das CMake tutorial bearbeitet.  
Habe ca. 1h dieses Zwischenstandsprotokol geschrieben.

### 4.20 - 4.5

Noch mehr über CMake gelernt.
Um OpenGL Bibliotheken compilen und linken zu können.
Zum testen ob linken und compilen erflogreich war absolute OpenGL Grundlagen gelernt.

### 4.5 - 23.5

Einarbeitung in OpenGL Grafikpipeline.
Gelernt was ein OpenGL- Context, Vertex Buffer, Index Buffer, Vertex Array Buffer,
Shader und im allgemeinen die notwendigen Funktion für die Verwendung von OpenGL erlernt.

### 23.5 - 30.5

Weiter OpenGL gelernt. Über GLSL gelernt, Texture Mapping und uniforms.

### 31.5 - 

