#pragma once

class CSakiColorGameTcpClient;

class CSakiColorInfo
{
public:
	virtual void Initialize() = 0;

	CString m_strName;
	bool m_bRequireRed;
	bool m_bRequireGreen;
	bool m_bRequireBlue;
	CBrush m_brush;
};

class CSakiColorRed : public CSakiColorInfo
{
public:
	virtual void Initialize() override;
};

class CSakiColorGreen : public CSakiColorInfo
{
public:
	virtual void Initialize() override;
};

class CSakiColorBlue : public CSakiColorInfo
{
public:
	virtual void Initialize() override;
};

class CSakiColorRG : public CSakiColorInfo
{
public:
	virtual void Initialize() override;
};

class CSakiColorRB : public CSakiColorInfo
{
public:
	virtual void Initialize() override;
};

class CSakiColorGB : public CSakiColorInfo
{
public:
	virtual void Initialize() override;
};


class CSakiColorGameClientEngine
{
public:
	CSakiColorGameClientEngine(void);
	~CSakiColorGameClientEngine(void);

	void UpdateState();
	CString GetStatus();
	CString GetMessage();
	void StartGame();
	void Draw(CDC* pDC);
	void Click(CPoint pt);
	void ToggleRed();
	void ToggleBlue();
	void ToggleGreen();
	bool HasRed();
	bool HasBlue();
	bool HasGreen();

	enum
	{
		countCell = 8,
		countColors = 6,
	};

	struct ColorCell
	{
		int m_colorIndex;
		bool m_colored;
	};

	struct ColorRow
	{
		ColorCell cells[countCell];
	};

	ColorRow m_rows[countCell];
	CSakiColorInfo* m_brushes[countColors];

	bool WaitForResponse(char* response, int response_size);

	bool HasColor(int index);

	bool m_bHasRed;
	bool m_bHasGreen;
	bool m_bHasBlue;

	CSakiColorGameTcpClient* m_pClient;

	CString m_strLastStatus;
	CString m_strLastMessage;
};

