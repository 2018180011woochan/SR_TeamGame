#pragma once
#ifndef __ENDING_TEXT_H__
#include "GameObject.h"
USING(Engine)
class CEndingText final : public CGameObject
{
private:
	CText*	m_pText;
	vector<TSTRING> m_sText;
	TSTRING m_sBuffer;

	UINT	m_nLength;
	UINT	m_nIndex;

	UINT	m_nLineIndex;

	float	m_fTime;
	float	m_fTypingSpeed;

	float	m_fDelayTime;
private:
	explicit CEndingText();
	explicit CEndingText(const CEndingText& _rOther);
	virtual ~CEndingText() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CEndingText* Create();
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

private:
	void Typing(const float _fDeltaTime);
};
#define __ENDING_TEXT_H__
#endif // !__ENDING_TEXT_H__
