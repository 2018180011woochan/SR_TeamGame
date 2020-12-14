#pragma once
#ifndef __TEXTURE_POOL_H__
#include "Base.h"
#include "Texture.h"
USING(Engine)
class CTexturePool final : public CBase
{
private:
	typedef unordered_map<TSTRING, vector<CTexture*>> TEXTURE;
	TEXTURE::iterator	m_Iter;
	TEXTURE				m_Texture;
private:
	explicit CTexturePool();
	virtual ~CTexturePool() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

private:
	HRESULT Find(const TSTRING& _sStateKey);
public:
	HRESULT AddTexture(const TSTRING& _sFilePath, const TSTRING _sStateKey, const UINT _nFileCount);
	static CTexturePool* Create();

	const vector<CTexture*> GetTexture(const TSTRING& _sStateKey);
};
#define __TEXTURE_POOL_H__
#endif // !__TEXTURE_POOL_H__
