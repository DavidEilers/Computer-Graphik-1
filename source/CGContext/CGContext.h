#pragma once
#include "CGFrameBuffer.h" // Our context needs a framebuffer.
#include "CGVertexOperations.h"
#include "CGPrimitiveOperations.h"
#include "CGProgramInterface.h"
#include "CGFragmentOperations.h"
#include "CGRasterizer.h"
#include "CGMatrix.h"
#include "CGTexture.h"
#include "CGProgram.h"
#include "CGRectangle.h"
#include <math.h>
#include <string.h>
#include "CG.h"            // This is the internal global header.

//---------------------------------------------------------------------------

class CGContext 
{
public:
	CGFrameBuffer m_frameBuffer;
private:
	/// Framebuffer.

	CGFragmentOperations m_fragmentOps;
	CGPointRasterizer m_pointRasterizer;
	CGStupidLineRasterizer m_stupidLineRasterizer;
	CGBresenhamLineRasterizer m_bresenhamLineRasterizer;
	CGTriangleRasterizer m_triangleRasterizer;
	CGPrimitiveOperations m_primitiveOps;
	CGVertexOperations m_vertexOps;



public:


	/// Internal state: vertex attribute pointers.

	struct CGAttributePointer
	{
		const float* position;
		const float* normal;
		const float* color;
		const float* texcoord;
		CGAttributePointer()
		{
			position=normal=color=texcoord=NULL;
		}
	}
	vertexAttributePointer;

	const CGProgram* currentProgram;


	/// Internal state: clear color.
	CGVec4 clearColor;
	struct CGCapabilities
	{
		bool blending;
		bool depthTest;
		bool backFaceCulling;
		bool bresenham;
		CGCapabilities()
		{
			blending = depthTest = backFaceCulling = bresenham = false;
		}
	}
	capabilities;

	CGPolygonMode polygonMode;
	CGRectangle viewport;

public:
	/// Context constructor.
	CGContext(int width, int height);

	/// Context destructor.
	~CGContext();


	/// Clears the bitwise specified buffers by the current clear value (e.g. m_cgClearColor)
	void clear(int bitfield);

	/// Render primitives with given mode. Note: this calls the pipeline directly (in our implementation).
	void drawArrays(CGPrimitiveType mode, int first, unsigned int count);


	/// Set the shader programs.
	void useProgram(const CGProgram& prog);

	/// Reads all data from the current color buffer into the provided memory
	void readPixels(unsigned char* data) const;

private:
};

//---------------------------------------------------------------------------

