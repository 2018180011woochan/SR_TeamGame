#include "..\Header\Font.h"
#include "Texture.h"
#include "GraphicDevice.h"
#include "Character.h"
USING(Engine)

CFont::CFont()
	: m_pTexture(nullptr)
	, m_nASCIIOffset(32)
{
	m_pDevice = CGraphicDevice::GetInstance()->GetDevice();
	SafeAddRef(m_pDevice);
}

CFont::CFont(const CFont & _rOther)
	: m_nASCIIOffset(32)
	, m_pTexture(_rOther.m_pTexture)
	, m_pDevice(_rOther.m_pDevice)
{
	UINT nSize = _rOther.m_Characters.size();

	m_Characters.reserve(nSize);
	for (UINT i = 0; i < nSize; ++i)
		m_Characters.push_back(nullptr);

	for (UINT i = 0; i < nSize; ++i)
	{
		m_Characters[i] = _rOther.m_Characters[i]->Clone();
	}

	SafeAddRef(m_pDevice);
	SafeAddRef(m_pTexture);
}

void CFont::Free()
{
	for (auto& pCharacter : m_Characters)
	{
		SafeRelease(pCharacter);
	}
	m_Characters.clear();
	m_Characters.shrink_to_fit();
	SafeRelease(m_pTexture);
	SafeRelease(m_pDevice);
}

CFont * CFont::Create(const TSTRING & _sFont)
{
	CFont* pInstance = new CFont;
	pInstance->LoadFont(_sFont);
	return pInstance;
}

CFont * CFont::Clone()
{
	CFont* pClone = new CFont(*this);
	return pClone;
}

CCharacter * CFont::GetCharacter(UINT _nChar)
{
	UINT nIndex = _nChar - m_nASCIIOffset;

	if (m_Characters.size() <= nIndex || 0 > nIndex)
	{
		PrintLog(TEXT("Warning"), TEXT("_nChar is out of range"));
		return nullptr;
	}

	return m_Characters[nIndex];
}

CTexture * CFont::GetTexture()
{
	return m_pTexture;
}

