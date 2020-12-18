#pragma once
#ifndef __SLIDER_H__
#include "GameObject.h"
USING(Engine)
class CSlider final : public CGameObject
{
private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	class CTexturePool*  m_pTexturePool = nullptr;
	bool				m_bReturn = false;
	float			m_fMoveSpeed = 0.f;

	//Test
	_vector			m_vSavePos;

private:
	void Move(const float _fDeltaTime);
	void IsBillboarding();
private:
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
public:
	static CSlider* Create();
private:
	virtual void Free() override;
};
#define __SLIDER_H__
#endif
