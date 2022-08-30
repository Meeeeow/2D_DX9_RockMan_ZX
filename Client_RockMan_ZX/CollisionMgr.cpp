#include "stdafx.h"
#include "CollisionMgr.h"
#include "ObjectBase.h"
#include "Player.h"
#include "Tile.h"



bool CCollisionMgr::TileCollision(CObjectBase* pObject, float * _y)
{
	for (auto iter : CTileMgr::Get_Instance()->Get_Tile())
	{
		auto pTileInfo = static_cast<CTile*>(iter)->Get_TileInfo();
		auto pTileRect = static_cast<CTile*>(iter)->Get_TileRect();
		RECT rc = {};
		if (pTileRect.left <= pObject->Get_Info().vPos.x && pTileRect.right >= pObject->Get_Info().vPos.x
			&& pTileInfo.vPos.y + 10 >= pObject->Get_Info().vPos.y + 20
			&& pTileInfo.vPos.y - 10 <= pObject->Get_Info().vPos.y + 20
			&& pTileInfo.iDrawID == 1)
		{
			float x1 = pTileRect.left;
			float x2 = pTileRect.right;
			float y1 = pTileRect.top;

			*_y = ((y1 - y1) / (x2 - x1)) * (pObject->Get_Info().vPos.x - x1) + y1 - 20;
			return true;
		}
		else if (pTileRect.left <= pObject->Get_Info().vPos.x && pTileRect.right >= pObject->Get_Info().vPos.x
			&& pTileInfo.vPos.y + 10 >= pObject->Get_Info().vPos.y + 20
			&& pTileInfo.vPos.y - 10 <= pObject->Get_Info().vPos.y + 20
			&& pTileInfo.iDrawID == 4)
		{
			float x1 = pTileRect.left;
			float x2 = pTileRect.right;
			float y1 = pTileRect.top;

			*_y = ((y1 - y1) / (x2 - x1)) * (pObject->Get_Info().vPos.x - x1) + y1 - 14;
			return true;
		}
		else if (pTileRect.left <= pObject->Get_Info().vPos.x && pTileRect.right >= pObject->Get_Info().vPos.x
			&& pTileInfo.vPos.y + 10 >= pObject->Get_Info().vPos.y + 20
			&& pTileInfo.vPos.y - 10 <= pObject->Get_Info().vPos.y + 20
			&& pTileInfo.iDrawID == 5)
		{
			float x1 = pTileRect.left;
			float x2 = pTileRect.right;
			float y1 = pTileRect.top;

			*_y = ((y1 - y1) / (x2 - x1)) * (pObject->Get_Info().vPos.x - x1) + y1 - 16;
			return true;
		}
		else if (pTileRect.left <= pObject->Get_Info().vPos.x && pTileRect.right >= pObject->Get_Info().vPos.x
			&& pTileInfo.vPos.y + 10 >= pObject->Get_Info().vPos.y + 20
			&& pTileInfo.vPos.y - 10 <= pObject->Get_Info().vPos.y + 20
			&& pTileInfo.iDrawID == 6)
		{
			float x1 = pTileRect.left;
			float x2 = pTileRect.right;
			float y1 = pTileRect.top;

			*_y = ((y1 - y1) / (x2 - x1)) * (pObject->Get_Info().vPos.x - x1) + y1 - 18;
			return true;
		}
		else if ( IntersectRect(&rc, &pTileRect,&pObject->Get_Rect())
			&& pTileInfo.iDrawID == 7)
		{
			float x1 = pTileRect.left;
			float x2 = pTileRect.right;
			float y1 = pTileRect.top;
			if(pObject->Get_Info().bPlayer)
				pObject->Default_Hit(10, pTileRect);

			if(pTileInfo.vPos.y < 300.f)
				*_y = ((y1 - y1) / (x2 - x1)) * (pObject->Get_Info().vPos.x - x1) + y1 + 40;

			else if (pTileInfo.vPos.y > 300.f)
				*_y = ((y1 - y1) / (x2 - x1)) * (pObject->Get_Info().vPos.x - x1) + y1 - 20;
			return true;
		}
	}
	return false;
}

bool CCollisionMgr::TileCollisionSerpant(CObjectBase * pObject, float * _y)
{
	for (auto iter : CTileMgr::Get_Instance()->Get_Tile())
	{
		auto pTileInfo = static_cast<CTile*>(iter)->Get_TileInfo();
		auto pTileRect = static_cast<CTile*>(iter)->Get_TileRect();
		RECT rc = {};
		if (pTileRect.left <= pObject->Get_Info().vPos.x && pTileRect.right >= pObject->Get_Info().vPos.x
			&& pTileInfo.vPos.y + 10 >= pObject->Get_Info().vPos.y + 20
			&& pTileInfo.vPos.y - 10 <= pObject->Get_Info().vPos.y + 20
			&& pTileInfo.iDrawID == 1)
		{
			float x1 = pTileRect.left;
			float x2 = pTileRect.right;
			float y1 = pTileRect.top;

			*_y = ((y1 - y1) / (x2 - x1)) * (pObject->Get_Info().vPos.x - x1) + y1 - 40;
			return true;
		}
		return false;
	}
}

