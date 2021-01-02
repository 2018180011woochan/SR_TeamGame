#pragma once
#ifndef __BLOODHAND_H__
#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CBloodHand : public CGameObject
{
private:
	explicit CBloodHand();
	explicit CBloodHand(const CBloodHand& other);
	virtual ~CBloodHand() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

	virtual void OnCollision(CGameObject * _pGameObject) override;

public:
	virtual CGameObject * Clone() override;
	static CBloodHand* Create();

private:
	HRESULT Movement(float fDeltaTime);

public:
	void SetTurretPos(const _vector _TurretPos, const _vector _PlayerPos);

public:
	HRESULT IsBillboarding();

private:
	virtual void Free() override;

private:
	CMeshRenderer*			m_pMeshRenderer = nullptr;
	CTexturePool*			m_pTexturePool;
	D3DXVECTOR3				m_vecDir;
	CCollider*				m_pCollider;

	bool					m_bDead;
	UINT					nIndex = 0;
	float					m_fBulletSpeed;
};

#define __BLOODHAND_H__
#endif // !__BLOODHAND_H__

