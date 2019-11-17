#include "stdafx.h"
#include "Component.h"
#include "RenderObj.h"
#include "game.h"


Component::Component(RenderObj* pObj)
{
	mObj = pObj;
	mGame = mObj->mGame;
}
