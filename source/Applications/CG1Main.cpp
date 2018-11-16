// Standard includes.
#include <stdlib.h>         // for rand()
#include <time.h>           // for time() in srand()
// Our includes.
#include "Utilities/CG1Helper/CG1Helper.h"
//---------------------------------------------------------------------------
// Defines, globals, etc.


#define FRAME_WIDTH  160   // Framebuffer width.
#define FRAME_HEIGHT 100   // Framebuffer height.
#define FRAME_SCALE  5     // Integer scaling factors (zoom).


#include "Uebungen/U01_JumpingPoint.h"
#include "Uebungen/U02_LineBenchmark.h"
#include "Uebungen/U02_U03_TestBresenham.h"
#include "Uebungen/U03_U04_TestRotatingTriangle.h"
#include "Uebungen/U05_TestViewport.h"
#include "Uebungen/U05_U06_TestProjection.h"
#include "Uebungen/U07_U08_HappyHolidays.h"
#include "Uebungen/U08_AwesomeTriangle.h"
#include "Uebungen/U09_Lighting.h"
#include "Uebungen/U10_Texturing.h"
#include "Project/Project_Chess.h"

int main(int /*argc*/,
	 char ** /*argv*/)
{
	srand((unsigned int)time(NULL)); // initialize seed for pseudo random number generator

	Application* app = (Application*) new App_LineBenchmark(FRAME_WIDTH,FRAME_HEIGHT);

	if (CG1Helper::initApplication(app, FRAME_SCALE)) {
		CG1Helper::runApplication();
	} else {
		fprintf(stderr,"failed to intialize CG1Helper!\n");
	}

	delete app;

	return 0;
}
//---------------------------------------------------------------------------
