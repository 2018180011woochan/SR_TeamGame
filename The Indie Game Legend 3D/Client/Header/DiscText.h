#pragma once
#include "GameObject.h"
USING(Engine)
class CDiscText final : public CGameObject
{
private:
	CText*	m_pText;
public:
	explicit CDiscText();
	explicit CDiscText(const CDiscText& _rOther);
	virtual ~CDiscText() = default;

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;

private:
	virtual void Free() override;
public:
	static CDiscText* Create();
};

