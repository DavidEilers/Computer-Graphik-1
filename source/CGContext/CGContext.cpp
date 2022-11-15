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
	if(bitfield & CG_DEPTH_BUFFER_BIT)
		m_frameBuffer.depthBuffer.clear();

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
void CGContext::readPixels(unsigned char **data) const
{
	int width = m_frameBuffer.getWidth();
	int height = m_frameBuffer.getHeight();
	unsigned char * buffer = m_frameBuffer.colorBuffer.getDataPointer();
	*data = (unsigned char *) malloc(sizeof(unsigned char)*4*width*height );
	//memcpy(data, buffer, sizeof(unsigned char)*4*width*height);
/*	for(int i=0;i<width*height*4;i++){
		(*data)[i]=buffer[i];
	}*/
//	printf("Width: %d\n Height:%d\n",width,height);
	for(int y=0;y<height;y++){
		for(int x=0;x<width;x++){
			int invY=height-y-1;
			int invX=width-x-1;
			memcpy(&((*data)[(y*width+x)*4]),&(buffer[(width*(invY)+invX-1)*4]),sizeof(unsigned char)*4);
			//(*data)[(y*width+x)*4+3]=0.0f;
		}
	}
	// ...
}
