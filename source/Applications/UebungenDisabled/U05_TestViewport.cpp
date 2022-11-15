#include "U05_TestViewport.h"
//------------------------------------------------------------------------------
App_TestViewport::App_TestViewport(int w, int h)
	:Application(w,h)
{
	frame_width = w;
	frame_height = h;
	context.clearColor = CGVec4(0.0f, 0.0f, 0.0f, 1.0f);
}
//------------------------------------------------------------------------------
void App_TestViewport::program_step()
{
	// clear the screen
	context.clear(CG_COLOR_BUFFER_BIT);

	// set the vertex attribute pointers
	context.vertexAttributePointer.position = vertexPosition_TestTriangle;
	context.vertexAttributePointer.color = vertexColor_TestTriangle;

	context.useProgram(prog);

	// ...
	context.viewport.width = frame_width/2;
	context.viewport.height = frame_height/2;
	for(int i=0;i<2;i++){
		for(int j =0;j<2;j++ ){
			context.viewport.bottom_left.x=i*context.viewport.width;
			context.viewport.bottom_left.y=j*context.viewport.height;
			context.drawArrays(CG_TRIANGLES, 0, 6);
		}

	} // 6 vertices for 2 triangle.

//	context.drawArrays(CG_TRIANGLES, 0, 6);
}
