#pragma once
#ifndef __TILEMGR_H__
#define __TILEMGR_H__

#include "ToolSingleton.h"
class CToolTile;
class CToolTileMgr :
	public CToolSingleton<CToolTileMgr>
{
public:
	CToolTileMgr();
	virtual ~CToolTileMgr();

public:
	void TileSetting(int x, int y);
	void LoadTile(const wstring& pFilePath);
	void RenderTile();
	void ReleaseTile();
	void ShirnkTile();
	_vec2 Get_Size() { return vSize; }
	void Chagne(const _vec3 vMouse, int DrawID, int iOptionID);
	vector<CToolTile*>	Get_vecTile() { return m_vecTile; }
private:
	vector<CToolTile*>	m_vecTile;

	_vec2 vSize;
};


#endif // !__TILEMGR_H__
