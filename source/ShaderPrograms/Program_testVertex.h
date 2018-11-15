#pragma once
#include "CGContext/CGProgram.h"
#include "CGContext/CGProgramInterface.h"
class Program_testVertex : public CGProgram
{
public:
	struct
	{

	}
	uniform;
	Program_testVertex()
	{

	}


	virtual void vertex_shader(const CGVertexAttributes& in,
				   CGVaryings& out) const
	{
		// ...
	}

	virtual void fragment_shader(const CGVaryings& in,
				     CGVec4& out) const
	{
		out = in.color;
	}
};
