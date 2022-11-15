#pragma once
#include "Applications/Application.h"
#include "ShaderPrograms/Program_passThrough.h"

class App_LineBenchmark :public Application {

	float vertex[2][3];	// x,y,z
	float color[2][4];	// r,g,b,a

		int frame_width,frame_height;
		struct JumpingPoint
		{
			float dx;
			float dy;
			float x;
			float y;
		} a,b;
	Program_passThrough prog;
public:
	App_LineBenchmark(int w, int h);
	void program_step();

};
