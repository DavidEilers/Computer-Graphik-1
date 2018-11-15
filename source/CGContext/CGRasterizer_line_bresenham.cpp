#include "CGRasterizer.h"
#include "CGProgramInterface.h"
#include "CGFragmentOperations.h"
#include <algorithm>

void CGBresenhamLineRasterizer::rasterize(const CGVaryings& a,const CGVaryings& b)
{
	// draw a line from a to  b
	CGVec2i from,to;
	from.set((int)a.position.x,
		 (int)a.position.y);
	to.set((int)b.position.x,
	       (int)b.position.y);

	// fragment to work on (initialize from vertex, set coordinates, push).
	CGFragmentData fragment;
	fragment.set(a);

	// TODO: modify this to bresenham algorithm
	// naive implementation
	int x = from.x, y;
	int dx = to.x-from.x;
	int dy = to.y-from.y;
	float m = (float)dy/(float)dx;
	float n = (float)from.y-m*(float)from.x;
	while (x <= to.x) {
		y = (int)(m*x+n+0.5f);
		fragment.coordinates.set(x,y);
		m_frag_ops.push_fragment(fragment);
		x++;
	}

	m_frag_ops.flush_fragments();
}
