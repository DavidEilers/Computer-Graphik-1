#pragma once

#include "CGVector.h"
#include "CG.h"

//---------------------------------------------------------------------------
class CGTexture2D
{
public:
	/// Constructor
	CGTexture2D();
	/// Destructor
	~CGTexture2D();
	/// Initialize the texture here.
	void allocate(int width, int height, const unsigned char* data4ub);
	//// destruct the texture object
	void destruct();
	/// Get Texel data as float vector
	CGVec4 fetchTexel(int x, int y) const;
	/// Sample with filter.
	CGVec4 sample(const CGVec4 &texcoord) const;

	CGFilterMode filterMode;
	CGWrapMode wrapMode;
private:
	int width, height;

	unsigned char* data; 
	void reset();
};
