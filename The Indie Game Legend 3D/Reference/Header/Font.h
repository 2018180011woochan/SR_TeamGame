#pragma once
#ifndef __FONT_H__
#include "Base.h"
BEGIN(Engine)
class CTexture;
class Font final : public CBase
{
private:
	CTexture*	m_pTexture;
private:
	explicit Font();
	explicit Font(const Font& _rOther);
	virtual ~Font() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
};
END
#define __FONT_H__
#endif // !__FONT_H__
