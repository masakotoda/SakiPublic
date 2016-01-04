
#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <cerrno>

#include "SakiFileLoader.h"

namespace SakiFileLoader
{
	std::string GetFileContents(const char *filepath)
	{
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			std::ostringstream contents;
			contents << in.rdbuf();
			in.close();
			return (contents.str());
		}
		throw(errno);
	}

	const char* GetLine(const char* head, std::string& line)
	{
		while (*head != 0 && iscntrl(*head))
		{
			head++;
		}

		const char* tail = head;
		while (*tail != 0 && !iscntrl(*tail))
		{
			tail++;
		}

		if (head != nullptr && head != tail)
		{
			line.assign(head, tail);
			if (*tail == 0)
			{
				tail = nullptr;
			}
			return tail;
		}
		else
		{
			return nullptr;
		}
	}
}
