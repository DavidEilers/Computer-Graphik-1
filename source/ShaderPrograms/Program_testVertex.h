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

		out.color=in.color;
		out.position=0.5*in.position;
		// ...
	}

	virtual void fragment_shader(const CGVaryings& in,
				     CGVec4& out) const
	{
		out = in.color;
	}
};
