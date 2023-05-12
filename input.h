#pragma once

#include <unordered_map>
#include "pgr.h"
#include "config.h"

class Input
{
public:
	static void initialize(Config* config);

	static void processKeyPressed(unsigned char keyPressed);
	static void processKeyReleased(unsigned char keyReleased);

	static void processSpecialKeyPressed(int keyPressed);
	static void processSpecialKeyReleased(int keyReleased);

	static void processPassiveMouseMotion(int mouseX, int mouseY);

	static bool getKeyPressed(unsigned char keyPressed);
	static bool getSpecialKeyPressed(unsigned char keyPressed);
};

