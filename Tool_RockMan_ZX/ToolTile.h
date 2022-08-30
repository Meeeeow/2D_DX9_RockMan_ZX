#pragma once
class CToolTile
{
public:
	static CToolTile* Create(float fX, float fY);
	static CToolTile* Create_Tile(TILEINFO tTile);
public:
	CToolTile();
	~CToolTile();
public:
	void Set_Pos(float fX, float fY);
	void Set_All(TILEINFO tTile) { m_tTileInfo = tTile; }
	void Render();
	void Change(int iDrawID, int iOptionID);
	const TILEINFO& Get_Tile() const { return m_tTileInfo; }
private:
	TILEINFO m_tTileInfo;

};


