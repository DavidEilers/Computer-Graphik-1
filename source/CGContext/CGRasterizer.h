#pragma once
struct CGVaryings;
class CGFragmentOperations;
struct CGVec4;
class CGContext;

class CGRasterizer
{
protected:
	const CGContext& m_context;
	CGFragmentOperations& m_frag_ops;

public:
	CGRasterizer(const CGContext& context, CGFragmentOperations& frag_ops)
		:m_context(context),m_frag_ops(frag_ops){}
};

// ----- Point Rasterization ---------------------------------------------------
class CGPointRasterizer :public CGRasterizer
{
public:
	CGPointRasterizer(const CGContext& context, CGFragmentOperations& frag_ops)
		:CGRasterizer(context,frag_ops){}
	virtual void rasterize(const CGVaryings& a);
};

// ----- Line Rasterization ----------------------------------------------------

class CGLineRasterizer :public CGRasterizer
{
public:
	CGLineRasterizer(const CGContext& context, CGFragmentOperations& frag_ops)
		:CGRasterizer(context,frag_ops){}
	virtual void rasterize(const CGVaryings& a, const CGVaryings& b)=0;
};

class CGStupidLineRasterizer :public CGLineRasterizer
{
public:
	CGStupidLineRasterizer(const CGContext& context, CGFragmentOperations& frag_ops)
		:CGLineRasterizer(context,frag_ops){}
	virtual void rasterize(const CGVaryings& a, const CGVaryings& b);
};

class CGBresenhamLineRasterizer :public CGLineRasterizer
{
public:
	CGBresenhamLineRasterizer(const CGContext& context, CGFragmentOperations& frag_ops)
		:CGLineRasterizer(context,frag_ops){}
	virtual void rasterize(const CGVaryings& a, const CGVaryings& b);
};

// ----- Triangle Rasterization ------------------------------------------------
class CGTriangleRasterizer :public CGRasterizer
{
public:

	CGTriangleRasterizer(const CGContext& context, CGFragmentOperations& frag_ops)
		:CGRasterizer(context,frag_ops){}

	virtual void rasterize(const CGVaryings& A,const CGVaryings& B,const CGVaryings& C);
};

