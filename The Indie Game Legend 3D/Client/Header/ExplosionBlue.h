#pragma once
#ifndef __EXPLOSIONBLUE_H__

#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CExplosionBlue final : public CGameObject
{
private:
	explicit CExplosionBlue();
	explicit CExplosionBlue(const CExplosionBlue& other);
	virtual ~CExplosionBlue() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

private:
	void IsBillBording();
	
public:
	void SetPos(const _vector _EggPos);

public:
	virtual CGameObject * Clone() override;
	static CExplosionBlue* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	CCollider*	  m_pCollider;



	float	m_fTextureIndex = 0.f;
	float	m_fAnimateSpeed = 0.f;
	float	m_fAnimateOneCycleTime = 0.f; 
	_int	m_nMaxFrame = 0;


	float				m_fMoveSpeed;

	float				m_fWalkSpeed;
	float				m_fWalkDeltaTime;

	float				m_fJumpPower;
	float				m_fJumpTime;
	float				m_fYTest;
	bool				m_bJump;
	bool				m_bDead;

	UINT nIndex;

	bool m_bIsTPS;
};

#define __EXPLOSIONBLUE_H__
#endif // !__EXPLOSIONBLUE_H__
