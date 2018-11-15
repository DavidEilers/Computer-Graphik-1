#include "CGContext/CGContext.h"
#include "Utilities/CG1Helper/CG1Helper.h"    // We need to speak with the helper during initialization.
#include <iostream>       // If we need to print something to the console...
#include "CGContext/CG.h"

//------------------------------------------------------------------------------
CGContext::CGContext(int width, int height)
	: m_frameBuffer(),
	  m_fragmentOps(*this,m_frameBuffer),
	  m_pointRasterizer(*this,m_fragmentOps),
	  m_stupidLineRasterizer(*this,m_fragmentOps),
	  m_bresenhamLineRasterizer(*this,m_fragmentOps),
	  m_triangleRasterizer(*this,m_fragmentOps),
	  m_primitiveOps(*this,
			 m_pointRasterizer,
			 m_stupidLineRasterizer,
			 m_bresenhamLineRasterizer,
			 m_triangleRasterizer),
	  m_vertexOps(*this,m_primitiveOps),
	  viewport(0,0,width,height)
{


	// Default all internal states.

	clearColor = CGVec4(0,0,0,1);

	polygonMode = CG_FILL;

	// allocate the framebuffer
	m_frameBuffer.allocate(width, height);
}

//------------------------------------------------------------------------------
CGContext::~CGContext()
{
}
//------------------------------------------------------------------------------
void CGContext::clear(int bitfield)
{
	if(bitfield & CG_COLOR_BUFFER_BIT)
		m_frameBuffer.colorBuffer.clear(clearColor);

	// ...
}
//------------------------------------------------------------------------------
void CGContext::drawArrays(CGPrimitiveType mode, int first, unsigned int count)
{
	m_primitiveOps.setPrimitiveMode(mode);
	int last = first + count;
	for(int i = first ; i< last;i++)
		m_vertexOps.pullVertex(i);
}

//------------------------------------------------------------------------------
void CGContext::useProgram(const CGProgram& prog)
{
	currentProgram = &prog;
}

//------------------------------------------------------------------------------
void CGContext::readPixels(unsigned char *data) const
{
	// ...
}
