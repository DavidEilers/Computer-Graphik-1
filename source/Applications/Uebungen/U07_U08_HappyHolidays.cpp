#include "U07_U08_HappyHolidays.h"
#include "CGContext/CGMath.h"
#include "Utilities/CG1Helper/CG1Helper.h"
//------------------------------------------------------------------------------
App_HappyHolidays::App_HappyHolidays(int w, int h)
	: Application (w,h), deers(NUM_DEERS), snowflakes(NUM_SNOWFLAKES)
{
	context.clearColor = CGVec4(0.1f, 0.1f, 0.4f, 1.0f);
	// enable the depth test
	// ...
	context.capabilities.bresenham = true;
	context.capabilities.backFaceCulling=true;
}
//------------------------------------------------------------------------------
void App_HappyHolidays::program_step()
{
	deers.updateAnimation(CG1Helper::getTimeDelta());
	snowflakes.updateAnimation(CG1Helper::getTimeDelta());

	context.useProgram(prog);

	if (CG1Helper::isKeyReleased('w'))
		context.polygonMode = (context.polygonMode == CG_FILL?CG_LINE:CG_FILL);


	context.clear(CG_COLOR_BUFFER_BIT);

	prog.uniform.projectionMatrix = CGMatrix4x4::getFrustum(-0.062132f, 0.062132f, -0.041421f, 0.041421f, 0.1f, 50.0f);

	//CGMatrix4x4 view = CGMatrix4x4::getTranslationMatrix(0.0f, -5.0, -25.0f); // TODO: REMOVE THIS later

	// Uebung 07, Aufgabe 4a)
	// Bird's-eye view
	const float dt=CG1Helper::getTimeDelta();
	if (CG1Helper::isKeyPressed(GLFW_KEY_UP)) polar += 45.0f * dt;
	if (CG1Helper::isKeyPressed(GLFW_KEY_DOWN)) polar -= 45.0f * dt;
	if (CG1Helper::isKeyPressed(GLFW_KEY_LEFT)) azimuth += 45.0f * dt;
	if (CG1Helper::isKeyPressed(GLFW_KEY_RIGHT)) azimuth -= 45.0f * dt;
	// keep polar and azimuth within the limitations
	if(polar<-90.0f){
		polar+=180.0f;
	}else if(polar>90.0f){
		polar-=180.0f;
	}

	if(azimuth<0.0f){
		azimuth+=360.0f;
	}else if(azimuth>360.0f){
		azimuth-=360.0f;
	}
	//CGMatrix4x4 view =

	// Uebung 07, Aufgabe 4c)
	// Camera rotating around center on r = 15 circle.
	float anim = CG1Helper::getTime() * 0.3f;
	CGVec4 eye( cos(anim) * 15.0f, 15.0f, sin(anim) * 15.0f);
	CGMatrix4x4 view = CGMatrix4x4::getLookAt(eye,CGVec4(0.0f, 2.0f, 0.0f), CGVec4(0.0f, 1.0f, 0.0f));

	drawGround(view);
	for (int i = 0; i < 10; i++) {
		float posX = float(i % 7) / 6.0f * 16.0f - 8.0f; // [0,6]->[-8,+8]
		float posZ = float(i % 4) / 3.0f * 16.0f - 8.0f; // [0,3]->[-8,+8]
		CGMatrix4x4 translationTree = CGMatrix4x4::getTranslationMatrix(posX, 0.0f, posZ);
		int count=6;
		for(int i=0;i<count;i++){
			drawTree(view, translationTree*CGMatrix4x4::getRotationMatrixY((180.0f/count)*i));
		}
	}
	snowflakes.render(context, prog.uniform.modelViewMatrix, view);
	renderDeers(view);

	// ...
}
//------------------------------------------------------------------------------
void App_HappyHolidays::drawTree(const CGMatrix4x4 &view, const CGMatrix4x4 translation)
{
	CGMatrix4x4 modelViewTrunk, modelViewLeaf[3], rotation;

	// ...
	modelViewTrunk =view*translation;
	rotation = CGMatrix4x4::getRotationMatrixZ(225.0f);

	modelViewLeaf[0] =modelViewTrunk*
	CGMatrix4x4::getTranslationMatrix(0.0f,3.5f,0.0f)*
	rotation*
	CGMatrix4x4::getScaleMatrix(0.5f,0.5f,0.5f);

	modelViewLeaf[1] =modelViewTrunk*
	CGMatrix4x4::getTranslationMatrix(0.0f,3.0f,0.0f)*
	rotation*
	CGMatrix4x4::getScaleMatrix(0.8f,0.8f,0.8f);

	modelViewLeaf[2] =modelViewTrunk*
	CGMatrix4x4::getTranslationMatrix(0.0f,2.5f,0.0f)*
	rotation;

	// draw TRUNK
	prog.uniform.modelViewMatrix = modelViewTrunk;
	context.vertexAttributePointer.position = verticesTrunk;
	context.vertexAttributePointer.color = colorsTrunk;
	context.drawArrays(CG_TRIANGLES, 0, 3);

	// draw LEAVES
	context.vertexAttributePointer.position = verticesLeafs;
	context.vertexAttributePointer.color = colorsLeafs;
	for(int i = 0; i < 3; i++) {
		prog.uniform.modelViewMatrix = modelViewLeaf[i];
		context.drawArrays(CG_TRIANGLES, 0, 3);
	}
}
//------------------------------------------------------------------------------
void App_HappyHolidays::drawGround(const CGMatrix4x4 &view)
{
	prog.uniform.modelViewMatrix = view;
	context.vertexAttributePointer.position = verticesGround;
	context.vertexAttributePointer.color = colorsGround;
	context.drawArrays(CG_TRIANGLES, 0, 6);
}
//------------------------------------------------------------------------------
void App_HappyHolidays::renderDeers(const CGMatrix4x4& view)
{
	for (size_t i = 0; i < deers.deer.size(); i++) {
		CGMatrix4x4 modelViewDeer = view * deers.deer[i].getModelMatrix();
		prog.uniform.modelViewMatrix = modelViewDeer;
		deers.deer[i].render(context);
	}
}
//------------------------------------------------------------------------------
void App_HappyHolidays::drawColoredRect(float r, float g, float b, float a)
{
	static const float vertexPositions[3 * 6] = {
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
	};
	float colors[4 * 6];
	for (int i = 0; i < 6; i++) {
		colors[4 * i] = r; colors[4 * i + 1] = g; colors[4 * i + 2] = b; colors[4 * i + 3] = a;
	}
	context.vertexAttributePointer.position = vertexPositions;
	context.vertexAttributePointer.color = colors;
	context.drawArrays(CG_TRIANGLES, 0, 6);
	context.vertexAttributePointer.position = NULL;
	context.vertexAttributePointer.color = NULL;
}
