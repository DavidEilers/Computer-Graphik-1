#include "CGRasterizer.h"
#include "CGProgramInterface.h"
#include "CGFragmentOperations.h"
#include "CGContext.h"
#include <algorithm>
//------------------------------------------------------------------------------
void CGTriangleRasterizer::rasterize(const CGVaryings& A,const CGVaryings& B,const CGVaryings& C)
{
	// shortcuts for position of each input vertex
	const CGVec4 &a = A.position;
	const CGVec4 &b = B.position;
	const CGVec4 &c = C.position;
	CGFragmentData fragment;

	// ...

	m_frag_ops.flush_fragments();
}
