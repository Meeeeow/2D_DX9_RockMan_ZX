#pragma once
class CRandomMgr
{
public:
	CRandomMgr();
	~CRandomMgr();
	static void Range(int min, int max)
	{
		distribution = uniform_int_distribution<int>(min, max);
		generator = bind(distribution, engine);
	}

	static int Random()
	{
		return generator();
	}
private:
	static mt19937 engine;
	static uniform_int_distribution<int> distribution;
	static _Binder<_Unforced, uniform_int_distribution<int>&, mt19937&> generator;
};









