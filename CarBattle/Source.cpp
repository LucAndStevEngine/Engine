#include "WindowControl.h"
#include "Game.h"

int main()
{
	WindowControl* windowControl = new WindowControl();
	windowControl->InitWindow("Car Battle", 800, 600, 4, 0, false, new Game());
	
	return 0;
}
