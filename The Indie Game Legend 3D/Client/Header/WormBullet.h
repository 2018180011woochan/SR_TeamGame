#pragma once
#ifndef __WORM_BULLET_H__
#include "Monster.h"
USING(Engine)
class CTexturePool;
class CWormBullet : public CMonster
{
private:
	CMeshRenderer*	m_pMeshRenderer;
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;

	UINT			m_nMaxFrame;
	UINT			m_nIndex;

	float			m_fTime;
	float			m_fAnimateSpeed;

	D3DXVECTOR3		m_vDir;

	float			m_fMoveSpeed;
private:
	explicit CWormBullet();
	explicit CWormBullet(const CWormBullet& _rOther);
	virtual ~CWormBullet() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CWormBullet* Create();
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

public:
	void SetBullet(D3DXVECTOR3 _vStart, D3DXVECTOR3 _vTarget);
	void Move(const float _fDeltaTime);
	void Animate(const float _fDeltaTime);
};
#define __WORM_BULLET_H__
#endif // !__WORM_BULLET_H__
