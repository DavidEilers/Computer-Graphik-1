#pragma once
#include "CGContext/CGProgram.h"
#include "CGContext/CGProgramInterface.h"
class Program_modelViewProjection : public CGProgram
{
public:
	struct
	{
		CGMatrix4x4 projectionMatrix;
		CGMatrix4x4 modelViewMatrix;
	}
	uniform;
	Program_modelViewProjection()
	{

	}

	virtual void vertex_shader(const CGVertexAttributes& in, CGVaryings& out) const
	{
		// ...
	}

	virtual void fragment_shader(const CGVaryings& in, CGVec4& out) const
	{
		out = in.color;
	}
};
