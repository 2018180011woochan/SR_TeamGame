#pragma once
#ifndef __NPC_TEXT_H__
#include "GameObject.h"
USING(Engine)
class CNPCText final : public CGameObject
{
private:
	CText* m_pText;
	TSTRING	m_sText;
	TSTRING m_sBuffer;

	UINT	m_nLength;
	UINT	m_nIndex;

	float	m_fTime;
	float	m_fTypingSpeed;

	bool	m_bIsEnd;
private:
	explicit CNPCText();
	explicit CNPCText(const CNPCText& _rOther);
	virtual ~CNPCText() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CNPCText* Create();
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
	void SetText(const TSTRING& _sText);

	bool IsTypingEnd();
private:
	void Typing(float _fDeltaTime);
};
#define __NPC_TEXT_H__
#endif // !__NPC_TEXT_H__
