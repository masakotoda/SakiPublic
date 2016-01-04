#pragma once
class SakiDay05
{
public:
	SakiDay05();
	~SakiDay05();
	static void compute(int part);

private:
	void compute1();
	void compute2();
	void compute(std::function<bool(const std::string&)> fnCheck);
	static bool check1(const std::string& line);
	static bool check2(const std::string& line);
};

