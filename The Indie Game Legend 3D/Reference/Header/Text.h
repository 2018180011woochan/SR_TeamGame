#pragma once
#ifndef __TEXT_H__
#include "Component.h"
BEGIN(Engine)
class CCharacter;
class CFont;
class CFontManager;
class ENGINE_DLL CText final : public CComponent
{
public:
	enum HORIZON { LEFT, CENTER, RIGHT };
private:
	typedef vector<CCharacter*> CHARACTERS;
	CHARACTERS		m_Characters;

	CFont*			m_pFont;
	CFontManager*	m_pFontManager;
	TSTRING			m_sFont;
	TSTRING			m_sText;
	float			m_fSize;

	HORIZON			m_eHorizon;

	//Text Rect 넓이 높이
	float			m_fWidth;
	float			m_fHeight;

	//문자열 길이
	UINT			m_nLength;


	//라인 길이.
	vector<UINT>	m_vecLineWidth;
	//라인 개수
	UINT			m_nLineCount;
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
	void	SetSize(const float _fSize);
	void	SetHorizon(const HORIZON _eHorizon);
	void	SetWidth(const float _fWidth);
	void	SetHeight(const float _fHeight);
};
END
#define __TEXT_H__
#endif // !__TEXT_H__
