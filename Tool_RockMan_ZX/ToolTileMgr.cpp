#include "stdafx.h"
#include "ToolTileMgr.h"
#include "ToolTile.h"

CToolTileMgr::CToolTileMgr()
{
	ZeroMemory(&vSize, sizeof(vSize));
}


CToolTileMgr::~CToolTileMgr()
{
	ReleaseTile();
}

void CToolTileMgr::TileSetting(int x, int y)
{
	if (!m_vecTile.empty())
		ReleaseTile();

	m_vecTile.reserve(x*y);
	vSize = { (float)x, (float)y };

	for (int i = 0; i < y; ++i)
	{
		for(int j= 0; j < x ; j++)
		{
			float fX = (float)(TILECX >> 1) + (j * TILECX);
			float fY = (float)(TILECY >> 1) + (i * TILECY);
			m_vecTile.emplace_back(CToolTile::Create(fX, fY));
		}
	}

}

void CToolTileMgr::LoadTile(const wstring & pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
		return;
	ReleaseTile();
	DWORD dwByte = 0;
	TILEINFO tTemp = {};

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(TILEINFO), &dwByte, NULL);
		if (dwByte == 0)
			break;
		m_vecTile.emplace_back(CToolTile::Create_Tile(tTemp));
	}
	ShirnkTile();
	CloseHandle(hFile);
}

void CToolTileMgr::RenderTile()
{
	for (auto iter : m_vecTile)
		iter->Render();
}

void CToolTileMgr::ReleaseTile()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CToolTile*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CToolTileMgr::ShirnkTile()
{
	m_vecTile.shrink_to_fit();
}

void CToolTileMgr::Chagne(const _vec3 vMouse, int DrawID, int iOptionID)
{
	int x = int(vMouse.x / 20);
	int y = int(vMouse.y / 20);

	int iIdx = y * vSize.x + x;

	if (0 > iIdx || m_vecTile.size() <= (size_t)iIdx)
		return;

	m_vecTile[iIdx]->Change(DrawID, iOptionID);
}
