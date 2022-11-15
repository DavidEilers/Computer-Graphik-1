#include "CGMatrix.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "CGVector.h"
#include "CGMath.h"
//------------------------------------------------------------------------------
// CGMATRIX4X4 : Static (factory) functions.
//------------------------------------------------------------------------------
CGMatrix4x4 CGMatrix4x4::getIdentityMatrix()
{
	CGMatrix4x4 m;
	return m;
}
//------------------------------------------------------------------------------
CGMatrix4x4 CGMatrix4x4::getTranslationMatrix(float x, float y, float z)
{
	CGMatrix4x4 m;
	m.m_matrix[3][0] = x; m.m_matrix[3][1] = y; m.m_matrix[3][2] = z;
	return m;
}
//------------------------------------------------------------------------------
CGMatrix4x4 CGMatrix4x4::getRotationMatrixX(float angle)
{
	return CGMatrix4x4::getRotationMatrix(angle, 1.0f, 0.0f, 0.0f);
}
//------------------------------------------------------------------------------
CGMatrix4x4 CGMatrix4x4::getRotationMatrixY(float angle)
{
	return CGMatrix4x4::getRotationMatrix(angle, 0.0f, 1.0f, 0.0f);
}
//------------------------------------------------------------------------------
CGMatrix4x4 CGMatrix4x4::getRotationMatrixZ(float angle)
{
	return CGMatrix4x4::getRotationMatrix(angle, 0.0f, 0.0f, 1.0f);
}
//------------------------------------------------------------------------------
CGMatrix4x4 CGMatrix4x4::getScaleMatrix(float x, float y, float z)
{
	CGMatrix4x4 m;
	m.m_matrix[0][0] = x; m.m_matrix[1][1] = y; m.m_matrix[2][2] = z;
	return m;
}
//------------------------------------------------------------------------------
CGMatrix4x4 CGMatrix4x4::getRotationMatrix(float angle, float x, float y, float z)
{
	const float c = (float)cosf(angle * (float)M_PI / 180.0f);
	const float one_minus_c = 1.0f - c;
	const float s = (float)sinf(angle * (float)M_PI / 180.0f);
	const float v_len = (float)sqrtf(x*x + y*y + z*z);
	float x_one_minus_c;
	float y_one_minus_c;
	float z_one_minus_c;
	CGMatrix4x4 rot;
	if (v_len == 0.0f)
		return rot;
	if (v_len != 1.0f) {
		x /= v_len; y /= v_len; z /= v_len;
	}
	x_one_minus_c = x * one_minus_c;
	y_one_minus_c = y * one_minus_c;
	z_one_minus_c = z * one_minus_c;
	rot.m_matrix[0][0] = x * x_one_minus_c + c;
	rot.m_matrix[0][1] = x * y_one_minus_c + z * s;
	rot.m_matrix[0][2] = x * z_one_minus_c - y * s;
	rot.m_matrix[1][0] = y * x_one_minus_c - z * s;
	rot.m_matrix[1][1] = y * y_one_minus_c + c;
	rot.m_matrix[1][2] = y * z_one_minus_c + x * s;
	rot.m_matrix[2][0] = z * x_one_minus_c + y * s;
	rot.m_matrix[2][1] = z * y_one_minus_c - x * s;
	rot.m_matrix[2][2] = z * z_one_minus_c + c;
	return rot;
}
//------------------------------------------------------------------------------
CGMatrix4x4 CGMatrix4x4::getFrustum(float left, float right, float bottom, float top, float nearVal, float farVal)
{
	CGMatrix4x4 f= getIdentityMatrix();
	f.at(0,0)=(2*nearVal)/(right-left);
	f.at(1,1)=(2*nearVal)/(top-bottom);
	f.at(2,0)=(right+left)/(right-left);
	f.at(2,1)=(top+bottom)/(top-bottom);
	f.at(2,2)=-(farVal+nearVal)/(farVal-nearVal);
	f.at(3,2)=-(2*farVal*nearVal)/(farVal-nearVal);
	f.at(2,3)=-1;
	f.at(3,3)=0.0;

	// ...
	return f;
}
//------------------------------------------------------------------------------
CGMatrix4x4 CGMatrix4x4::getOrtho(float left, float right, float bottom, float top, float nearVal, float farVal)
{
	CGMatrix4x4 o;
	o.at(0,0)=2.0f/(right-left);
	o.at(1,1)=2.0f/(top-bottom);
	o.at(2,2)=-(2.0f/(farVal-nearVal));
	o.at(3,0)=-((right+left)/(right-left));
	o.at(3,1)=-((top+bottom)/(top-bottom));
	o.at(3,2)=-((farVal+nearVal)/(farVal-nearVal));
	o.at(3,3)=1.0f;
	return o;
}
//------------------------------------------------------------------------------
CGMatrix4x4 CGMatrix4x4::getPerspective(float fov_y, float aspect, float zNear, float zFar)
{
	CGMatrix4x4 P;
	float t = zNear*tan((fov_y/2.0f)*((float)M_PI/180.0f));
	float r= t*aspect;
	P = getFrustum(-(r/2.0f),r/(2.0f),-(t/2.0f),t/2.0f,zNear,zFar );
	// ...
	return P;
}
//------------------------------------------------------------------------------
CGMatrix4x4 CGMatrix4x4::getLookAt(const CGVec4& eye, const CGVec4& center, const CGVec4 up)
{
	CGVec4 f = (center-eye);
	f/=sqrt(f.x*f.x+f.y*f.y+f.z*f.z);
	f.w=0.0f;
	CGVec4 s = CGVec4(f.y*up.z-up.y*f.z,f.z*up.x-up.z*f.x,f.x*up.y-up.x*f.y,0.0f);
	s/=sqrt(s.x*s.x+s.y*s.y+s.z*s.z);
	s.w=0.0f;
	CGVec4 u = CGVec4(s.y*f.z-f.y*s.z,s.z*f.x-f.z*s.x,s.x*f.y-f.x*s.y,0.0f);
	CGMatrix4x4 V(1.0f);
	CGMatrix4x4 R(1.0f);
	R.setRow(0,s);
	R.setRow(1,u);
	f=-1*f;
	R.setRow(2,f);
	V=R*CGMatrix4x4::getTranslationMatrix(-eye.x, -eye.y, -eye.z);
	return V;
}


