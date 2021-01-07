#pragma once
#ifndef __FINAL_NORMAL_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CFinalNormal final : public CGameObject
{
private:
	CMeshRenderer*		m_pMeshRenderer;

	CTexturePool*		m_pTexturePool;
	TSTRING				m_sTextureKey;

	UINT				m_nIndex;
	UINT				m_nMaxFrame;

	float				m_fTime;
	float				m_fAnimationSpeed;

	float				m_fMoveSpeed;

	bool				m_bRemove;

	D3DXVECTOR3			m_vMoveDirection;

	CCollider*			m_pCollider;
private:
	explicit CFinalNormal();
	explicit CFinalNormal(const CFinalNormal& _rOther);
	virtual ~CFinalNormal() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CFinalNormal* Create();
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
	void	SetBullet(D3DXVECTOR3 _vStart);
private:
	void	Animate(const float _fDeltaTime);
	void	Move(const float _fDeltaTime);
};
#define __FINAL_NORMAL_H__
#endif // !__FINAL_NORMAL_H__
