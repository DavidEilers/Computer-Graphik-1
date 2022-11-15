#pragma once
#include <algorithm>
#include "CGContext/CGProgram.h"
#include "CGContext/CGProgramInterface.h"
#include "CGContext/CGMath.h"

// TODO: one project requirement is phong shading
// you might want to use Program_perVertexLighting
// (from after Uebung 10) as a basis and modify it
// for phong shading...

class Program_phong : public CGProgram
{
public:

	Program_phong()
	{
		uniform.texture = NULL;
	}

	void vertex_shader(const CGVertexAttributes& in,
							   CGVaryings& out) const
	{
		out.position = uniform.projectionMatrix * uniform.modelViewMatrix* in.position;
		out.normal= CGMath::normalize(uniform.normalMatrix*in.normal);
		out.color=in.color;
		out.position_es=uniform.modelViewMatrix*in.position;
		if(NULL!=uniform.texture){
			out.texcoord = in.texcoord;
		}
		//out.material=in.material
	}

	void fragment_shader(const CGVaryings& in,
								 CGVec4& out) const
	{
		CGVec4 ambi(0.0f), diff(0.0f), spec(0.0f);


		// Transform from Object Space into Eye Space.
		CGVec4 fPos = in.position;
		CGVec4 fNrm = CGMath::normalize(in.normal);
		//fNrm = CGMath::normalize(fNrm);

		CGVec4 E = CGVec4(0.0f,0.0f,0.0f,1.0f)-in.position_es;
		E= CGMath::normalize(E);
		out=CGVec4(0.0f,0.0f,0.0f,0.0f);

		for(int i=0;i<uniform.lightCount;i++){
			ambi= uniform.material.ambient*uniform.light[i].ambient;
			// L is vector direction from current point (fPos) to the light source (uniforms.lightPosition[0])
			CGVec4 L;
			if(uniform.light[i].position_es.w!=0.0f){
				L = uniform.light[i].position_es-in.position_es;
			}else{
				L = uniform.light[i].position_es;
			}

			L = CGMath::normalize(L);
			// calculate dot product of nrm and L
			float NdotL = CGMath::dot(fNrm,L);
			float spotFactor = 1.0f;

				if(uniform.light[i].spot_cutoff!=180.0f){
					//puts("happend!");
					//ambi.print();
					spotFactor=0.0f;
					CGVec4 mL = CGMath::normalize(CGVec4(0.0f,0.0f,0.0f,0.0f)-L);
					CGVec4 S = CGMath::normalize(uniform.light[i].spot_direction);
					spotFactor = std::max(CGMath::dot(S,mL),0.0f);
					if( spotFactor < cos( (3.141592653f*uniform.light[i].spot_cutoff)/180.0f ) ){
						spotFactor = 0.0f;
					}else{
						spotFactor = std::pow(spotFactor,uniform.light[i].spot_exponent);
					}
				}

			if(NdotL>0.0f){
				// diffuse
				if(spotFactor!=0.0f){
					diff = uniform.material.diffuse*uniform.light[i].diffuse*NdotL;
					// E is direction from current point (vPos) to eye position
					// H is halfway vector between L and E
					CGVec4 H = L+E;
					H=CGMath::normalize(H);
					float NdotH = CGMath::dot(fNrm,H);
					// specular
					NdotH = std::max(NdotH,0.0f);
					spec = uniform.material.specular*uniform.light[i].specular*std::pow(NdotH,uniform.material.shininess);
				}else{
					diff=spec=CGVec4(0.0f,0.0f,0.0f,0.0f);
				}
			}

		// sum up the final output color
		out += ambi + spotFactor*(diff + spec);

}
		//out=CGVec4(1.0f,1.0f,1.0f,1.0f);
		// Explicitly set alpha of the color
		out.w = uniform.material.diffuse.w;
		out = CGMath::clamp(out,
			CGVec4(0.0f,0.0f,0.0f,0.0f),
			CGVec4(1.0f,1.0f,1.0f,1.0f)
		);


		//out = in.color;
		if(NULL !=uniform.texture){
			out *= uniform.texture->sample(in.texcoord);
		}else{
			out *= in.color;
		}
		// clamp color values to range [0,1]
		//out.color = ...

		// Transform from Eye Space into Clip Space.
		// ...
	}
};
