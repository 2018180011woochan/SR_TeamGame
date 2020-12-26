#pragma once
#ifndef __WALL_H__
#include "GameObject.h"
USING(Engine)
class CWall : public CGameObject
{
public:
	enum Direction {Top, Bottom, Left, Right};
private:
	TSTRING		m_sMesh;
	Direction	m_eDirection;
	CCollider*	m_pCollider;

private:
	explicit CWall();
	explicit CWall(const CWall& _rOther);
	virtual ~CWall() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static CWall* Create();
	// CGameObject��(��) ���� ��ӵ�
	virtual CGameObject * Clone() override;
public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

	void SetMesh(const TSTRING& _sMesh);
	void SetDirection(const TSTRING& _sDirection);
};
#define __WALL_H__
#endif // !__WALL_H__
