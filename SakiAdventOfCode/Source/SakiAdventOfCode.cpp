// SakiAdventOfCode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SakiDay01.h"
#include "SakiDay02.h"
#include "SakiDay03.h"
#include "SakiDay04.h"
#include "SakiDay05.h"
#include "SakiDay06.h"
#include "SakiDay07.h"
#include "SakiDay08.h"
#include "SakiDay09.h"
#include "SakiDay10.h"
#include "SakiDay11.h"
#include "SakiDay12.h"
#include "SakiDay13.h"
#include "SakiDay14.h"
#include "SakiDay15.h"
#include "SakiDay16.h"
#include "SakiDay17.h"
#include "SakiDay18.h"
#include "SakiDay19.h"
#include "SakiDay20.h"
#include "SakiDay21.h"
#include "SakiDay22.h"
#include "SakiDay23.h"
#include "SakiDay24.h"
#include "SakiDay25.h"

int main(int argc, char* argv[])
{
	bool exit = false;
	while (exit == false)
	{
		std::cout << "Enter Day # and Part1. (e.g. 1-1, 1-2, ..., 24-1, 24-2, 25-1)" << std::endl;

		std::string dayAndPart;
		std::cin >> dayAndPart;

		int day = 0;
		int part = 0;
		sscanf_s(dayAndPart.c_str(), "%d-%d", &day, &part);

		switch (day)
		{
		case 1:
			SakiDay01::compute(part);
			break;
		case 2:
			SakiDay02::compute(part);
			break;
		case 3:
			SakiDay03::compute(part);
			break;
		case 4:
			SakiDay04::compute(part);
			break;
		case 5:
			SakiDay05::compute(part);
			break;
		case 6:
			SakiDay06::compute(part);
			break;
		case 7:
			SakiDay07::compute(part);
			break;
		case 8:
			SakiDay08::compute(part);
			break;
		case 9:
			SakiDay09::compute(part);
			break;
		case 10:
			SakiDay10::compute(part);
			break;
		case 11:
			SakiDay11::compute(part);
			break;
		case 12:
			SakiDay12::compute(part);
			break;
		case 13:
			SakiDay13::compute(part);
			break;
		case 14:
			SakiDay14::compute(part);
			break;
		case 15:
			SakiDay15::compute(part);
			break;
		case 16:
			SakiDay16::compute(part);
			break;
		case 17:
			SakiDay17::compute(part);
			break;
		case 18:
			SakiDay18::compute(part);
			break;
		case 19:
			SakiDay19::compute(part);
			break;
		case 20:
			SakiDay20::compute(part);
			break;
		case 21:
			SakiDay21::compute(part);
			break;
		case 22:
			SakiDay22::compute(part);
			break;
		case 23:
			SakiDay23::compute(part);
			break;
		case 24:
			SakiDay24::compute(part);
			break;
		case 25:
			SakiDay25::compute(part);
			break;
		default:
			exit = true;
			break;
		}	
	}
	
	return 0;
}

