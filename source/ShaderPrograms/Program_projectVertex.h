#pragma once
#include "CGContext/CGProgram.h"
#include "CGContext/CGProgramInterface.h"
class Program_projectVertex : public CGProgram
{
public:
	struct
	{
		// ...
		CGMatrix4x4 projectionMatrix;
	}
	uniform;
	Program_projectVertex()
	{

	}


	virtual void vertex_shader(const CGVertexAttributes& in,
				   CGVaryings& out) const
	{
		out.color=in.color;
		out.position=uniform.projectionMatrix*in.position;
		// ...//0.5*in.position;
	}

	virtual void fragment_shader(const CGVaryings& in,
				     CGVec4& out) const
	{
		out = in.color;
	}
};
