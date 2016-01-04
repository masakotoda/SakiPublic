#include "stdafx.h"
#include <windows.h>
#include <wincrypt.h>
#include "SakiFileLoader.h"
#include "SakiDay04.h"


SakiDay04::SakiDay04()
{
}

SakiDay04::~SakiDay04()
{
}

void SakiDay04::compute(int part)
{
	SakiDay04 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay04::compute1()
{
	findHash(0xf0);
}

void SakiDay04::compute2()
{
	findHash(0xff);
}

void SakiDay04::findHash(unsigned char mask)
{
	const char* in = "ckczppom";
	const DWORD maxHashLen = 16;

	HCRYPTPROV hCryptProv;
	if (CryptAcquireContext(&hCryptProv,
		NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET))
	{
		const int bufferSize = 63;
		char buffer[bufferSize + 1] = { 0 };

		for (int i = 0; ; i++)
		{
			sprintf_s(buffer, bufferSize, "%s%d", in, i);

			DWORD cbContent = strlen(buffer);
			const BYTE* pbContent = (const BYTE*)buffer;

			HCRYPTHASH hHash;
			if (CryptCreateHash(hCryptProv,
				CALG_MD5,
				0, 0, &hHash))
			{
				BYTE bHash[maxHashLen];
				DWORD dwHashLen = 16;
				if (CryptHashData(hHash, pbContent, cbContent, 0))
				{
					if (CryptGetHashParam(hHash, HP_HASHVAL, bHash, &dwHashLen, 0))
						;
					else
						throw;
				}
				else
					throw;

				if (bHash[0] == 0 && bHash[1] == 0 && (bHash[2] & mask) == 0)
				{
					std::cout << i << std::endl;
					break;
				}
			}
			CryptDestroyHash(hHash);
		}
	}
	else
		throw;

	CryptReleaseContext(hCryptProv, 0);
}
