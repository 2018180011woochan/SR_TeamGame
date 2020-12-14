#include "..\Header\Texture.h"
#include "GraphicDevice.h"

USING(Engine)

CTexture::CTexture()
	: m_pTexture(nullptr)
{
	m_pDevice = CGraphicDevice::GetInstance()->GetDevice();
	SafeAddRef(m_pDevice);
	ZeroMemory(&m_tImageInfo, sizeof(D3DXIMAGE_INFO));
}

void CTexture::Free()
{
	SafeRelease(m_pTexture);
	SafeRelease(m_pDevice);
}

CTexture * CTexture::Create()
{
	CTexture* pInstance = new CTexture;

	if (nullptr == pInstance)
	{
		PrintLog(TEXT("Error"), TEXT("Failed to create CTexture."));
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

HRESULT CTexture::Load(const TSTRING & _sFilePath)
{
	if (FAILED(D3DXGetImageInfoFromFile(_sFilePath.c_str(), &m_tImageInfo)))
		return E_FAIL;
	if (FAILED(D3DXCreateTextureFromFileEx(
		m_pDevice,
		_sFilePath.c_str(),
		m_tImageInfo.Width,
		m_tImageInfo.Height,
		m_tImageInfo.MipLevels,
		0,
		m_tImageInfo.Format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		nullptr,
		nullptr,
		&m_pTexture
	)))
		return E_FAIL;

	return S_OK;
}

LPDIRECT3DTEXTURE9 CTexture::GetTexture()
{
	return m_pTexture;
}

