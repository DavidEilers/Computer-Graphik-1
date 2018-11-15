#pragma once
#include <algorithm>
#include "CGProgram.h"
#include "CGProgramInterface.h"
#include "CGMath.h"

// TODO: one project requirement is phong shading
// you might want to use Program_perVertexLighting
// (from after Uebung 10) as a basis and modify it
// for phong shading...

class Program_phong : public CGProgram
{
public:
	struct
	{
		//...
	}
	uniform;
	Program_phong()
	{
		//...
	}
	virtual void vertex_shader(const CGVertexAttributes& in,
				   CGVaryings& out) const
	{
		// ...
	}
	virtual void fragment_shader(const CGVaryings& in,
				     CGVec4& out) const
	{
		// ...
	}
};
