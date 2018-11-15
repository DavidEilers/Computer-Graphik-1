#include "CGRasterizer.h"
#include "CGProgramInterface.h"
#include "CGFragmentOperations.h"

void CGPointRasterizer::rasterize(const CGVaryings& a)
{
	// Initialize the fragment.
	CGFragmentData fragment;
	fragment.set(a);

	// And set coordinates. (SetFragment initializes just the attributes + varyings).
	fragment.coordinates.set((int) a.position.x,
				 (int) a.position.y);

	// Push the fragment into the pipeline.
	m_frag_ops.push_fragment(fragment);
	m_frag_ops.flush_fragments();
}
