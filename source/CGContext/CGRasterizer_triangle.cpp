#include "CGRasterizer.h"
#include "CGProgramInterface.h"
#include "CGFragmentOperations.h"
#include "CGContext.h"
#include <algorithm>
//------------------------------------------------------------------------------

float baryzent(CGVec4 a, CGVec4 b, CGVec4 p){
	p.x+=0.5;
	p.y+=0.5;
	return ((a.x-p.x)*(b.y-p.y))-((a.y-p.y)*(b.x-p.x));
}

int min(int a, int b){
	return a<b? a:b;
}
int max(int a, int b){
	return a<b? b:a;
}

void CGTriangleRasterizer::rasterize(const CGVaryings& A,const CGVaryings& B,const CGVaryings& C)
{
	// shortcuts for position of each input vertex
	const CGVec4 &a = A.position;
	const CGVec4 &b = B.position;
	const CGVec4 &c = C.position;
	CGFragmentData fragment;
	CGVec4 p;
	float surfaceArea=((b.x-a.x)*(c.y-a.y))-((b.y-a.y)*(c.x-a.x));
	float minx=min(min(a.x,b.x),c.x);
	p.y=min(min(a.y,b.y),c.y);
	int width= max(max(a.x,b.x),c.x);
	int height= max(max(a.y,b.y),c.y);


	for(;p.y<=height;p.y++){
		for(p.x=minx;p.x<=width;p.x++){

			float nAlpha = baryzent(b,c,p);
			float nBeta = baryzent(c, a,p);
			float nGamma = baryzent(a,b,p);
				float alpha = nAlpha/surfaceArea;
				float beta = nBeta/surfaceArea;
				float gamma = nGamma/surfaceArea;
			if(alpha>=0&&beta>=0&&gamma>=0){
				fragment.coordinates.set(p.x,p.y);
				fragment.set(A,B,C,alpha,beta,gamma);
				m_frag_ops.push_fragment(fragment);
			}
		}
	}
	m_frag_ops.flush_fragments();
}
