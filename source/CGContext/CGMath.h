#pragma once

#include  "CG.h"
#include "CGVector.h"
#include "CGMatrix.h"
#define _USE_MATH_DEFINES
#include <cmath>
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841971693993751058209749
#endif
#include <stdio.h>
//#include <assert.h>

#define ISZERO(x) (fabs(x)<1.0e-5)

namespace  CGMath
{
inline float dot(const CGVec4& a, const CGVec4& b)
{
	//assert(!(!ISZERO(a[3]) && !ISZERO(b[3]))); // scalar product only with vector(s)
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2]+a[3]*b[3];
}
inline CGVec4 cross(const CGVec4& a, const CGVec4& b)
{
	//assert(ISZERO(a[3]) && ISZERO(b[3])); // no cross product for points
	CGVec4 c;
	c[0]=a[1]*b[2]-a[2]*b[1];
	c[1]=a[2]*b[0]-a[0]*b[2];
	c[2]=a[0]*b[1]-a[1]*b[0];
	c[3]=0.0f;
	return c;
}
inline float length(const CGVec4& a)
{
	return sqrtf(dot(a,a));
}
inline CGVec4 normalize(const CGVec4& a)
{
	return a / length(a);
}
inline CGVec4 divw(const CGVec4& a)
{
	CGVec4 c;
	//assert(!ISZERO(a[3]));
	float s=1.0f/a[3];
	c[0]=s*a[0]; c[1]=s*a[1]; c[2]=s*a[2]; c[3]=1.0f;
	return c;
}
inline CGVec4 max(const CGVec4& a, float m)
{
	CGVec4 c;
	for (unsigned int i=0; i<4; i++)
		c[i]=a[i]>m?a[i]:m;
	return c;
}
inline CGVec4 min(const CGVec4& a, float n)
{
	CGVec4 c;
	for (unsigned int i=0; i<4;i++)
		c[i]=a[i]>n?n:a[i];
	return c;
}
inline CGVec4 max(const CGVec4& a, const CGVec4& b)
{
	CGVec4 c;
	for (unsigned int i=0; i<4; i++)
		c[i]=a[i]>b[i]?a[i]:b[i];
	return c;
}
inline CGVec4 min(const CGVec4& a, const CGVec4& b)
{
	CGVec4 c;
	for (unsigned int i=0; i<4; i++)
		c[i]=a[i]>b[i]?b[i]:a[i];
	return c;
}
inline CGVec4 clamp(const CGVec4& a, float m, float n)
{
	return max(min(a,n),m);
}
inline CGVec4 clamp(const CGVec4& a, const CGVec4& m, const CGVec4& n)
{
	return max(min(a,n),m);
}
inline CGVec4 abs(const CGVec4& a)
{
	CGVec4 c;
	for (unsigned int i=0; i<4; i++)
		c[i]=fabs(a[i]);
	return c;
}
inline CGVec4 ceil(const CGVec4& a)
{
	CGVec4 c;
	for (unsigned int i=0; i<4; i++)
		c[i]=std::ceil(a[i]);
	return c;
}
inline CGVec4 floor(const CGVec4& a)
{
	CGVec4 c;
	for (unsigned int i=0; i<4; i++)
		c[i]=std::floor(a[i]);
	return c;
}
inline CGVec4 fract(const CGVec4& a)
{
	CGVec4 c;
	for (unsigned int i=0; i<4; i++)
		c[i]=a[i]-std::floor(a[i]);
	return c;
}
inline float distance(const CGVec4& a, const CGVec4& b)
{
	//assert(ISZERO(a[3]-b[3])); // a[3] must equal b[3]
	return length(a-b);
}


};
