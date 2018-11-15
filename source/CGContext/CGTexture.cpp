#include "CGTexture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CGMath.h"

//------------------------------------------------------------------------------
// CGTEXTURE2D
//------------------------------------------------------------------------------
CGTexture2D::CGTexture2D()
{
	reset();
}
//------------------------------------------------------------------------------
CGTexture2D::~CGTexture2D()
{
	destruct();
}
//------------------------------------------------------------------------------
void CGTexture2D::reset()
{
	width=height=0;
	filterMode=CG_NEAREST;
	wrapMode=CG_CLAMP;
	data=NULL;
}
//------------------------------------------------------------------------------
void CGTexture2D::allocate(int width, int height, const unsigned char* data4ub)
{
	this->width = width;
	this->height = height;
	data = (unsigned char*) realloc(data,(size_t)(width*height)*4);
	memcpy(data,data4ub,(size_t)(width*height)*4);
}
//------------------------------------------------------------------------------
void CGTexture2D::destruct()
{
	if (data) {
		free(data);
	}
	reset();
}
//------------------------------------------------------------------------------
CGVec4 CGTexture2D::fetchTexel(int x, int y) const
{
	// ...
	CGVec4 sample;
	// TODO: for now, just return always white
	sample.set(1.0f); // TODO: remove this;
	return sample;
}
//------------------------------------------------------------------------------
CGVec4 CGTexture2D::sample(const CGVec4 &texcoord) const
{
	CGVec4 sample;
	// ...
	// TODO: for now, just return always white
	sample.set(1.0f); // TODO: remove this;
	return sample;
}
//------------------------------------------------------------------------------
