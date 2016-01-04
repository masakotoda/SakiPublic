#pragma once
class SakiDay06
{
public:
	SakiDay06();
	~SakiDay06();
	static void compute(int part);

private:
	void compute1();
	void compute2();
	void compute(
		std::function<void(unsigned short&)> fnToggle,
		std::function<void(unsigned short&)> fnTurnOn,
		std::function<void(unsigned short&)> fnTurnOff);
};

