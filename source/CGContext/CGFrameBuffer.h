#pragma once

//---------------------------------------------------------------------------
struct CGVec4;
struct CGVec2i;
class CGFrameBuffer
{
public:
	bool allocate(int width, int height);
	int getWidth() const;
	int getHeight() const;

	class CGBuffer4UB
	{
	public:
		CGBuffer4UB();
		~CGBuffer4UB();
		bool reserve(int width, int height);
		void set_white(const CGVec2i &p);
		void set(const CGVec2i& p, const CGVec4& rgba);
		CGVec4 get(const CGVec2i& p) const;
		void clear(const CGVec4 &rgba);
		unsigned char* getDataPointer() const;
		int getWidth() const;
		int getHeight() const;
	private:
		int width, height;
		unsigned char* pBufferData;
	} colorBuffer;

	class CGBuffer1f
	{
	public:
		CGBuffer1f();
		~CGBuffer1f();
		bool reserve(int width, int height);
		float get(const CGVec2i& p) const;
		void set(const CGVec2i& p, const float z);
		void clear();
		float* getDataPointer() const;
		int getWidth() const;
		int getHeight() const;
	private:
		int width, height;
		float* pBufferData;
	} depthBuffer;
};

//---------------------------------------------------------------------------

