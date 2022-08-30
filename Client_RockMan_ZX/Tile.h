#pragma once
#ifndef __TILE_H__
#define __TILE_H__

class CTile
{
public:
	static CTile* Create_Tile(TILEINFO _pTile);
	void Set_Tile(TILEINFO _pTile);
	void Init_Tile();
	void Update_Tile();
	void Update_Tile_Rect();
	void Late_Update_Tile();
	void Render_Tile();
	void Release_Tile();
	TILEINFO	Get_TileInfo() { return m_tTile; }
	RECT		Get_TileRect() { return m_tRect; }
private:
	explicit CTile();
public:
	~CTile();

public:
	TILEINFO	m_tTile;
	RECT		m_tRect;
};


#endif // !__TILE_H__
