#pragma once
#ifndef __LOADING_TEXT_H__
#include "GameObject.h"
USING(Engine)
class CLoadingText final : public CGameObject
{
private:
	CText* m_pText;
private:
	explicit CLoadingText();
	explicit CLoadingText(const CLoadingText& _rOther);
	virtual ~CLoadingText() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CLoadingText* Create();
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

	void SetText(const TSTRING& _sText);
};
#define __LOADING_TEXT_H__
#endif // !__LOADING_TEXT_H__

