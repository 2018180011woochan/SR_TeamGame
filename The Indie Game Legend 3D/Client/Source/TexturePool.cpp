#include "stdafx.h"
#include "..\Header\TexturePool.h"

CTexturePool::CTexturePool()
{
}

void CTexturePool::Free()
{
	for (auto rPair : m_Texture)
	{
		for (CTexture* pTexture : rPair.second)
		{
			SafeRelease(pTexture);
		}
		rPair.second.clear();
		rPair.second.shrink_to_fit();
	}
	m_Texture.clear();
}

HRESULT CTexturePool::Find(const TSTRING & _sStateKey)
{
	m_Iter = m_Texture.find(_sStateKey);

	if (m_Texture.end() == m_Iter)
		return E_FAIL;

	return S_OK;
}

HRESULT CTexturePool::AddTexture(const TSTRING & _sFilePath, const TSTRING _sStateKey, const UINT _nFileCount)
{
	if (SUCCEEDED(Find(_sStateKey)))
		return E_FAIL;

	TCHAR szFilePath[MAX_PATH] = TEXT("");

	CTexture* pTexture = nullptr;

	m_Iter = (m_Texture.emplace(_sStateKey, vector<CTexture*>())).first;
	m_Iter->second.reserve(_nFileCount);

	for (UINT i = 0; i < _nFileCount; ++i)
	{
		pTexture = CTexture::Create();

		_stprintf_s(szFilePath, MAX_PATH, _sFilePath.c_str(), i);

		if (FAILED(pTexture->Load(szFilePath)))
		{
			SafeRelease(pTexture);
			return E_FAIL;
		}

		m_Iter->second.emplace_back(pTexture);
	}

	return S_OK;
}

CTexturePool * CTexturePool::Create()
{
	CTexturePool* pInstance = new CTexturePool();
	if (nullptr == pInstance)
	{
		PrintLog(TEXT("Error"), TEXT("Failed to create CTexturePool"));
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

const vector<CTexture*> CTexturePool::GetTexture(const TSTRING & _sStateKey)
{
	if (FAILED(Find(_sStateKey)))
		return vector<CTexture*>();

	return m_Iter->second;
}

