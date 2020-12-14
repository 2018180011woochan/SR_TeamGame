#include "stdafx.h"
#include "..\Header\TexturePoolManager.h"

IMPLEMENT_SINGLETON(CTexturePoolManager)

CTexturePoolManager::CTexturePoolManager()
{
	Load(TEXT("../Resources/Texture"));
}

void CTexturePoolManager::Free()
{
	for (auto& rPair : m_TexturePool)
	{
		SafeRelease(rPair.second);
	}
	m_TexturePool.clear();
}

HRESULT CTexturePoolManager::AddTexturePool(const TSTRING & _sFilePath, const TSTRING & _sObjectKey, const TSTRING & _sStateKey, const UINT _nFileCount)
{
	if (SUCCEEDED(Find(_sObjectKey)))
	{
		if (FAILED(m_Iter->second->AddTexture(_sFilePath, _sStateKey, _nFileCount)))
			return E_FAIL;
	}
	else
	{
		CTexturePool* pTexturePool = CTexturePool::Create();

		if (nullptr == pTexturePool)
			return E_FAIL;
		if (FAILED(pTexturePool->AddTexture(_sFilePath, _sStateKey, _nFileCount)))
		{
			SafeRelease(pTexturePool);
			return E_FAIL;
		}

		m_TexturePool.emplace(_sObjectKey, pTexturePool);
	}
	return S_OK;
}

HRESULT CTexturePoolManager::Find(const TSTRING & _sObjectKey)
{
	m_Iter = m_TexturePool.find(_sObjectKey);

	if (m_TexturePool.end() == m_Iter)
		return E_FAIL;

	return S_OK;
}

HRESULT CTexturePoolManager::Load(const TSTRING & _sPath)
{
	WIN32_FIND_DATA tFindData;

	TSTRING sPath = _sPath + TEXT("/*.*");

	HANDLE hFind = FindFirstFile(sPath.c_str(), &tFindData);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		FindClose(hFind);
		return E_FAIL;
	}

	do
	{
		if (TEXT('.') == tFindData.cFileName[0])
			continue;
		else if (FILE_ATTRIBUTE_DIRECTORY == tFindData.dwFileAttributes)
		{
			TSTRING sNextPath = _sPath + TEXT('/') + tFindData.cFileName;
			Load(sNextPath);
		}
		else
		{
			TSTRING sFullPath = TEXT("");
			TSTRING sStateKey = TEXT("");
			TSTRING sObjectKey = TEXT("");
			TSTRING sFileName = tFindData.cFileName;
			TSTRING sFormat = TEXT("");
			TSTRING sTemp = _sPath;
			int nFind = sFileName.rfind(TEXT('.'));

			//포맷
			sFormat = sFileName.substr(nFind);
			//파일 이름 재설정 
			sFileName = sFileName.substr(0, nFind - 1) + TEXT("%d") + sFormat;
			//재설정 된 파일 이름으로 경로 설정.
			sFullPath = _sPath + TEXT('/') + sFileName;
			//StateKey 추출.
			nFind = sTemp.rfind(TEXT('/'));
			sStateKey = sTemp.substr(nFind + 1);
			sTemp = sTemp.substr(0, nFind);
			//ObjectKey 추출.
			nFind = sTemp.rfind(TEXT('/'));
			sObjectKey = sTemp.substr(nFind + 1);
			//디렉토리 안 파일 개수 계산.
			UINT nFileCount = 0;
			do
			{
				if (FILE_ATTRIBUTE_SYSTEM == tFindData.dwFileAttributes || TEXT('.') == tFindData.cFileName[0])
					continue;
				++nFileCount;
			} while (TRUE == FindNextFile(hFind, &tFindData));

			if (FAILED(AddTexturePool(sFullPath, sObjectKey, sStateKey, nFileCount)))
			{
				PrintLog(TEXT("Warning"), TEXT("Failed to create CTexturePool"));
				return E_FAIL;
			}
			break;
		}
	} while (TRUE == FindNextFile(hFind, &tFindData));

	FindClose(hFind);
	return S_OK;
}

CTexturePool * CTexturePoolManager::GetTexturePool(const TSTRING & _sObjectKey)
{
	if (FAILED(Find(_sObjectKey)))
		return nullptr;

	return m_Iter->second;
}
