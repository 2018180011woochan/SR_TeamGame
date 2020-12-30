#pragma once
#ifndef __DIALOG_TEXT_H__
#include "GameObject.h"
USING(Engine)
class CDialogText final : public CGameObject
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
	explicit CDialogText();
	explicit CDialogText(const CDialogText& _rOther);
	virtual ~CDialogText() = default;
	// CBase을(를) 통해 상속됨
	virtual void			Free() override;
public:
	static CDialogText* Create();
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
	void SetText(const TSTRING& _sText, const D3DXVECTOR3 _vPosition);
	bool IsTypingEnd();
private:
	void Typing(float _fDeltaTime);
};
#define __DIALOG_TEXT_H__
#endif // !__DIALOG_TEXT_H__