HRESULT CFont::LoadFont(const TSTRING & _sFont)
{
	FILE* pFile = nullptr;

	TSTRING sPath = TEXT("../Resources/Font/");
	TSTRING sFormat = TEXT(".fnt");
	TSTRING sFullPath = sPath + _sFont + sFormat;
	if (0 != _tfopen_s(&pFile, sFullPath.c_str(), TEXT("r")))
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to open %s file"), sFullPath.c_str());
		PrintLog(TEXT("Warning"), szBuf);
		return E_FAIL;
	}

	TCHAR szLine[MAX_PATH + 1] = TEXT("");
	TSTRING sConversionFormat;
	//.fnt
	//ù ��° �� : ��ϵ��� ���� ���ڵ鿡 ���� �⺻ ��Ʈ ����?
	_fgetts(szLine, MAX_PATH, pFile);
	//�� ��° �� : ��Ʈ�� ��Ʈ �̹��� ũ�� ����?
	_fgetts(szLine, MAX_PATH, pFile);
	sConversionFormat = szLine;
	
	UINT nTextureWidth = 0;
	UINT nTextureHeight = 0;

	size_t nReplaceStart = 0;
	size_t nReplaceEnd = 0;

	nReplaceStart = sConversionFormat.find(TEXT("scaleW="), 7);
	//nReplaceStart ��ġ ���� ' '�� ��ġ�� ã��.
	nReplaceEnd = sConversionFormat.find(TEXT(' '), nReplaceStart);
	//nReplaceStart + "scaleW="�� ���ڿ� ���� ��ġ���� nReplaceEnd - nReplaceStart ��ŭ�� ������ ���ڸ� ġȯ.
	sConversionFormat.replace(nReplaceStart + 7, nReplaceEnd - (nReplaceStart + 7), TEXT("%d"));
	
	nReplaceStart = sConversionFormat.find(TEXT("scaleH="), 7);
	//nReplaceStart ��ġ ���� ' '�� ��ġ�� ã��.
	nReplaceEnd = sConversionFormat.find(TEXT(' '), nReplaceStart);
	//nReplaceStart + "scaleH="�� ���ڿ� ���� ��ġ���� nReplaceEnd - nReplaceStart ��ŭ�� ������ ���ڸ� ġȯ.
	sConversionFormat.replace(nReplaceStart + 7, nReplaceEnd - (nReplaceStart + 7), TEXT("%d"));

	//BitMap Font�� ���Ǵ� �ؽ�ó�� ����, ���α��� �о��
	_stscanf_s(szLine, sConversionFormat.c_str(), &nTextureWidth, &nTextureHeight);

	//�� ��° �� : ��Ʈ�� ��Ʈ�� ���Ǵ� �̹��� ���� �̸�.
	TCHAR szTextureFile[MAX_PATH] = TEXT("");
	
	_fgetts(szLine, MAX_PATH, pFile);

	sConversionFormat = szLine;

	nReplaceStart = sConversionFormat.find(TEXT("file=\""), 6);
	nReplaceEnd = sConversionFormat.find(TEXT('\"'), nReplaceStart + 6);

	sConversionFormat.replace(nReplaceStart + 6, nReplaceEnd - (nReplaceStart + 6), TEXT("%[^\"]s"));
	_stscanf_s(szLine, sConversionFormat.c_str(), szTextureFile, MAX_PATH);

	TSTRING sTexturePath = TEXT("");
	sTexturePath = sPath;
	sTexturePath += szTextureFile;
	
	m_pTexture = CTexture::Create();
	m_pTexture->Load(sTexturePath);

	//�� ��° �� : Character �� ����.
	_fgetts(szLine, MAX_PATH, pFile);
	UINT nCount = 0;
	_stscanf_s(szLine, TEXT("chars count=%d"), &nCount);
	m_Characters.reserve(size_t(nCount));

	for (UINT i = 0; i < nCount; ++i)
	{
		m_Characters.push_back(nullptr);
	}

	//
	CCharacter* pCharacter = nullptr;
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer = nullptr;
	LPDIRECT3DINDEXBUFFER9  pIndexBuffer = nullptr;
	LPVERTEXRECT			pVertices = nullptr;
	LPINDEX16				pIndices = nullptr;
	UINT nID = 0;
	UINT nStartX = 0;
	UINT nStartY = 0;
	UINT nWidth = 0;
	UINT nHeight = 0;
	UINT nNotUse[5] = { 0,0,0,0,0 };


	while (false == feof(pFile))
	{
		_fgetts(szLine, MAX_PATH, pFile);
		_stscanf_s(szLine,
			TEXT("char id=%d   x=%d     y=%d     width=%d     height=%d    xoffset=%d     yoffset=%d     xadvance=%d     page=%d  chnl=%d")
			, &nID, &nStartX, &nStartY, &nWidth, &nHeight, &nNotUse[0], &nNotUse[1], &nNotUse[2], &nNotUse[3], &nNotUse[4]);
		
		/*
		1--------2
		|		 |
		|		 |
		|		 |
		0--------3
		*/

		m_pDevice->CreateVertexBuffer(
			sizeof(VERTEXRECT) * 4,
			D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
			VERTEXRECT::FVF,
			D3DPOOL_DEFAULT,
			&pVertexBuffer,
			0);

		pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
		pVertices[0].Position = D3DXVECTOR3(-float(nWidth * 0.5f), -float(nHeight*0.5f), 0.f);
		pVertices[1].Position = D3DXVECTOR3(-float(nWidth * 0.5f),  float(nHeight*0.5f), 0.f);
		pVertices[2].Position = D3DXVECTOR3( float(nWidth * 0.5f),  float(nHeight*0.5f), 0.f);
		pVertices[3].Position = D3DXVECTOR3( float(nWidth * 0.5f), -float(nHeight*0.5f), 0.f);

		//pVertices[0].Position = D3DXVECTOR3(-float(nWidth * 0.5f) - 0.5f, -float(nHeight*0.5f) - 0.5f, 0.f);
		//pVertices[1].Position = D3DXVECTOR3(-float(nWidth * 0.5f) - 0.5f, float(nHeight*0.5f) - 0.5f, 0.f);
		//pVertices[2].Position = D3DXVECTOR3(float(nWidth * 0.5f) - 0.5f, float(nHeight*0.5f) - 0.5f, 0.f);
		//pVertices[3].Position = D3DXVECTOR3(float(nWidth * 0.5f) - 0.5f, -float(nHeight*0.5f) - 0.5f, 0.f);

		pVertices[0].UV = D3DXVECTOR2(float(nStartX) / float(nTextureWidth), float(nStartY + nHeight) / float(nTextureHeight));
		pVertices[1].UV = D3DXVECTOR2(float(nStartX) / float(nTextureWidth), float(nStartY) / float(nTextureHeight));
		pVertices[2].UV = D3DXVECTOR2(float(nStartX + nWidth) / float(nTextureWidth), float(nStartY) / float(nTextureHeight));
		pVertices[3].UV = D3DXVECTOR2(float(nStartX + nWidth) / float(nTextureWidth), float(nStartY + nHeight) / float(nTextureHeight));


		//pVertices[0].UV = D3DXVECTOR2(float(nStartX + 0.5f) / float(nTextureWidth), float(nStartY + nHeight + 0.5f) / float(nTextureHeight));
		//pVertices[1].UV = D3DXVECTOR2(float(nStartX + 0.5f) / float(nTextureWidth), float(nStartY + 0.5f) / float(nTextureHeight));
		//pVertices[2].UV = D3DXVECTOR2(float(nStartX + nWidth + 0.5f) / float(nTextureWidth), float(nStartY + 0.5f) / float(nTextureHeight));
		//pVertices[3].UV = D3DXVECTOR2(float(nStartX + nWidth + 0.5f) / float(nTextureWidth), float(nStartY + nHeight + 0.5f) / float(nTextureHeight));

		//pVertices[0].UV = D3DXVECTOR2(float(nStartX) / float(nTextureWidth) + 0.5f/ nTextureWidth, float(nStartY + nHeight) / float(nTextureHeight) + 0.5f/ nTextureHeight);
		//pVertices[1].UV = D3DXVECTOR2(float(nStartX) / float(nTextureWidth) + 0.5f / nTextureWidth, float(nStartY) / float(nTextureHeight) + 0.5f / nTextureHeight);
		//pVertices[2].UV = D3DXVECTOR2(float(nStartX + nWidth) / float(nTextureWidth) + 0.5f / nTextureWidth, float(nStartY) / float(nTextureHeight) + 0.5f / nTextureHeight);
		//pVertices[3].UV = D3DXVECTOR2(float(nStartX + nWidth) / float(nTextureWidth) + 0.5f / nTextureWidth, float(nStartY + nHeight) / float(nTextureHeight) + 0.5f / nTextureHeight);

		pVertexBuffer->Unlock();


		m_pDevice->CreateIndexBuffer(
			sizeof(INDEX16) * 6,
			0,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&pIndexBuffer,
			0
		);

		pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);
		pIndices[0] = { 0,1,2 };
		pIndices[1] = { 0,2,3 };
		pIndexBuffer->Unlock();
		
		pCharacter = CCharacter::Create(m_pDevice, pVertexBuffer, pIndexBuffer, nWidth, nHeight);

		UINT nIndex = nID - m_nASCIIOffset;

		if (nullptr != m_Characters[nIndex])
		{
			SafeRelease(pCharacter);
			continue;
		}

		m_Characters[nIndex] = pCharacter;
	}


	return S_OK;
}
