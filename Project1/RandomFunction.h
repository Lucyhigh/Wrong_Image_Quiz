#pragma once
#include"SingletonBase.h"

class RandomFunction : public SingletonBase <RandomFunction>
{
public:
	RandomFunction() 
	{
		srand(GetTickCount());
	}
	~RandomFunction(){}

	HRESULT init() { return S_OK; }

	inline int getlnt(int num)
	{
		return rand() % num;
	}
		
	inline int getFromintTo(int fromNum, int toNum)
	{
		return rand() % (toNum - fromNum + 1) + fromNum;
	}
	
	float getFloat(void)
	{
		return(float)rand() / (float)RAND_MAX;
	}

	float getFloat(float num)
	{
		return(float)rand() / (float)RAND_MAX*num;
	}

	float getFromFloatTo(float fromNum,float toNum)
	{
		float rnd = (float)rand() / (float)RAND_MAX;
		return(rnd*(toNum - fromNum) + fromNum);
	}
};