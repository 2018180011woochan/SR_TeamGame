#pragma once
#ifndef __CFont_H__
#include "Base.h"
BEGIN(Engine)
//ASCII 전용.....
class CTexture;
class CGraphicDevice;
class CCharacter;
class CFont final : public CBase
{
private:
	typedef vector<CCharacter*> CHARACTERS;
	CHARACTERS			m_Characters;
	LPDIRECT3DDEVICE9	m_pDevice;
	CTexture*			m_pTexture;

	const UINT			m_nASCIIOffset;
private:
	explicit CFont();
	explicit CFont(const CFont& _rOther);
	virtual ~CFont() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CFont* Create(const TSTRING& _sFont);
	CFont* Clone();

	CCharacter* GetCharacter(UINT _nChar);
	CTexture*	GetTexture();
private:
	HRESULT LoadFont(const TSTRING& _sFont);
};
END
#define __CFont_H__
#endif // !__CFont_H__
