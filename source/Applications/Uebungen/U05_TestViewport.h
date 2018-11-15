#pragma once
#include "Applications/Application.h"
#include "ShaderPrograms/Program_passThrough.h"
#include "ShaderPrograms/Program_testVertex.h"

class App_TestViewport :public Application {

	Program_passThrough prog; // TODO: change program
	int frame_width,frame_height;
	const 	float vertexPosition_TestTriangle[6*3]={
		-1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		-1.0f,-1.0f, 0.0f,  1.0f,-1.0f, 0.0f,  1.0f, 1.0f, 0.0f};
	const float vertexColor_TestTriangle[6*4] ={1,0,0,1, 0,0,1,1, 0,1,0,1, 1,1,1,1, 0,1,1,1, 1,0,1,1};
public:
	App_TestViewport(int w, int h);
	void program_step();

};

