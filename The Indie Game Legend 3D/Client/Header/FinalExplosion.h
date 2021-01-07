#pragma once
#ifndef __FINAL_EXPLOSION_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CFinalExplosion final : public CGameObject
{
private:
	CMeshRenderer*		m_pMeshRenderer;
	CCollider*			m_pCollider;


	CTexturePool*		m_pTexturePool;
	TSTRING				m_sTextureKey;

	UINT				m_nIndex;
	UINT				m_nMaxFrame;

	float				m_fTime;
	float				m_fAnimationSpeed;

	bool				m_bRemove;

private:
	explicit CFinalExplosion();
	explicit CFinalExplosion(const CFinalExplosion& _rOther);
	virtual ~CFinalExplosion() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CFinalExplosion* Create();
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
	void	SetPosition(D3DXVECTOR3 _vPosition);
	void	Remove();
private:

};
#define __FINAL_EXPLOSION_H__
#endif // !__FINAL_EXPLOSION_H__
