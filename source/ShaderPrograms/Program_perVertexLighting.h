#pragma once
#include "CGContext/CGProgram.h"
#include "CGContext/CGProgramInterface.h"
#include "CGContext/CGMath.h"
#include <algorithm>
class Program_perVertexLighting : public CGProgram
{
public:

	Program_perVertexLighting()
	{
		uniform.texture = NULL;
	}

	void vertex_shader(const CGVertexAttributes& in,
							   CGVaryings& out) const
	{
		CGVec4 ambi(0.0f), diff(0.0f), spec(0.0f);

		CGVec4 vPos = uniform.modelViewMatrix * in.position;
		CGVec4 vNrm = uniform.normalMatrix * in.normal;
		vNrm = CGMath::normalize(vNrm);
		// E is direction from current point (vPos) to eye position
		CGVec4 E = (-1)*vPos ;
		E.w=0.0f;
		E= CGMath::normalize(E);

		out.color=CGVec4(0.0f,0.0f,0.0f,0.0f);

		for(int i=0;i<uniform.lightCount;i++){
			ambi= uniform.light[i].ambient*uniform.material.ambient;
			// Transform from Object Space into Eye Space.
			// L is vector direction from current point (vPos) to the light source (uniforms.lightPosition[0])
			CGVec4 L;
			if(uniform.light[i].position_es.w!=0.0f){
				L = uniform.light[i].position_es-vPos;
			}else{
				L = uniform.light[i].position_es;
			}

			L = CGMath::normalize(L);
			// calculate dot product of nrm and L
			float NdotL = CGMath::dot(vNrm,L);
			float spotFactor = 1.0f;


				if(uniform.light[i].spot_cutoff!=180.0f){
					CGVec4 S = CGMath::normalize(uniform.light[i].spot_direction);
					spotFactor = std::max(CGMath::dot(S,(-1)*L),0.0f);
					if(spotFactor < cosf((uniform.light[i].spot_cutoff)*(3.14159265f/180.0f)) ){
						spotFactor = 0.0f;
					}else{
						spotFactor = std::pow(spotFactor,uniform.light[i].spot_exponent);
					}
				}

			if(NdotL>0.0f){

				// diffuse
				diff = uniform.material.diffuse*uniform.light[i].diffuse*NdotL;
				// H is halfway vector between L and E
				CGVec4 H = L+E;
				H=CGMath::normalize(H);
				float NdotH = CGMath::dot(vNrm,H);
				// specular
				NdotH = std::max(NdotH,0.0f);
				spec = uniform.material.specular*uniform.light[i].specular*std::pow(NdotH,uniform.material.shininess);
			}

			// sum up the final output color
			out.color += ambi + spotFactor*( diff + spec);
			// Explicitly set alpha of the color
			out.color.w = uniform.material.diffuse.w;
			out.color = CGMath::clamp(out.color,
				CGVec4(0.0f,0.0f,0.0f,0.0f),
				CGVec4(1.0f,1.0f,1.0f,1.0f)
			);
	}
		out.color*=in.color;
		// clamp color values to range [0,1]
		//out.color = ...
		if(NULL!=uniform.texture){
			out.texcoord = in.texcoord;
		}
		// Transform from Eye Space into Clip Space.
		out.position = uniform.projectionMatrix * vPos;

		// forward tex-coords as varying
		// ...
	}

	void fragment_shader(const CGVaryings& in,
								 CGVec4& out) const
	{

		out = in.color;
		if(NULL !=uniform.texture){
			out *= uniform.texture->sample(in.texcoord);
		}
		// ...
	}
};
