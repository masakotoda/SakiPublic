#pragma once
class SakiDay04
{
public:
	SakiDay04();
	~SakiDay04();
	static void compute(int part);

private:
	void compute1();
	void compute2();

	void findHash(unsigned char mask);
};

