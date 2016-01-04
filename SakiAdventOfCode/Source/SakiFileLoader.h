#pragma once

namespace SakiFileLoader
{
	std::string GetFileContents(const char *filepath);
	const char* GetLine(const char* head, std::string& line);
	const std::string s_defaultRoot = "C:\\Users\\Masako\\Documents\\Visual Studio 2012\\Projects\\MFCApplication3\\";
};