void CCollisionMgr::WallTileCollision(CObjectBase * pObject)
{
	RECT rc = {};
	for (auto iter : CTileMgr::Get_Instance()->Get_Tile())
	{
		auto pTileInfo = static_cast<CTile*>(iter)->Get_TileInfo();
		auto pTileRect = static_cast<CTile*>(iter)->Get_TileRect();
		if((pTileInfo.iDrawID == 2 || pTileInfo.iDrawID == 3)
			&& IntersectRect(&rc, &pTileRect, &pObject->Get_Rect()))
		{
			float fX = abs(pTileInfo.vPos.x - pObject->Get_Info().vPos.x);
			if (pObject->Get_Info().vPos.x > pTileInfo.vPos.x)
				pObject->Set_PosX(fX / 5);
			else
				pObject->Set_PosX(-fX / 5);
		}
	}
}

void CCollisionMgr::WallClimbCollision(CObjectBase * pObject)
{
	RECT rc = {};
	for (auto iter : CTileMgr::Get_Instance()->Get_Tile())
	{
		auto pTileInfo = static_cast<CTile*>(iter)->Get_TileInfo();
		auto pTileRect = static_cast<CTile*>(iter)->Get_TileRect();
		if (pTileInfo.iDrawID == 2
			&& IntersectRect(&rc, &pTileRect, &pObject->Get_Rect()))
		{
			static_cast<CPlayer*>(pObject)->WallClimbActive();
			static_cast<CPlayer*>(pObject)->CanAirDashInActive();
			static_cast<CPlayer*>(pObject)->AirDashInActive();
			static_cast<CPlayer*>(pObject)->AirDashAttackInActive();
			static_cast<CPlayer*>(pObject)->DashInActive();
			static_cast<CPlayer*>(pObject)->WalkInActive();
			//static_cast<CPlayer*>(pObject)->JumpInActive();

			if (pTileRect.right >= pObject->Get_Rect().right
				&& pTileRect.left >= pObject->Get_Rect().left)
			{
				pObject->Setting_PosX(pTileRect.left - 9.9f);
				static_cast<CPlayer*>(pObject)->HitBbox_Update();
				static_cast<CPlayer*>(pObject)->Set_Direction(-1);
				static_cast<CPlayer*>(pObject)->WallClimbRightActive();
				static_cast<CPlayer*>(pObject)->WallClimbLeftInActive();
			}

			if (pTileRect.right <= pObject->Get_Rect().right
				&& pTileRect.left <= pObject->Get_Rect().left)
			{
				pObject->Setting_PosX(pTileRect.right + 9.9f);
				static_cast<CPlayer*>(pObject)->HitBbox_Update();
				static_cast<CPlayer*>(pObject)->Set_Direction(1);
				static_cast<CPlayer*>(pObject)->WallClimbLeftActive();
				static_cast<CPlayer*>(pObject)->WallClimbRightInActive();
			}
			
			/*if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
			{
				pObject->Set_PosX(-10);
				static_cast<CPlayer*>(pObject)->HitBbox_Update();
				static_cast<CPlayer*>(pObject)->WallClimbRightInActive();
				static_cast<CPlayer*>(pObject)->WallClimbInActive();
				static_cast<CPlayer*>(pObject)->HitBbox_Update();
				return;
			}
			if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
			{
				pObject->Set_PosX(10);
				static_cast<CPlayer*>(pObject)->HitBbox_Update();
				static_cast<CPlayer*>(pObject)->WallClimbLeftInActive();
				static_cast<CPlayer*>(pObject)->WallClimbInActive();
				static_cast<CPlayer*>(pObject)->HitBbox_Update();
				return;
			}*/
		}
	}
}

void CCollisionMgr::FalseWallCollision(CObjectBase * pObject)
{
	RECT rc = {};
	for (auto iter : CTileMgr::Get_Instance()->Get_Tile())
	{
		auto pTileInfo = static_cast<CTile*>(iter)->Get_TileInfo();
		auto pTileRect = static_cast<CTile*>(iter)->Get_TileRect();
		if (pTileInfo.iDrawID == 3
			&& IntersectRect(&rc, &pTileRect, &pObject->Get_Rect()))
		{
			float fX = abs(pTileInfo.vPos.x - pObject->Get_Info().vPos.x);
			if (pObject->Get_Info().vPos.x > pTileInfo.vPos.x)
				pObject->Set_PosX(fX / 5);
			else
				pObject->Set_PosX(-fX / 5);
		}
	}
}

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}
