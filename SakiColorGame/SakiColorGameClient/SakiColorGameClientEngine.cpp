#include "stdafx.h"
#include "SakiColorGameClientEngine.h"


void CSakiColorRed::Initialize()
{
	m_strName = _T("RED");
	m_bRequireRed = true;
	m_bRequireGreen = false;
	m_bRequireBlue = false;
	m_brush.CreateSolidBrush(RGB(255, 0, 0));
}


void CSakiColorGreen::Initialize()
{
	m_strName = _T("GRN");
	m_bRequireRed = false;
	m_bRequireGreen = true;
	m_bRequireBlue = false;
	m_brush.CreateSolidBrush(RGB(0, 255, 0));
}


void CSakiColorBlue::Initialize()
{
	m_strName = _T("BLU");
	m_bRequireRed = false;
	m_bRequireGreen = false;
	m_bRequireBlue = true;
	m_brush.CreateSolidBrush(RGB(0, 0, 255));
}


void CSakiColorRG::Initialize()
{
	m_strName = _T("R+G");
	m_bRequireRed = true;
	m_bRequireGreen = true;
	m_bRequireBlue = false;
	m_brush.CreateSolidBrush(RGB(255, 255, 0));
}


void CSakiColorRB::Initialize()
{
	m_strName = _T("R+B");
	m_bRequireRed = true;
	m_bRequireGreen = false;
	m_bRequireBlue = true;
	m_brush.CreateSolidBrush(RGB(255, 0, 255));
}


void CSakiColorGB::Initialize()
{
	m_strName = _T("G+B");
	m_bRequireRed = false;
	m_bRequireGreen = true;
	m_bRequireBlue = true;
	m_brush.CreateSolidBrush(RGB(0, 255, 255));
}


CSakiColorGameClientEngine::CSakiColorGameClientEngine(void)
	:
	 m_bHasRed(false)
	, m_bHasGreen(false)
	, m_bHasBlue(false)

{
	srand (time(NULL));

	//Generate number between 0 and 5 (0: Red, 1: Green, 2: Blue, 3: R+G, 4: R+B, 5: G+B)
	for (int i = 0; i < countCell; i++)
	{
		for (int j = 0; j < countCell; j++)
		{
			m_rows[i].cells[j].m_colorIndex = rand() % 6 + 0;
			m_rows[i].cells[j].m_colored = false;
		}
	}

	ASSERT(countColors == 6);
	m_brushes[0] = new CSakiColorRed();
	m_brushes[1] = new CSakiColorGreen();
	m_brushes[2] = new CSakiColorBlue();
	m_brushes[3] = new CSakiColorRG();
	m_brushes[4] = new CSakiColorRB();
	m_brushes[5] = new CSakiColorGB();
	for (auto x : m_brushes)
	{
		x->Initialize();
	}
}


CSakiColorGameClientEngine::~CSakiColorGameClientEngine(void)
{
	for (auto x : m_brushes)
	{
		delete x;
	}
}

void CSakiColorGameClientEngine::Draw(CDC* pDC)
{
	const int scaleFactor = 40;
	for (int i = 0; i < countCell; i++)
	{
		for (int j = 0; j < countCell; j++)
		{
			auto& cell = m_rows[i].cells[j];
			int colorIndex = cell.m_colorIndex;
			bool colored = cell.m_colored;
			if (colored)
			{
				pDC->SelectObject(m_brushes[colorIndex]->m_brush);
				pDC->Rectangle(j * scaleFactor, i * scaleFactor, (j + 1) * scaleFactor, (i + 1) * scaleFactor);
			}
			else
			{
				pDC->TextOut(j * scaleFactor, i * scaleFactor, m_brushes[colorIndex]->m_strName);
			}
		}
	}
}


void CSakiColorGameClientEngine::Click(CPoint pt)
{
	ColorCell* pCell = nullptr;
	const int scaleFactor = 40;
	for (int i = 0; i < countCell && pCell == nullptr; i++)
	{
		auto& row = m_rows[i];
		for (int j = 0; j < countCell && pCell == nullptr; j++)
		{
			auto& cell = row.cells[j];
			if (pt.x >= j * scaleFactor &&
				pt.x < (j + 1) * scaleFactor &&
				pt.y >= i * scaleFactor &&
				pt.y < (i + 1) * scaleFactor)
			{
				pCell = &cell;
			}
		}
	}

	if (pCell != nullptr)
	{
		if (HasColor(pCell->m_colorIndex))
		{
			pCell->m_colored = true;
		}
	}
}


void CSakiColorGameClientEngine::AcquiredRed()
{
	m_bHasRed = m_bHasRed ? false : true;
}



void CSakiColorGameClientEngine::AcquiredBlue()
{
	m_bHasBlue = m_bHasBlue ? false : true;
}



void CSakiColorGameClientEngine::AcquiredGreen()
{
	m_bHasGreen = m_bHasGreen ? false : true;
}

bool CSakiColorGameClientEngine::HasColor(int index)
{
	bool hasColor = false;

	if (m_brushes[index]->m_bRequireRed == m_bHasRed &&
		m_brushes[index]->m_bRequireBlue == m_bHasBlue &&
		m_brushes[index]->m_bRequireGreen == m_bHasGreen)
	{
		hasColor = true;
	}

	return hasColor;
}
