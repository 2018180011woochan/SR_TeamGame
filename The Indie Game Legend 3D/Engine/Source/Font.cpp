#include "..\Header\Font.h"
#include "Texture.h"
USING(Engine)

Font::Font()
	:m_pTexture(nullptr)
{
}

Font::Font(const Font & _rOther)
{
}

void Font::Free()
{
	SafeRelease(m_pTexture);
}
