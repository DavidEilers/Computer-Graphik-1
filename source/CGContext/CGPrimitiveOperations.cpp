#include "CGPrimitiveOperations.h"
#include "CGPrimClipper.h"
#include "CGContext.h"
#include "CGRasterizer.h"
//------------------------------------------------------------------------------
void CGPrimitiveOperations::invoke_line_rasterization(const CGVaryings &a, const CGVaryings &b)
{
	if(m_context.capabilities.bresenham) {
		m_bresenham_line_rasterizer.rasterize(a,b);
	}
	else {
		m_stupid_line_rasterizer.rasterize(a,b);
	}
}
//------------------------------------------------------------------------------
bool CGPrimitiveOperations::backface_culling(const CGVaryings &a, const CGVaryings &b, const CGVaryings &c)
{
	// return true if the triangle is visible, false otherwise
	// ...
	return true; // TODO: change this later
}
//------------------------------------------------------------------------------
void CGPrimitiveOperations::invoke_triangle_rasterization(const CGVaryings &a, const CGVaryings &b, const CGVaryings &c)
{
	// ...
}
//------------------------------------------------------------------------------
CGPrimitiveOperations::CGPrimitiveOperations(CGContext &context, CGPointRasterizer &point_raster, CGLineRasterizer &line_raster, CGLineRasterizer &bresenham_line_raster, CGTriangleRasterizer &triangle_raster)
	:m_context(context),m_point_rasterizer(point_raster),m_stupid_line_rasterizer(line_raster),m_bresenham_line_rasterizer(bresenham_line_raster),m_triangle_rasterizer(triangle_raster)
{
	m_num_vertices = 0;

	setPrimitiveMode(CG_POINTS);
}
//------------------------------------------------------------------------------
void CGPrimitiveOperations::setPrimitiveMode(CGPrimitiveType prim_mode)
{
	m_primitive_mode = prim_mode;
	switch (m_primitive_mode) {
	case CG_POINTS: m_required_vertices=1; break;
	case CG_LINES: m_required_vertices=2; break;
	case CG_TRIANGLES: m_required_vertices=3; break;
	}
}
//------------------------------------------------------------------------------
void CGPrimitiveOperations::pushVertex(const CGVaryings& v)
{
	// primitive assembly
	m_vertex_varyings[m_num_vertices++]=v;
	if(m_num_vertices < m_required_vertices)
	{
		return;
	}

	// This might split our single primitive and create new ones.
	// This is deactivated initally. Later, we will enable this:
	//clip_primitive();

	perspective_divide_primitives();
	viewport_transform_primitives();

	// We might have to rasterize more than one primitive.
	for(unsigned int i=0; i<m_num_vertices; i+=m_required_vertices) {

		switch (m_primitive_mode) {
		case CG_POINTS: m_point_rasterizer.rasterize((m_vertex_varyings[i])); break;
		case CG_LINES: invoke_line_rasterization(m_vertex_varyings[i],m_vertex_varyings[i+1]); break;
		case CG_TRIANGLES: invoke_triangle_rasterization(m_vertex_varyings[i], m_vertex_varyings[i+1],
					m_vertex_varyings[i+2]); break;
		}

	}

	// Purge vertex list.
	m_num_vertices = 0;
}
//------------------------------------------------------------------------------
void CGPrimitiveOperations::clip_primitive()
{
	int newPrimCount;

	switch(m_primitive_mode) {
	case CG_POINTS:
		m_num_vertices =
				CGPrimClipper::clipPoint(m_vertex_varyings, m_num_vertices);
		break;
	case CG_LINES:
		m_num_vertices =
				CGPrimClipper::clipLine(m_vertex_varyings, m_num_vertices);
		break;
	case CG_TRIANGLES:
		// Clipped vertices need to be re-assembled into triangles
		newPrimCount = CGPrimClipper::clipPoly(m_vertex_varyings, m_num_vertices)-2;
		m_num_vertices = 0; //reset the pipeline
		for (int i=0; i<newPrimCount; ++i) { //as triangle fan
			m_vertex_varyings[m_num_vertices++]=CGPrimClipper::outBuf()[0];
			m_vertex_varyings[m_num_vertices++]=CGPrimClipper::outBuf()[i+1];
			m_vertex_varyings[m_num_vertices++]=CGPrimClipper::outBuf()[i+2];
		}
		break;
		// Insert other primitive types here.
	}
}
//------------------------------------------------------------------------------
void CGPrimitiveOperations::perspective_divide_primitives()
{
	for(unsigned int i = 0 ; i< m_num_vertices; i++)
	{
		CGVec4& pos = m_vertex_varyings[i].position;
		// transform position from clip space into NDC
		// ...
	}
}
//------------------------------------------------------------------------------
void CGPrimitiveOperations::viewport_transform_primitives()
{
	for(unsigned int i = 0 ; i< m_num_vertices; i++)
	{
		CGVec4& pos = m_vertex_varyings[i].position;
		// transform position from NDC into window space
		// ...
	}
}