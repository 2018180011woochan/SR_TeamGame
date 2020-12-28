#include "..\Header\FontManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFontManager)

CFontManager::CFontManager()
{
}

void CFontManager::Free()
{
	for (auto rPair : m_Font)
	{
		SafeRelease(rPair.second);
	}
	m_Font.clear();
}

CFont * CFontManager::Clone(const TSTRING & _sFont)
{
	auto iter = m_Font.find(_sFont);

	if (m_Font.end() == iter)
	{
		CFont* pFont = CFont::Create(_sFont);

		if (nullptr == pFont)
		{
			PrintLog(TEXT("Warning"), TEXT("Failed to load Font from file."));
			return nullptr;
		}
		iter = (m_Font.emplace(_sFont, pFont)).first;
	}

	return iter->second->Clone();
}
