#pragma once
class SakiDay17
{
public:
	struct SakiDay17Context
	{
		std::vector<int> containers;
		std::map<int, int> counts;
		int count = 0;
	};

public:
	SakiDay17();
	~SakiDay17();
	static void compute(int part);

private:
	void compute1();
	void compute2();
	void compute(SakiDay17Context& context);
};

