#pragma once
class SakiDay09
{
public:
	SakiDay09();
	~SakiDay09();
	static void compute(int part);

private:
	void compute1();
	void compute2();
	void compute(int initMinMax, std::function<int(int, int)> fnMinMax);
};

