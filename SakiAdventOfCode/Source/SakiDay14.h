#pragma once
class SakiDay14
{
public:
	SakiDay14();
	~SakiDay14();
	static void compute(int part);

private:
	void compute1();
	void compute2();
	void compute(
		int& winningDistance,
		int& winningPoint);
};

