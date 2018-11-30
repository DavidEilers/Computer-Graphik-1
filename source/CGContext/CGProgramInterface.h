#pragma once
#include "CGMatrix.h"
#include "CGVector.h"
#include "CGTexture.h"
#include "CG.h"
//---------------------------------------------------------------------------

/// Vertex attribute container (vertex processing inputs).
struct CGVertexAttributes
{
	CGVec4 position;
	CGVec4 normal;
	CGVec4 color;
	CGVec4 texcoord;
};

/// Vertex varying container (vertex processing outputs).
struct CGVaryings
{
	CGVec4 position;
	CGVec4 normal;
	CGVec4 color;
	CGVec4 texcoord;
	CGVec4 position_es;
};

/// Fragment data container.
struct CGFragmentData
{
	CGVec2i coordinates;   /// Fragment specific framebuffer coordinate
	CGVaryings varyings;   /// Varyings as copied from vertex or interpolated.
	void set(const CGVaryings& vertex)
	{
		varyings = vertex;
	}
	void set(const CGVaryings& A, const CGVaryings& B, float ratio)
	{
		varyings.color=(1-ratio)*A.color+ratio*B.color;
		varyings.normal=(1-ratio)*A.normal+ratio*B.normal;
		varyings.texcoord=(1-ratio)*A.texcoord+ratio*B.texcoord;
		varyings.position_es=(1-ratio)*A.position_es+ratio*B.position_es;
		// interpolation
		// ...
	}
	void set(const CGVaryings& A, const CGVaryings& B, const CGVaryings& C,
		 float alpha, float beta, float gamma)
	{
		varyings.color= alpha*A.color+beta*B.color+gamma*C.color;
		varyings.normal= alpha*A.normal+beta*B.normal+gamma*C.normal;
		varyings.texcoord= alpha*A.texcoord+beta*B.texcoord+gamma*C.texcoord;
		varyings.position_es= alpha*A.position_es+beta*B.position_es+gamma*C.position_es;
		// interpolation
		// ...
	}
};
