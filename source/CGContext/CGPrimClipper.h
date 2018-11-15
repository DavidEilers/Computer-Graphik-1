#pragma once
#include "CG.h"

// Forwad declaration
struct CGVaryings;
//----------------------------------------------------------------------------
// Primitive clipper works in homogeneous clip space.
//----------------------------------------------------------------------------
class CGPrimClipper
{
public:
	/// Bit mask, each bit for one clip boundary
	static unsigned int ClipBits;

	/// Pointer to output vertices.
	static const CGVaryings* outBuf();

	/// A point clipper, it does actually culling
	static unsigned int clipPoint(CGVaryings *vinBuf, int vinCnt);

	/// A line clipper
	/// Return the vertex count after clipping.
	static unsigned int clipLine(CGVaryings *vinBuf, int vinCnt);

	/// A polygon clipper
	/// Return the vertex count after clipping.
	static unsigned int clipPoly(CGVaryings *vinBuf, int vinCnt);

private:
	static CGVaryings sm_voutBuf[CG_MAX_VERTICES_IN_PIPELINE];  //output buffer, it is also the internal working buffer

	/// Liang-Barsky line-clipping
	static unsigned int clipLine_LB(CGVaryings *vinBuf, int vinCnt);

	/// Sutherland-Hodgman polygon-clipping
	static unsigned int clipPoly_SH(CGVaryings *vinBuf, int vinCnt);

	/// Sutherland-Hodgman polygon-clipping against single clipping plane
	/// voutBufSize is the maximal size of the voutBuf.
	/// Return the number of output vertices.
	static int clipPoly_SH_single(unsigned int clipBound, const CGVaryings *vinBuf, int nvin, CGVaryings *voutBuf);
};

