#pragma once
#include "CGVector.h"
struct CGVaryings;
struct CGVertexAttributes;


class CGProgram
{
public:
	CGProgram() {}
//	virtual ~CGProgram(){}
	/// This will be called during the vertex processing stage.
	virtual void vertex_shader(const CGVertexAttributes& in,
							   CGVaryings& out) const = 0;

	/// This will be called during the fragment processing stage.
	virtual void fragment_shader(const CGVaryings& in,
								 CGVec4& out) const = 0;

	struct{

	 struct MaterialData
		{
			CGVec4 ambient;
			CGVec4 diffuse;
			CGVec4 specular;
			CGVec4 emissive;
			float shininess;

			MaterialData()
			{
				ambient  = CGVec4(0.3f, 0.3f, 0.3f, 1.0f);
				diffuse  = CGVec4(0.8f, 0.8f, 0.8f, 1.0f);
				specular = CGVec4(0.3f, 0.3f, 0.3f, 1.0f);
				shininess = 1.0f;
			}
//			virtual ~MaterialData(){}
		};

		// define a the properties of a light source
		// parameters contain the necessary parameters for classical
		// blinn-phong reflection model
		struct LightData
		{
			CGVec4 ambient;
			CGVec4 diffuse;
			CGVec4 specular;
			CGVec4 position_es;
			CGVec4 spot_direction;
			float spot_cutoff;
			float spot_exponent;

			LightData()
			{
				spot_direction = CGVec4(-1.0f,-1.0f,0.0f,0.0f);
				spot_exponent = 0.0f;
				spot_cutoff = 180.0f;
				position_es = CGVec4(0.0f,0.0f,0.0f,1.0f);
			}
			//virtual ~LightData(){}
		};

		// the actual uniforms
		MaterialData material;
		LightData light[5];
		CGMatrix4x4 projectionMatrix;
		CGMatrix4x4 modelViewMatrix;
		CGMatrix4x4 normalMatrix;
		CGTexture2D* texture;
		int lightCount=1;
		//virtual ~Uniform(){}

		// helper to calculate the normalMatrix from the modelViewMatrix
		// call this whenever after you set a new modelViewMatrix!
		void updateNormalMatrix()
		{
			normalMatrix = modelViewMatrix;
			for(unsigned int i = 0; i< 3;i++){
				normalMatrix.at(3,i) = 0.0f;
				normalMatrix.at(i,3) = 0.0f;
			}
			normalMatrix.at(3,3)=1.0f;

			normalMatrix.invert();
			normalMatrix.transpose();
		}
	}uniform;

};
