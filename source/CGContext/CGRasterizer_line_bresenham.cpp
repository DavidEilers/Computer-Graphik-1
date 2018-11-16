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
	bool swapxy;
	int dx = abs(to.x-from.x);
	int dy = abs(to.y-from.y);
	if(dx<dy){
		swapxy=true;
		int buffer = from.x;
		from.x = from.y;
		from.y=buffer;
		buffer = to.x;
		to.x=to.y;
		to.y=buffer;
		buffer = dx;
		dx=dy;
		dy=buffer;
	}else{
		swapxy=false;
	}

	int x = from.x, y;
	y=from.y;
	int d = 2*dy-dx;
	int deltaE=2*dy;
	int deltaNE=2*(dy-dx);
	int xstep = (from.x<=to.x)?1:-1;
	int ystep = (from.y<=to.y)?1:-1;
	while (x != to.x+xstep) {
		if(swapxy==true){
			fragment.coordinates.set(y,x);
		}else{
			fragment.coordinates.set(x,y);
		}
		x+=xstep;
		if(d<=0){
			d+=deltaE;
		}else{
			y+=ystep;
			d+=deltaNE;
		}
		m_frag_ops.push_fragment(fragment);
	}

	m_frag_ops.flush_fragments();
}
