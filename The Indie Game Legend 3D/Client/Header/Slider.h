#pragma once
#ifndef __SLIDER_H__
#include "GameObject.h"
USING(Engine)
class CSlider final : public CGameObject
{
private:
	CMeshRenderer*			m_pMeshRenderer = nullptr;
	CCollider*				m_pCollider;
	class CTexturePool*		m_pTexturePool = nullptr;
	bool					m_bReverse = false;
	float					m_fMoveSpeed = 0.f;
	_vector					m_vDir;

private:
	void Move(const float _fDeltaTime);
	void IsBillboarding();
public:
	explicit CSlider();
	explicit CSlider(const CSlider& _rOther);
	virtual ~CSlider() = default;

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;

	virtual void OnCollision(CGameObject* _pGameObject) override;
	void SetDirVertical();
public:
	static CSlider* Create();
private:
	virtual void Free() override;
};
#define __SLIDER_H__
#endif
