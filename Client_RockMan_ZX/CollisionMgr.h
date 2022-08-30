#pragma once
#ifndef __COLLISION_MGR_H__
#define __COLLISION_MGR_H__

#include "Singleton.h"
class CObjectBase;
class CPlayer;
class CCollisionMgr :
	public CSingleton<CCollisionMgr>
{
public:
	bool TileCollision(CObjectBase* pObject, float* _y);
	bool TileCollisionSerpant(CObjectBase* pObject, float* _y);
	void WallTileCollision(CObjectBase* pObject);
	void WallClimbCollision(CObjectBase* pObject);
	void FalseWallCollision(CObjectBase* pObject);


public:
	CCollisionMgr();
	virtual ~CCollisionMgr();
};


#endif // !__COLLISION_MGR_H__
