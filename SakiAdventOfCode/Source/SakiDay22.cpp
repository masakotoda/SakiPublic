#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay22.h"

// It's pretty pathetic codes but gives me right answers...

SakiDay22::SakiDay22()
{
}

SakiDay22::~SakiDay22()
{
}

void SakiDay22::compute(int part)
{
	SakiDay22 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

struct gameContext
{
	int totalMana = 0;
	int me = 50;
	int myArmor = 0;
	int myMana = 500;
	int op = 58;
	int myTimer[5] = { 0, 0, 0, 0, 0 };
};

const int myTimerMax[5] = { 0, 0, 6, 6, 5 };
const int cost[5] = { 53, 73, 113, 173, 229 };
enum
{
	misssile,
	drain,
	shield,
	poison,
	recharge
};

static void processTimer(gameContext& theContext)
{
	if (theContext.myTimer[2] > 0)
	{
		theContext.myArmor = 7;
		theContext.myTimer[2]--;
	}
	else
	{
		theContext.myArmor = 0;
	}

	if (theContext.myTimer[3] > 0)
	{
		theContext.op -= 3;
		theContext.myTimer[3]--;
	}

	if (theContext.myTimer[4] > 0)
	{
		theContext.myMana += 101;
		theContext.myTimer[4]--;
	}
}

static void processOpponent(gameContext& theContext)
{
	theContext.me -= 9;
	theContext.me += theContext.myArmor;
}

static int processMe(int i, gameContext& theContext)
{
	if (theContext.myTimer[i] != 0)
		return 0;

	if (cost[i] > theContext.myMana)
		return 0;

	theContext.myMana -= cost[i];
	switch (i)
	{
	case 0:
		theContext.op -= 4;
		break;
	case 1:
		theContext.op -= 2;
		theContext.me += 2;
		break;
	default:
		theContext.myTimer[i] = myTimerMax[i];
		break;
	}

	return cost[i];
}

static void iter(int hardParam, gameContext& theContext, std::set<int>& setMana)
{
	for (int i1 = 0; i1 < 5; i1++)
	{
		gameContext prev = theContext;

		if (theContext.me > hardParam)
		{
			int manaSpent = processMe(i1, theContext);
			if (manaSpent > 0)
			{
				theContext.me -= hardParam;
				theContext.totalMana += manaSpent;

				if (theContext.op <= 0)
				{
					setMana.insert(theContext.totalMana);
					theContext = prev;
				}
				else
				{
					processTimer(theContext);
					if (theContext.op <= 0)
					{
						setMana.insert(theContext.totalMana);
						theContext = prev;
					}
					else if (theContext.me <= hardParam)
					{
						theContext = prev;
					}
					else
					{
						theContext.me -= hardParam;
						processOpponent(theContext);
						if (theContext.op <= 0)
						{
							setMana.insert(theContext.totalMana);
							theContext = prev;
						}
						else if (theContext.me <= 0)
						{
							theContext = prev;
						}
						else
						{
							processTimer(theContext);
							if (theContext.op <= 0)
							{
								setMana.insert(theContext.totalMana);
								theContext = prev;
							}
							else if (theContext.me <= 0)
							{
								theContext = prev;
							}
							else if (theContext.me > 0)
							{
								iter(hardParam, theContext, setMana);
								theContext = prev;
							}
						}
					}
				}
			}
		}
	}
}

void SakiDay22::compute1()
{
	std::set<int> setMana;
	gameContext theContext;
	iter(0, theContext, setMana);
	if (setMana.size() > 0)
	{
		int minimum = *setMana.begin();
		std::cout << minimum << std::endl;
	}
}

void SakiDay22::compute2()
{
	std::set<int> setMana;
	gameContext theContext;
	iter(1, theContext, setMana);
	if (setMana.size() > 0)
	{
		int minimum = *setMana.begin();
		std::cout << minimum << std::endl;
	}
}
