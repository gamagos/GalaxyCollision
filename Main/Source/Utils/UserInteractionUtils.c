// (C) Sebastian Fiault 2026
#include <stdio.h>

#include "../GalaxyCollision.h"

void printKeybinds()
{
	printf_s("\n");
	printf_s("###############################################################\n");
	printf_s("#####################     Keybindings     #####################\n");
	printf_s("###############################################################\n");
	printf_s("##  Decrease radius:                  W                      ##\n");
	printf_s("##  Increase radius:                  S                      ##\n");
	printf_s("##  Increase camera height:           Space                  ##\n");
	printf_s("##  Decrease camera height:           Right Ctrl; Left Ctrl  ##\n");
	printf_s("##  Increase camera speed:            Q                      ##\n");
	printf_s("##  Decrease camera speed:            E                      ##\n");
	printf_s("##  Increase camera orbiting speed:   I                      ##\n");
	printf_s("##  Decrease camera orbiting speed:   O                      ##\n");
	printf_s("##  Rotate camera clockwise:          A                      ##\n");
	printf_s("##  Rotate camera counter-clockwise:  D                      ##\n");
	printf_s("##  Increase particle mass:           X                      ##\n");
	printf_s("##  Decrease particle mass:           C                      ##\n");
	printf_s("##  Increase drag:                    F                      ##\n");
	printf_s("##  Decrease drag:                    G                      ##\n");
	printf_s("##  Increase velocity cap:            1                      ##\n");
	printf_s("##  Decrease velocity cap:            2                      ##\n");
	printf_s("###############################################################\n\n");
}
