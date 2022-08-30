#include "stdafx.h"
#include "TileMgr.h"
#include "Tile.h"


void CTileMgr::Add_Tile_Path(const wstring & pPathFile, int iX, int iY)
{
	HANDLE hFile = CreateFile(pPathFile.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
		return;
	Release_TileMgr();
	m_vecTile.reserve(iX*iY);
	DWORD dwByte = 0;
	TILEINFO tTemp = {};

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(TILEINFO), &dwByte, NULL);
		if (dwByte == 0)
			break;
		m_vecTile.emplace_back(CTile::Create_Tile(tTemp));
	}

	CloseHandle(hFile);
}

void CTileMgr::Render_TileMgr()
{
	int iScrollX = abs((int)CScrollMgr::Get_Instance()->Get_Scroll().x);
	int iScrollY = abs((int)CScrollMgr::Get_Instance()->Get_Scroll().y);

	int iCullX = iScrollX / 20;
	int iCullY = iScrollY / 20;

	int iCullEndX = iCullX + (WINCX / 20) + 2;
	int iCullEndY = iCullY + (WINCY / 20) + 2;

	for (int i = iCullY; i < iCullEndY; ++i)
	{
		for (int j = iCullX; j < iCullEndX; ++j)
		{
			int iIdx = i * CSceneMgr::Get_Instance()->Get_TileArray().x + j;

			if (0 > iIdx || m_vecTile.size() <= (size_t)iIdx)
				continue;

			m_vecTile[iIdx]->Render_Tile();
		}
	}
}

void CTileMgr::Release_TileMgr()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<CTile*>);
	m_vecTile.clear();
}

CTileMgr::CTileMgr()
{
}


CTileMgr::~CTileMgr()
{
	Release_TileMgr();
}
