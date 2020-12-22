#pragma once
#ifndef __BULLETSPARTK_H_
#include "GameObject.h"
USING(Engine)
class CBulletSpark final : public CGameObject
{

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	class CTexturePool*  m_pTexturePool = nullptr;

	float	m_fTextureIndex = 0.f;
	float	m_fAnimateSpeed = 0.f;
	float	m_fAnimateOneCycleTime = 0.f; // 한번 순회하는데 몇초로 설정할거냐
	bool	m_bDead = false;
	_int	m_nMaxFrame = 0;
private:
	void IsBillBording();
	HRESULT Animate(const float _fDeltaTime);
public:
	void SetPosition(const _vector& _vPos);
public:
	explicit CBulletSpark();
	explicit CBulletSpark(const CBulletSpark& _rOther);
	virtual ~CBulletSpark() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;

public:
	static CBulletSpark* Create();
	virtual void Free() override;
};
#define __BULLETSPARTK_H_
#endif
