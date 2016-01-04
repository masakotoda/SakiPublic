#pragma once
class SakiDay16
{
public:
	SakiDay16();
	~SakiDay16();
	static void compute(int part);

private:
	void compute1();
	void compute2();
	void compute(std::function<bool(const char* key, int cnt)> checkSue);

	const std::unordered_map<std::string, int> c_sue = {
		{ "children:", 3 },
		{ "cats:", 7 },
		{ "samoyeds:", 2 },
		{ "pomeranians:", 3 },
		{ "akitas:", 0 },
		{ "vizslas:", 0 },
		{ "goldfish:", 5 },
		{ "trees:", 3 },
		{ "cars:", 2 },
		{ "perfumes:", 1 } };
};

