#include "U02_LineBenchmark.h"
#include "Utilities/CG1Helper/CG1Helper.h"
//------------------------------------------------------------------------------
App_LineBenchmark::App_LineBenchmark(int w, int h)
	:Application(w,h)
{
	// ...

		frame_width = w;
		frame_height = h;

		// prepare the data for the simulation
		a={0.0f, 0.0f, 20.0f, 0.0f};
		b={(float)(w-1), (float)(h-1), 70.0f, 50.0f};

		// prepare vertex array for point a
		color[0][0]=1.0f;
		color[0][1]=1.0f;
		color[0][2]=1.0f;
		color[0][3]=1.0f;

		vertex[0][0]=0.0f;
		vertex[0][1]=0.0f;
		vertex[0][2]=0.0f;

		// prepare vertex array for point b
		color[1][0]=0.0f;
		color[1][1]=1.0f;
		color[1][2]=0.0f;
		color[1][3]=1.0f;

		vertex[1][0]=w-1.0f;
		vertex[1][1]=h-1.0f;
		vertex[1][2]=0.0f;

		// set the background color
		context.capabilities.bresenham=true;
		context.clearColor = CGVec4(0.0f, 0.0f, 0.0f, 1.0f); // TODO: change this later
}
//------------------------------------------------------------------------------
void App_LineBenchmark::program_step()
{

	// clear the screen
	context.clear(CG_COLOR_BUFFER_BIT);

	// set the vertex attribute pointers
	context.vertexAttributePointer.position = &vertex[0][0];
	context.vertexAttributePointer.color =&color[0][0];

	// select the program used
	context.useProgram(prog);

	// the draw call
	for(int i=0;i<1000;i++){
	context.drawArrays(CG_LINES, 0, 2);
}
}
