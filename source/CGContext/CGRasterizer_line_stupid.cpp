#include "CGRasterizer.h"
#include "CGProgramInterface.h"
#include "CGFragmentOperations.h"
#include <algorithm>
void CGStupidLineRasterizer::rasterize(const CGVaryings& a,const CGVaryings& b)
{

	// draw a line from vertex0 to vertex1
	CGVec2i from,to;
	from.set((int)a.position.x,
		 (int)a.position.y);
	to.set((int)b.position.x,
	       (int)b.position.y);
	// draw a line from <from> to <to>
	// ...

	if (from[0] > to[0]) {
		std::swap(from, to);
	}


	// Fragment to work on (initialize from vertex, set coordinates, push).
	CGFragmentData fragment;
	fragment.set(a);
	fragment.varyings.color.set(1.0f,0.0f,0.0f,1.0f);
	// ...
		int dx=to[0]-from[0];
		int dy=to[1]-from[1];
		float m;
		float n=0;
		if(dx>=dy){
			m = (dy)/(float)(dx);
			n = from[1]-m*from[0];
			for(int x=from[0];x<to[0];x++){
				fragment.coordinates.set(x,(int)(m*x+n));
				m_frag_ops.push_fragment(fragment);
			}
		}else{
			m= (dx)/(float)(dy);
			n= from[1]-from[0]/m;
			for(int y=from[1];y<to[1];y++){
				fragment.coordinates.set((int)((y-n)*m),y);
				m_frag_ops.push_fragment(fragment);
			}
		}
	m_frag_ops.flush_fragments();
}
