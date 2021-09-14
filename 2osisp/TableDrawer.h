#pragma once
#include "TextGenerator.h"

#define MAX_ROWS 7
#define MAX_COLUMNS 7

class TableDrawer
{
	const int _horizontalTab = 0;
	int _maxRows = MAX_ROWS;
	int _maxColumns = MAX_COLUMNS;
	RECT _clientSize;
	std::vector<std::wstring> _phrases;
	int _fontHeight = 0, _verticalTab = 0, _totalHeight = 0;
	int _rowCount = 0, _columnCount = 0;
	int _horizontalBorders[MAX_ROWS][2];
	int _verticalBorders[MAX_COLUMNS][2];


	void DrawBorders(HDC hDc);
	void DrawTable(HDC hDc, bool isDraw);
	void PrintPhrases(HDC hDc);
	int RecalculateVerticalBorders();
	void CalculateHorizontalBorders();
	int CalculateVerticalBorders(HDC hDc);

public:
	TableDrawer(int rowCount, int columnCount, std::vector<std::wstring> phrases);

	void RefreshTable(HDC hDc);
	void SetClientSize(RECT clientSize);
};

