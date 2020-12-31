#pragma once
#ifndef __MONSTER_H__
#include "GameObject.h"
USING(Engine)
class CMonster : public CGameObject
{
public:
	explicit CMonster();
	explicit CMonster(const CMonster& other);
	virtual ~CMonster() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

public:
	HRESULT IsBillboarding();

public:
	//Getter
	bool	GetDead() { return m_bDead; };
	virtual CGameObject * Clone() = 0;
	virtual void OnCollision(CGameObject* _pGameObject) override;
protected:
	const CTransform*	m_pPlayerTransform;
	class CVIBuffer*	m_pVIBufferCom = nullptr;
	class CTexture*		m_pTextureCom = nullptr;
	CCollider*			m_pCollider;
	_vector				m_vecInitPos;
	//wjm test
	bool				m_bDead = false;
	int					m_iHP;
};

#define __MONSTER_H__
#endif // !__MONSTER_H__

