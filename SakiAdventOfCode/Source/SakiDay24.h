#pragma once
class SakiDay24
{
public:
	struct SakiDay24Context
	{
		int goal = 0;
		std::vector<int> containers;
		std::set<long long unsigned> quantumE;
	};

public:
	SakiDay24();
	~SakiDay24();
	static void compute(int part);

private:
	void compute1();
	void compute2();
	void compute(SakiDay24Context& context);
};

