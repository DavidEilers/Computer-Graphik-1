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
		varyings.position=((1-ratio)*A.position+ratio*B.position);
		float alpha = (1.0f - ratio)*A.position.w;
		float beta = ratio*B.position.w;
		varyings.color=(alpha*A.color+beta*B.color)/(alpha+beta);
		varyings.normal=(alpha*A.normal+beta*B.normal)/(alpha+beta);
		varyings.texcoord=(alpha*A.texcoord+beta*B.texcoord)/(alpha+beta);
		varyings.position_es=(alpha*A.position_es+beta*B.position_es)/(alpha+beta);

		// interpolation
		// ...
	}
	void set(const CGVaryings& A, const CGVaryings& B, const CGVaryings& C,
		 float alpha, float beta, float gamma)
	{
		varyings.position=(alpha*A.position+beta*B.position+gamma*C.position);
		alpha*=A.position.w;
		beta*=B.position.w;
		gamma*=C.position.w;
		varyings.color= (alpha*A.color+beta*B.color+gamma*C.color)/(alpha+beta+gamma);
		varyings.normal= (alpha*A.normal+beta*B.normal+gamma*C.normal)/(alpha+beta+gamma);
		varyings.texcoord= (alpha*A.texcoord+beta*B.texcoord+gamma*C.texcoord)/(alpha+beta+gamma);
		varyings.position_es= (alpha*A.position_es+beta*B.position_es+gamma*C.position_es)/(alpha+beta+gamma);

		// interpolation
		// ...
	}
};
