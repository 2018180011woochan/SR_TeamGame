#pragma once
#ifndef __BULLET_H__
#include "GameObject.h"
#include "TexturePoolManager.h"
#include "MsgManager.h"

USING(Engine)
class CBullet : public CGameObject
{
protected:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool = nullptr;
	CCollider*		m_pCollider = nullptr;
	_vector			m_vDiraction;

	float			m_fLive;
	float			m_fLivetime;
	float			m_fMoveSpeed;

	bool			m_bDead ;
	//
	float	m_fTextureIndex = 0.f;
	float	m_fAnimateSpeed = 0.f;
	float	m_fAnimateOneCycleTime = 0.f; 
	_uint   m_nMaxTexture;

public:
	explicit CBullet();
	explicit CBullet(const CBullet& _rOther);
	virtual ~CBullet() = default;

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() PURE;
	virtual HRESULT Awake() PURE;
	virtual HRESULT Start() PURE;
	virtual UINT Update(const float _fDeltaTime) PURE;
	virtual UINT LateUpdate(const float _fDeltaTime) PURE;
	virtual HRESULT Render() PURE;
	virtual CGameObject * Clone() PURE;

	virtual HRESULT Fire() PURE;

	HRESULT Animate(const float _fDeltaTime);

protected:
	virtual HRESULT IsBillboarding();
	virtual void Free() override;
	virtual void OnCollision(CGameObject * _pGameObject) PURE;
};
#define __BULLET_H__
#endif
