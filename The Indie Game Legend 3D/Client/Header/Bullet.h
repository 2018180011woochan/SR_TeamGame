#pragma once
#ifndef __BULLET_H__
#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CBullet : public CGameObject
{
protected:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool = nullptr;
	_vector			m_vDiraction;

	float			m_fLive;
	float			m_fLivetime;
	float			m_fMoveSpeed;


	//test			
	bool			m_bDead ;
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


protected:
	HRESULT IsBillboarding();
	virtual void Free() override;
	virtual void OnCollision(CGameObject * _pGameObject) PURE;
};
#define __BULLET_H__
#endif
