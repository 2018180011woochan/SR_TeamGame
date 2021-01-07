#pragma once
#ifndef __STAR_PARTICLE_H__
#include "GameObject.h"
USING(Engine)
class CStarPool;
class CStarParticle final : public CGameObject
{
private:
	CStarPool*		m_pStarPool;
	CMeshRenderer*	m_pMeshRenderer;
	D3DXVECTOR3		m_vMoveDirection;
	float			m_fMoveSpeed;
private:
	explicit CStarParticle();
	explicit CStarParticle(const CStarParticle& _rOther);
	virtual ~CStarParticle() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CStarParticle* Create();
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

	virtual void OnEnable() override;
private:
	void Move(const float _fDeltaTime);
};
#define __STAR_PARTICLE_H__
#endif // !__STAR_PARTICLE_H__
