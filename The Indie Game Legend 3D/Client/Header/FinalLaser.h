#pragma once
#ifndef __FINAL_LASER_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CFinalExplosion;
class CFinalLaser final : public CGameObject
{
public:
	enum DIRECTION { LEFT, RIGHT, TOP, BOTTOM };
private:
	CMeshRenderer*		m_pMeshRenderer;

	CTexturePool*		m_pTexturePool;
	TSTRING				m_sTextureKey;

	UINT				m_nIndex;
	UINT				m_nMaxFrame;

	float				m_fTime;
	float				m_fAnimationSpeed;

	bool				m_bRender;

	D3DXVECTOR3			m_vDirection;

	bool				m_bRemove;

	vector<CFinalExplosion*>	m_vecFinalExplosion;
private:
	explicit CFinalLaser();
	explicit CFinalLaser(const CFinalLaser& _rOther);
	virtual ~CFinalLaser() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CFinalLaser* Create();
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
	void	SetLaser(DIRECTION _eDirection);
private:
	void	Animate(const float _fDeltaTime);
	void	Explosion(const float _fDeltaTime);
	void	SetTextureKey(const TSTRING _sTextureKey);
};
#define __FINAL_LASER_H__
#endif // !__FINAL_LASER_H__
