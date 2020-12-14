#pragma once
#ifndef __GIZMO_H__
#include "GameObject.h"
BEGIN(Engine)
class CGizmo final : public CGameObject
{
public:
	CGizmo();
	virtual ~CGizmo();
};
END
#define __GIZMO_H__
#endif
