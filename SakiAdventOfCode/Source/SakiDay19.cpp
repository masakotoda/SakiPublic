#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay19.h"


SakiDay19::SakiDay19()
{
}

SakiDay19::~SakiDay19()
{
}

void SakiDay19::compute(int part)
{
	// I'm not sure if I'm solving this correctly or being super lucky to get the correct answer...
	SakiDay19 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay19::compute1()
{
	std::unordered_map<std::string, std::vector<std::string> > dict;
	dict["Al"] = std::vector<std::string>{ "ThF", "ThRnFAr" };
	dict["B"] = std::vector<std::string>{ "BCa", "TiB", "TiRnFAr" };
	dict["Ca"] = std::vector<std::string>{ "CaCa", "PB", "PRnFAr", "SiRnFYFAr", "SiRnMgAr", "SiTh" };
	dict["F"] = std::vector<std::string>{ "CaF", "PMg", "SiAl" };
	dict["H"] = std::vector<std::string>{ "CRnAlAr", "CRnFYFYFAr", "CRnFYMgAr", "CRnMgYFAr", "HCa", "NRnFYFAr", "NRnMgAr", "NTh", "OB", "ORnFAr" };
	dict["Mg"] = std::vector<std::string>{ "BF", "TiMg" };
	dict["N"] = std::vector<std::string>{ "CRnFAr", "HSi" };
	dict["O"] = std::vector<std::string>{ "CRnFYFAr", "CRnMgAr", "HP", "NRnFAr", "OTi" };
	dict["P"] = std::vector<std::string>{ "CaP", "PTi", "SiRnFAr" };
	dict["Si"] = std::vector<std::string>{ "CaSi" };
	dict["Th"] = std::vector<std::string>{ "ThCa" };
	dict["Ti"] = std::vector<std::string>{ "BP", "TiTi" };
	dict["e"] = std::vector<std::string>{ "HF", "NAl", "OMg" };

	std::unordered_set<std::string> result;
	const std::string text = "CRnSiRnCaPTiMgYCaPTiRnFArSiThFArCaSiThSiThPBCaCaSiRnSiRnTiTiMgArPBCaPMgYPTiRnFArFArCaSiRnBPMgArPRnCaPTiRnFArCaSiThCaCaFArPBCaCaPTiTiRnFArCaSiRnSiAlYSiThRnFArArCaSiRnBFArCaCaSiRnSiThCaCaCaFYCaPTiBCaSiThCaSiThPMgArSiRnCaPBFYCaCaFArCaCaCaCaSiThCaSiRnPRnFArPBSiThPRnFArSiRnMgArCaFYFArCaSiRnSiAlArTiTiTiTiTiTiTiRnPMgArPTiTiTiBSiRnSiAlArTiTiRnPMgArCaFYBPBPTiRnSiRnMgArSiThCaFArCaSiThFArPRnFArCaSiRnTiBSiThSiRnSiAlYCaFArPRnFArSiThCaFArCaCaSiThCaCaCaSiRnPRnCaFArFYPMgArCaPBCaPBSiRnFYPBCaFArCaSiAl";
	for (auto& x : dict)
	{
		size_t pos = -1;
		while (true)
		{
			pos = text.find(x.first, pos + 1);
			if (pos != std::string::npos)
			{
				for (auto& r : x.second)
				{
					std::string replaced;
					replaced.assign(text.begin(), text.begin() + pos);
					replaced += r;
					replaced.append(text.begin() + pos + x.first.length(), text.end());
					result.insert(replaced);
				}
			}
			else
			{
				break;
			}
		}
	}

	std::cout << result.size() << std::endl;
}

static void replace(const std::string& text, int pos, int len, std::string& r, std::string& replaced)
{
	replaced.reserve(text.length() + r.length() - len);
	replaced.assign(text.begin(), text.begin() + pos);
	replaced += r;
	replaced.append(text.begin() + pos + len, text.end());
}

static void iter(const std::string& cur, int& depth, std::unordered_map<std::string, std::string>& dict, std::set<int>& setDepth)
{
	for (auto x1 : dict)
	{
		size_t pos = -1;
		while (setDepth.size() == 0)
		{
			pos = cur.find(x1.first, pos + 1);
			if (pos == std::string::npos)
			{
				break;
			}

			std::string newcur;
			replace(cur, pos, x1.first.length(), x1.second, newcur);
			if (newcur == "e")
			{
				setDepth.insert(depth);
			}
			else
			{
				++depth;
				iter(newcur, depth, dict, setDepth);
				--depth;
			}
		}
	}
}

void SakiDay19::compute2()
{
	/*
	Replace as follows:
	Th->T
	Al->L
	Ti->I
	Si->J
	Ar->A
	Ca->D
	Mg->M
	Rn->R
	*/

	std::unordered_map<std::string, std::string> dict;
	dict["TF"] = "L";
	dict["TRFA"] = "L";
	dict["BD"] = "B";
	dict["IB"] = "B";
	dict["IRFA"] = "B";
	dict["DD"] = "D";
	dict["PB"] = "D";
	dict["PRFA"] = "D";
	dict["JRFYFA"] = "D";
	dict["JRMA"] = "D";
	dict["JT"] = "D";
	dict["DF"] = "F";
	dict["PM"] = "F";
	dict["JL"] = "F";
	dict["CRLA"] = "H";
	dict["CRFYFYFA"] = "H";
	dict["CRFYMA"] = "H";
	dict["CRMYFA"] = "H";
	dict["HD"] = "H";
	dict["NRFYFA"] = "H";
	dict["NRMA"] = "H";
	dict["NT"] = "H";
	dict["OB"] = "H";
	dict["ORFA"] = "H";
	dict["BF"] = "M";
	dict["IM"] = "M";
	dict["CRFA"] = "N";
	dict["HJ"] = "N";
	dict["CRFYFA"] = "O";
	dict["CRMA"] = "O";
	dict["HP"] = "O";
	dict["NRFA"] = "O";
	dict["OI"] = "O";
	dict["DP"] = "P";
	dict["PI"] = "P";
	dict["JRFA"] = "P";
	dict["DJ"] = "J";
	dict["TD"] = "T";
	dict["BP"] = "I";
	dict["II"] = "I";
	dict["HF"] = "e";
	dict["NL"] = "e";
	dict["OM"] = "e";

	const std::string text = "CRJRDPIMYDPIRFAJTFADJTJTPBDDJRJRIIMAPBDPMYPIRFAFADJRBPMAPRDPIRFADJTDDFAPBDDPIIRFADJRJLYJTRFAADJRBFADDJRJTDDDFYDPIBDJTDJTPMAJRDPBFYDDFADDDDJTDJRPRFAPBJTPRFAJRMADFYFADJRJLAIIIIIIIRPMAPIIIBJRJLAIIRPMADFYBPBPIRJRMAJTDFADJTFAPRFADJRIBJTJRJLYDFAPRFAJTDFADDJTDDDJRPRDFAFYPMADPBDPBJRFYPBDFADJL";

	int depth = 1;
	std::set<int> setDepth;

	iter(text, depth, dict, setDepth);

	auto it = setDepth.begin();
	std::cout << *it << std::endl;
}
