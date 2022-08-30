#pragma once
#ifndef __TILE_MGR_H__
#define __TILE_MGR_H__

#include "Singleton.h"
class CTile;
class CTileMgr :
	public CSingleton<CTileMgr>
{
public:
	const vector<CTile*>& Get_Tile() const { return m_vecTile; }
	void Add_Tile_Path(const wstring& pPathFile,int iX, int iY);

	void Render_TileMgr();
	void Release_TileMgr();

public:
	CTileMgr();
	virtual ~CTileMgr();

private:
	vector<CTile*> m_vecTile;
};


#endif // !__TILE_MGR_H__
