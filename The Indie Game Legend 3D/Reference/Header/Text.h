#pragma once
#ifndef __TEXT_H__
#include "Component.h"
BEGIN(Engine)
class CCharacter;
class CFont;
class CFontManager;
class ENGINE_DLL CText final : public CComponent
{
private:
	typedef vector<CCharacter*> CHARACTERS;
	CHARACTERS		m_Characters;

	CFont*			m_pFont;
	CFontManager*	m_pFontManager;
	TSTRING			m_sFont;
	TSTRING			m_sText;
	UINT			m_nSize;

	//문자열 길이
	UINT			m_nLength;
	//문자열 넓이.
	UINT			m_nStringWidth;
private:
	explicit CText(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	virtual ~CText() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CText* Create(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	// CComponent을(를) 통해 상속됨
	virtual HRESULT Initialize() override;

	HRESULT Render();
public:
	HRESULT SetFont(const TSTRING& _sFont);
	HRESULT SetText(const TSTRING& _sText);
};
END
#define __TEXT_H__
#endif // !__TEXT_H__
