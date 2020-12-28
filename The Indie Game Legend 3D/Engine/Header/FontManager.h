#pragma once
#ifndef __FONT_MANAGER_H__
#include "Base.h"
#include "Font.h"
BEGIN(Engine)
class CFontManager : public CBase
{
	DECLARE_SINGLETON(CFontManager)
private:
	typedef unordered_map<TSTRING, CFont*> FONT;
	FONT	m_Font;
	
private:
	explicit CFontManager();
	virtual ~CFontManager() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	CFont* Clone(const TSTRING& _sFont);
};
END
#define __FONT_MANAGER_H__
#endif // !__FONT_MANAGER_H__

