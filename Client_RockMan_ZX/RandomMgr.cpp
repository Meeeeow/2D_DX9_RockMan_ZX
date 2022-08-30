#include "stdafx.h"
#include "RandomMgr.h"


mt19937 CRandomMgr::engine((unsigned int)time(NULL));
uniform_int_distribution<int> CRandomMgr::distribution;
_Binder<_Unforced, uniform_int_distribution<int>&, mt19937&> CRandomMgr::generator(bind(distribution, engine));

CRandomMgr::CRandomMgr()
{
}


CRandomMgr::~CRandomMgr()
{
}
