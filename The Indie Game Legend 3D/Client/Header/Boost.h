#pragma once
#ifndef __BOOST_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CBoost : public CGameObject
{
private:
	CMeshRenderer*		m_pMeshRenderer;

	CTexturePool*		m_pTexturePool;
	TSTRING				m_sTextureKey;

	UINT				m_nIndex;
	UINT				m_nMaxFrame;

	float				m_fTime;
	float				m_fAnimationSpeed;

	CTransform*			m_pBattleShipTransform;

	D3DXVECTOR3			m_vOffset;
private:
	explicit CBoost();
	explicit CBoost(const CBoost& _rOther);
	virtual ~CBoost() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CBoost* Create();
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
private:
	void	Animate(const float _fDeltaTime);
	void	SetPosition();
};
#define __BOOST_H__
#endif // !__BOOST_H__
