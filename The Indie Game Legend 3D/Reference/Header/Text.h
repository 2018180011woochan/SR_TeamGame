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

	//���ڿ� ����
	UINT			m_nLength;
	//���ڿ� ����.
	UINT			m_nStringWidth;
private:
	explicit CText(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	virtual ~CText() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static CText* Create(CGameObject* const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice);
	// CComponent��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;

	HRESULT Render();
public:
	HRESULT SetFont(const TSTRING& _sFont);
	HRESULT SetText(const TSTRING& _sText);
};
END
#define __TEXT_H__
#endif // !__TEXT_H__
