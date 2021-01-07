#pragma once
#ifndef __WORM_PART_H__
#include "Monster.h"
USING(Engine)
class CTexturePool;
class CWorm;
class CExplosion;
class CWormPart : public CMonster
{
protected:
	CWormPart*		m_pParent;
	CWormPart*		m_pChild;

	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;
	CMeshRenderer*	m_pMeshRenderer;

	D3DXVECTOR3		m_vPrevPosition;

	float			m_fInterval;

	D3DXVECTOR3		m_vMoveDir;

	CWorm*			m_pWorm;

	float			m_fEffectTime;
	UINT			m_nEffectCount;

	CExplosion*		m_pEffect;

	bool			m_bRemove;
protected:
	explicit CWormPart();
	explicit CWormPart(const CWormPart& _rOther);
	virtual ~CWormPart() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CWormPart* Create();
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
	virtual void OnDisable() override;
	virtual void OnCollision(CGameObject* _pGameObject) override;
public:
	//진행 방향 회전
	void RotationZ(D3DXVECTOR3 _vMoveDir);
	//
	void Move(const float _fDistance);

	void Dead(const float _fDeltaTime);
public:
	void SetParent(CWormPart* _pParent);
	void SetChild(CWormPart* _pChild);
	void SetPrevPosition(D3DXVECTOR3 _vPrevPosition);

	D3DXVECTOR3 GetPrevPosition();
};
#define __WORM_PART_H__
#endif // !__WORM_PART_H__
