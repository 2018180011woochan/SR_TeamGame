#pragma once
#ifndef __GEM_TEXT_H__
#include "GameObject.h"
USING(Engine)
class CGemText final : public CGameObject
{
private:
	CText*	m_pText;
private:
	explicit CGemText();
	explicit CGemText(const CGemText& _rOther);
	virtual ~CGemText() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CGemText* Create();
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
};
#define __GEM_TEXT_H__
#endif // !__GEM_TEXT_H__
