#include "..\Header\Text.h"
#include "FontManager.h"
#include "Character.h"
#include "Texture.h"
#include "GameObject.h"
USING(Engine)

CText::CText(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
	: CComponent(_pGameObject, _pDevice)
	, m_nSize(1)
	, m_sText(TEXT(""))
	, m_sFont(TEXT(""))
	, m_pFont(nullptr)
	, m_pFontManager(CFontManager::GetInstance())
	, m_nStringWidth(0)
	, m_nLength(0)
{
	SafeAddRef(m_pFontManager);
}

void CText::Free()
{
	CComponent::Free();
	SafeRelease(m_pFont);
	SafeRelease(m_pFontManager);
}

CText * CText::Create(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
{
	CText* pInstance = new CText(_pGameObject, _pDevice);
	if (FAILED(pInstance->Initialize()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

HRESULT CText::Initialize()
{
	return S_OK;
}

HRESULT CText::Render()
{
	if (FAILED(m_pDevice->SetTexture(0, m_pFont->GetTexture()->GetTexture())))
		return E_FAIL;
	if (FAILED(m_pDevice->SetFVF(VERTEXRECT::FVF)))
		return E_FAIL;
	D3DXMATRIX matWorld = ((CTransform*)(m_pGameObject->GetComponent<CTransform>()))->Get_WorldMatrix();

	D3DXVECTOR3 vPosition;
	vPosition.x = matWorld._41;
	vPosition.y = matWorld._42;
	vPosition.z = 0.f;

	D3DXMATRIX matScale,matTrans;
	D3DXMatrixScaling(&matScale, 5.f, 5.f, 0.f);

	for (UINT i = 0; i < m_nLength; ++i)
	{
		D3DXMatrixTranslation(&matTrans, vPosition.x, vPosition.y, vPosition.z);
		matWorld = matScale * matTrans;
		m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_Characters[i]->Render();
		vPosition.x += m_Characters[i]->GetWidth() * 5.f;
	}
	
	return S_OK;
}

HRESULT CText::SetFont(const TSTRING & _sFont)
{
	if (_sFont == m_sFont)
		return S_OK;

	//이전 폰트 해제
	SafeRelease(m_pFont);

	m_sFont = _sFont;

	m_pFont = m_pFontManager->Clone(_sFont);

	if (nullptr == m_pFont)
		return E_FAIL;

	return S_OK;
}

HRESULT CText::SetText(const TSTRING & _sText)
{
	if (_sText == m_sText)
		return S_OK;

	m_sText = _sText;

	//기존에 보관 중이던 Character 비움.
	m_Characters.clear();
	m_Characters.shrink_to_fit();

	//새로운 공간 할당.
	m_nLength = m_sText.length();

	m_Characters.reserve(m_nLength);

	m_nStringWidth = 0;

	for (UINT i = 0; i < m_nLength; ++i)
	{
		m_Characters.push_back(m_pFont->GetCharacter(UINT(m_sText[i])));
		m_nStringWidth += m_Characters[i]->GetWidth();
	}

	return S_OK;
}