//------------------------------------------------------------------------------
// CGMATRIX4X4 : Non-static operants.
//------------------------------------------------------------------------------
CGMatrix4x4::CGMatrix4x4(const float d)
{
	m_matrix[0][0] =    d; m_matrix[1][0] = 0.0f; m_matrix[2][0] = 0.0f; m_matrix[3][0] = 0.0f;
	m_matrix[0][1] = 0.0f; m_matrix[1][1] =    d; m_matrix[2][1] = 0.0f; m_matrix[3][1] = 0.0f;
	m_matrix[0][2] = 0.0f; m_matrix[1][2] = 0.0f; m_matrix[2][2] =    d; m_matrix[3][2] = 0.0f;
	m_matrix[0][3] = 0.0f; m_matrix[1][3] = 0.0f; m_matrix[2][3] = 0.0f; m_matrix[3][3] =    d;
}
//------------------------------------------------------------------------------
void CGMatrix4x4::setColumn(const int c, const CGVec4 &v)
{
	for(int i = 0 ; i< 4;i++){
		m_matrix[c][i] = v[i];
	}
}
//------------------------------------------------------------------------------
void CGMatrix4x4::setRow(const int r, const CGVec4 &v)
{
	for(int i = 0 ; i< 4;i++){
		m_matrix[i][r] = v[i];
	}
}
//------------------------------------------------------------------------------
void CGMatrix4x4::identity()
{
	m_matrix[0][0] = 1.0f; m_matrix[1][0] = 0.0f; m_matrix[2][0] = 0.0f; m_matrix[3][0] = 0.0f;
	m_matrix[0][1] = 0.0f; m_matrix[1][1] = 1.0f; m_matrix[2][1] = 0.0f; m_matrix[3][1] = 0.0f;
	m_matrix[0][2] = 0.0f; m_matrix[1][2] = 0.0f; m_matrix[2][2] = 1.0f; m_matrix[3][2] = 0.0f;
	m_matrix[0][3] = 0.0f; m_matrix[1][3] = 0.0f; m_matrix[2][3] = 0.0f; m_matrix[3][3] = 1.0f;
}
//------------------------------------------------------------------------------
CGMatrix4x4 CGMatrix4x4::operator*(const CGMatrix4x4& b) const
{
	CGMatrix4x4 m;
	for (int i = 0; i<4; i++)
		for (int j = 0; j<4; j++) {
			m.m_matrix[i][j] = 0.0f;
			for (int k = 0; k<4; k++)
				m.m_matrix[i][j] += m_matrix[k][j] * b.m_matrix[i][k];
		}
	return m;
}
//------------------------------------------------------------------------------
void CGMatrix4x4::print(const char *prefix) const
{
	if (prefix == NULL)
		prefix = "";
	printf("%s[%f %f %f %f]\n", prefix, m_matrix[0][0], m_matrix[1][0], m_matrix[2][0], m_matrix[3][0]);
	printf("%s[%f %f %f %f]\n", prefix, m_matrix[0][1], m_matrix[1][1], m_matrix[2][1], m_matrix[3][1]);
	printf("%s[%f %f %f %f]\n", prefix, m_matrix[0][2], m_matrix[1][2], m_matrix[2][2], m_matrix[3][2]);
	printf("%s[%f %f %f %f]\n", prefix, m_matrix[0][3], m_matrix[1][3], m_matrix[2][3], m_matrix[3][3]);
}
//------------------------------------------------------------------------------
CGVec4 CGMatrix4x4::operator*(const CGVec4& b) const
{
	CGVec4 c;
	for (int i = 0; i<4; i++) {
		c[i] = 0.0f;
		for (int j = 0; j<4; j++)
			c[i] += m_matrix[j][i] * b[j];
	}
	return c;
}
//------------------------------------------------------------------------------
void CGMatrix4x4::transpose()
{
	int i, j;
	for (i = 0; i<4; i++) {
		for (j = 0; j<i; j++) {
			float tmp;
			tmp = m_matrix[i][j];
			m_matrix[i][j] = m_matrix[j][i];
			m_matrix[j][i] = tmp;
		}
	}
}
//------------------------------------------------------------------------------
void CGMatrix4x4::invert()
{
#define m(a,b) m_matrix[b][a]
#define det3x3(a1, a2, a3, b1, b2, b3, c1, c2, c3) (a1 * (b2 * c3 - b3 * c2) + b1 * (c2 * a3 - a2 * c3) + c1 * (a2 * b3 - a3 * b2))
	const float cof00 = det3x3(m(1, 1), m(1, 2), m(1, 3), m(2, 1), m(2, 2), m(2, 3), m(3, 1), m(3, 2), m(3, 3));
	const float cof01 = -det3x3(m(1, 2), m(1, 3), m(1, 0), m(2, 2), m(2, 3), m(2, 0), m(3, 2), m(3, 3), m(3, 0));
	const float cof02 = det3x3(m(1, 3), m(1, 0), m(1, 1), m(2, 3), m(2, 0), m(2, 1), m(3, 3), m(3, 0), m(3, 1));
	const float cof03 = -det3x3(m(1, 0), m(1, 1), m(1, 2), m(2, 0), m(2, 1), m(2, 2), m(3, 0), m(3, 1), m(3, 2));
	const float inv_det = 1.0f / (m(0, 0) * cof00 + m(0, 1) * cof01 + m(0, 2) * cof02 + m(0, 3) * cof03);
	const float cof10 = -det3x3(m(2, 1), m(2, 2), m(2, 3), m(3, 1), m(3, 2), m(3, 3), m(0, 1), m(0, 2), m(0, 3));
	const float cof11 = det3x3(m(2, 2), m(2, 3), m(2, 0), m(3, 2), m(3, 3), m(3, 0), m(0, 2), m(0, 3), m(0, 0));
	const float cof12 = -det3x3(m(2, 3), m(2, 0), m(2, 1), m(3, 3), m(3, 0), m(3, 1), m(0, 3), m(0, 0), m(0, 1));
	const float cof13 = det3x3(m(2, 0), m(2, 1), m(2, 2), m(3, 0), m(3, 1), m(3, 2), m(0, 0), m(0, 1), m(0, 2));
	const float cof20 = det3x3(m(3, 1), m(3, 2), m(3, 3), m(0, 1), m(0, 2), m(0, 3), m(1, 1), m(1, 2), m(1, 3));
	const float cof21 = -det3x3(m(3, 2), m(3, 3), m(3, 0), m(0, 2), m(0, 3), m(0, 0), m(1, 2), m(1, 3), m(1, 0));
	const float cof22 = det3x3(m(3, 3), m(3, 0), m(3, 1), m(0, 3), m(0, 0), m(0, 1), m(1, 3), m(1, 0), m(1, 1));
	const float cof23 = -det3x3(m(3, 0), m(3, 1), m(3, 2), m(0, 0), m(0, 1), m(0, 2), m(1, 0), m(1, 1), m(1, 2));
	const float cof30 = -det3x3(m(0, 1), m(0, 2), m(0, 3), m(1, 1), m(1, 2), m(1, 3), m(2, 1), m(2, 2), m(2, 3));
	const float cof31 = det3x3(m(0, 2), m(0, 3), m(0, 0), m(1, 2), m(1, 3), m(1, 0), m(2, 2), m(2, 3), m(2, 0));
	const float cof32 = -det3x3(m(0, 3), m(0, 0), m(0, 1), m(1, 3), m(1, 0), m(1, 1), m(2, 3), m(2, 0), m(2, 1));
	const float cof33 = det3x3(m(0, 0), m(0, 1), m(0, 2), m(1, 0), m(1, 1), m(1, 2), m(2, 0), m(2, 1), m(2, 2));
#undef det3x3
#undef m
	m_matrix[0][0] = cof00 * inv_det; m_matrix[1][0] = cof10 * inv_det; m_matrix[2][0] = cof20 * inv_det; m_matrix[3][0] = cof30 * inv_det;
	m_matrix[0][1] = cof01 * inv_det; m_matrix[1][1] = cof11 * inv_det; m_matrix[2][1] = cof21 * inv_det; m_matrix[3][1] = cof31 * inv_det;
	m_matrix[0][2] = cof02 * inv_det; m_matrix[1][2] = cof12 * inv_det; m_matrix[2][2] = cof22 * inv_det; m_matrix[3][2] = cof32 * inv_det;
	m_matrix[0][3] = cof03 * inv_det; m_matrix[1][3] = cof13 * inv_det; m_matrix[2][3] = cof23 * inv_det; m_matrix[3][3] = cof33 * inv_det;
}
