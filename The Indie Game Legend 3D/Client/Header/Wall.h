#pragma once
#ifndef __WALL_H__
#include "GameObject.h"
USING(Engine)
class CWall : public CGameObject
{
public:
	enum WallType { Wall, Door };

private:
	explicit CWall();
	explicit CWall(const CWall& _rOther);
	virtual ~CWall() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CWall* Create();
	// CGameObject을(를) 통해 상속됨
	virtual CGameObject * Clone() override;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
};
#define __WALL_H__
#endif // !__WALL_H__
