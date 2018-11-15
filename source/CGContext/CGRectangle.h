#pragma once

#include "CGVector.h"
class CGRectangle
{
public:
	CGRectangle(int x, int y, int w, int h)
		:bottom_left(x,y),width(w),height(h){};
	CGVec2i bottom_left;
	int width;
	int height;


	/// returns true if the point p is inside this rectangle
	bool isInside(const CGVec2i& p) const
	{
		// ...
		return false; // TODO: change this later...
	}
};


