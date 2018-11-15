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
		// interpolation
		// ...
	}
	void set(const CGVaryings& A, const CGVaryings& B, const CGVaryings& C,
		 float alpha, float beta, float gamma)
	{
		// interpolation
		// ...
	}
};

