<!-- (C) Sebastian Fiault 2026 -->

# Notes

## USEFUL FORMULAS

### Gravity

Gravitational force:  
$$\; F_g = G \frac{m_1 m_2}{d^{2}} \; \big| \; µ = G m_1$$  
$$\leftrightarrow F_g = \frac{µ m_2}{d^{2}}$$  
Standard gravitational parameter:  
$$µ = Gm$$

### Orbital Velocity

Orbital velocity when orbiting object mass is negligible:  
$$V_{rel} = \sqrt{ \frac{GM}{R} }$$  
When orbiting object mass is not negligible:  
$$V_{rel} = \sqrt{ \frac{G(M + m}{R} }$$  

-----

## Useful data type information:

### Float32

Max positive float32 number: $\text{~} 3.4 \cdot 10^{38}$  
Min positive float32 number: $\text{~}3.4 \cdot 10^{-45}$  
Remark: Prefer to use floats as most GPUs are optimized for 32 bit floating point arithmetic  

### Double64

Max positive number: $\text{~}1.8 \cdot 10^{308}$  
Min positive double64 number: $\text{~}2.2 \cdot 10^{-308}$  
Remarks: Double64 runs much slower on modern GPUs than float32!  

-----

## Useful SI prefixes:

| Prefix     | In Powers of 10 |
| ---------- | --------------- |
| Quetta (Q) | $10^{30}$       |
| Ronna (R)  | $10^{27}$       |
| Yotta (Y)  | $10^{24}$       |
| Zetta (Z)  | $10^{21}$       |
| Exa (E)    | $10^{18}$       |
| Peta (P)   | $10^{15}$       |
| Tera (T)   | $10^{12}$       |
| Giga (G)   | $10^{9}$        |
| Mega (M)   | $10^{6}$        |
| Kilo (K)   | $10^{3}$        |
| -          | $10^{0}$        |
| Milli (m)  | $10^{-3}$       |
| Micro (µ)  | $10^{-6}$       |
| Nano (n)   | $10^{-9}$       |
| Pico (p)   | $10^{-12}$      |

-----

## Useful Values

|                   Name                        |                                         Value                                                       |
| --------------------------------------------- | --------------------------------------------------------------------------------------------------- |
| Age of universe                               | $\text{~} 14 \cdot 10^{9}$ years = $\text{~}4.41806 \cdot 10^{17}$ Seconds (s)                      |
| Luminosity least luminous red dwarf           | $\text{~} 3 \cdot 10^{-4}$ solar luminosities (L☉) = ~11.25 Ronnalumen                             |
| Luminosity most luminous star (R136a1)        | $\text{~} 7.244 \cdot 10^{6}$ solar luminosities (L☉) = $\text{~} 2.7165 \cdot 10^{5} Quettalumen$ |
| Mass of largest observed black hole (IC 1101) | $\text{~} 1 \cdot 10^{11}$ solar masses (M☉) = $\text{~}1.989 \cdot 10^{14}$ Quettagrams           |
| Mass of most massive observed star (R136a1)   | $\text{~} 291$ solar masses = $\text{~} 5,76471 \cdot 10^{5}$ Quettagrams                           |
| Minimal star mass                             | $\text{~} 6 \cdot 10^{-2}$ solar masses (M☉) = $\text{~}0.11934 \cdot 10^3$ Quettagrams (Qq)       |
| Size of Milky-way                             | $~9 Kiloparsec (kpc) = $3.0857 \cdot 10^{19}$ Meters (m)                                            |
| Time to Andromeda galaxy collision            | $\text{~} 4.5 \cdot 10^6$ years = $1.42 \cdot 10^{17}$ seconds (s)                                  |

See constants.h for more  

-----

## Useful Units

|             Unit          |         Unit converted to Metric Unit         |
| ------------------------- | --------------------------------------------- |
| 1 Arcsecond (arcsec or ") | $\frac{1}{3600}°$                             |
| 1 Astronomical Unit (AU)  | $\text{~} 1.495979 \cdot 10^{11}$ Meters (m)  | 
| 1 Lightyear (ly)          | $\text{~} 9.4607 \cdot 10^{15}$ Meters (m)    |
| 1 Parsec (pc)             | $\text{~} 3.0857 \cdot 10^{16}$ Meters (m)    |
| 1 Solar luminosity (L☉)  | $\text{~} 3.75 \cdot 10^{28} Lumen (lm)$      |
| 1 Solar mass (M☉)        | $\text{~} 1.989 \cdot 10^{30}$ Kilograms (kg) |
| 1 Year (y)                | $3.15506 \cdot 10^{7}$ Seconds (s)            |

-----

## Units used

### Notes  

Use only SI Units or we loose computation on conversions!  
- Units that are stored in integers need to have 1[Unit] be of a step size so that it is precise enough for accurate simulation,  
  yet also be able to span to galactic scales.  
  Meaning int stored units need to have 1[Unit] be at the lower end of the range of numbers the unit encompasses.
- Units stored in floats need to have 1[Unit] be exactly at the middle of the range of number the unit will span for maximum precision  

### 32 bit precision

#### Int

- Distances: Terameters (Tm)

#### Float

- Luminosity: Quettalumen
- Standard gravitational parameter: Terameters cubed per petasecond squared ($\frac{\text{Tm}^3}{\text{Ps}^2}$)
- Time: Petaseconds (Ps)  
- Velocity: Kilometers per Second (km/s)  
- Weight: $10^{7}$ Quettagrams (Qq)  

### 64 bit precision

#### Int

- Distances: Kilometers (Km)

#### Double

- Luminosity: Quettalumen
- Standard gravitational parameter: Kilometers cubed per petasecond squared ($\frac{\text{Km}^3}{\text{Ps}^2}$)
- Time: Petaseconds (Ps)
- Velocity: Kilometers per second (km/s)
- Weight: $10^7$ Quettagrams (Qg)