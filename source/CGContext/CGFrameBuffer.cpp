#include "CGFrameBuffer.h"
#include <stdlib.h> // for malloc
#include <string.h> // for memcpy
#include "CG.h"
#include "CGVector.h"
//------------------------------------------------------------------------------
// CGFRAMEBUFFER
//------------------------------------------------------------------------------
bool CGFrameBuffer::allocate(int width, int height)
{
	bool retval = colorBuffer.reserve(width,height);
	// also reserve a depth buffer
	// ...
	depthBuffer.reserve(width,height);
	return retval;
}
//------------------------------------------------------------------------------
int CGFrameBuffer::getWidth() const
{
	return colorBuffer.getWidth();
}
//------------------------------------------------------------------------------
int CGFrameBuffer::getHeight() const
{
	return colorBuffer.getHeight();
}


//------------------------------------------------------------------------------
// CGFRAMEBUFFER::CGBUFFER4UB
//------------------------------------------------------------------------------
CGFrameBuffer::CGBuffer4UB::CGBuffer4UB()
{
	pBufferData = NULL;
}
//------------------------------------------------------------------------------
CGFrameBuffer::CGBuffer4UB::~CGBuffer4UB()
{
	free(pBufferData);
}
//------------------------------------------------------------------------------
bool CGFrameBuffer::CGBuffer4UB::reserve(int width, int height)
{
	this->width = width;
	this->height = height;

	// allocate the memory for the buffer
	// ...
	pBufferData = (unsigned char *) malloc(sizeof(unsigned char)*4*width*height);
	if(pBufferData==NULL||pBufferData==nullptr){
		return false;
	}
	return true;
}
//------------------------------------------------------------------------------
void CGFrameBuffer::CGBuffer4UB::set_white(const CGVec2i& p)
{
	//...
	for(int i=0;i<4;i++){
		pBufferData[(p.x+p.y*width)*4+i]=255;
	}
}
//------------------------------------------------------------------------------
void CGFrameBuffer::CGBuffer4UB::set(const CGVec2i &p, const CGVec4 &rgba)
{
	// ...
	for(int i=0;i<4;i++){
		pBufferData[(p.x+p.y*width)*4+i]=(unsigned char)(255*rgba[i]);
	}
}
//------------------------------------------------------------------------------
CGVec4 CGFrameBuffer::CGBuffer4UB::get(const CGVec2i &p ) const
{
	CGVec4 rgba;
	// ...
	for(int i=0;i<4;i++){
		rgba[i]=pBufferData[(p.x+p.y*width)*4+i]/255.0f;
	}
	return rgba;
}

void CGFrameBuffer::CGBuffer4UB::clear(const CGVec4& rgba)
{
	// ...
	for(int i=0;i<width;i++){
		for(int j=0;j<height;j++){
			for(int k=0;k<4;k++){
				pBufferData[(i+j*width)*4+k]=(unsigned char)255*rgba[k];
			}
		}
	}
}
//------------------------------------------------------------------------------
unsigned char* CGFrameBuffer::CGBuffer4UB::getDataPointer() const
{
	return pBufferData;
}
//------------------------------------------------------------------------------
int CGFrameBuffer::CGBuffer4UB::getWidth() const
{
	return width;
}
//------------------------------------------------------------------------------
int CGFrameBuffer::CGBuffer4UB::getHeight() const
{
	return height;
}


//------------------------------------------------------------------------------
// CGFRAMEBUFFER::CGBUFFER1F
//------------------------------------------------------------------------------
CGFrameBuffer::CGBuffer1f::CGBuffer1f()
{
	pBufferData = NULL;
}
//------------------------------------------------------------------------------
CGFrameBuffer::CGBuffer1f::~CGBuffer1f()
{
	free(pBufferData);
}
//------------------------------------------------------------------------------
bool CGFrameBuffer::CGBuffer1f::reserve(int width, int height)
{
	this->width = width;
	this->height = height;
	// ...
	pBufferData = (float *) malloc(sizeof(float)*width*height);
	if(pBufferData==NULL){
		return false;
	}
	clear();
	return true;
}
//------------------------------------------------------------------------------
float CGFrameBuffer::CGBuffer1f::get(const CGVec2i &p) const
{
	// ...
	return pBufferData[ p.y*width+p.x ] ; // TODO: change this
}
//------------------------------------------------------------------------------
void CGFrameBuffer::CGBuffer1f::set(const CGVec2i &p, const float z)
{
	pBufferData[p.y*width+p.x]=z;
	// ...
}
//------------------------------------------------------------------------------
void CGFrameBuffer::CGBuffer1f::clear()
{
	for(int i =0;i<width*height;i++){
		pBufferData[i]=1.0f;
	}
	// ...
}
//------------------------------------------------------------------------------
float* CGFrameBuffer::CGBuffer1f::getDataPointer() const
{
	return pBufferData;
}
//------------------------------------------------------------------------------
int CGFrameBuffer::CGBuffer1f::getWidth() const
{
	return width;
}
//------------------------------------------------------------------------------
int CGFrameBuffer::CGBuffer1f::getHeight() const
{
	return height;
}
//------------------------------------------------------------------------------
