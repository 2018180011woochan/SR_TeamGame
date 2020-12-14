#pragma once
#ifndef __TEXTURE_POOL_MANAGER_H__
#include "Base.h"
#include "TexturePool.h"
USING(Engine)
class CTexturePoolManager final : public CBase
{
	DECLARE_SINGLETON(CTexturePoolManager)
private:
	typedef unordered_map<TSTRING, CTexturePool*> TEXTUREPOOL;
	TEXTUREPOOL::iterator	m_Iter;
	TEXTUREPOOL				m_TexturePool;

private:
	explicit CTexturePoolManager();
	virtual ~CTexturePoolManager() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

private:
	HRESULT AddTexturePool(const TSTRING& _sFilePath, const TSTRING& _sObjectKey, const TSTRING& _sStateKey, const UINT _nFileCount);
	HRESULT Find(const TSTRING& _sObjectKey);
	HRESULT Load(const TSTRING& _sPath);

public:
	CTexturePool* GetTexturePool(const TSTRING& _sObjectKey);
};
#define __TEXTURE_POOL_MANAGER_H__
#endif // !__TEXTURE_POOL_MANAGER_H__
