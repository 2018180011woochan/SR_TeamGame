#pragma once
#ifndef __DISC_TEXT_H__
#include "GameObject.h"
USING(Engine)
class CDiscText final : public CGameObject
{
private:
	CText*	m_pText;
	UINT	m_nMaxCount;
	UINT	m_nCount;
private:
	explicit CDiscText();
	explicit CDiscText(const CDiscText& _rOther);
	virtual ~CDiscText() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CDiscText* Create();
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
	void SetMaxCount(const UINT _nMaxCount);
	void SetCount(const UINT _nCount);
};
#define __DISC_TEXT_H__
#endif // !__DISC_TEXT_H__
