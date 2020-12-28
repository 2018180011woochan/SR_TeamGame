#include "..\Header\Text.h"
#include "FontManager.h"
#include "Character.h"
#include "Texture.h"
#include "GameObject.h"
USING(Engine)

CText::CText(CGameObject * const _pGameObject, LPDIRECT3DDEVICE9 const _pDevice)
	: CComponent(_pGameObject, _pDevice)
	, m_fSize(1.f)
	, m_sText(TEXT(""))
	, m_sFont(TEXT(""))
	, m_pFont(nullptr)
	, m_pFontManager(CFontManager::GetInstance())
	, m_nLength(0)
	, m_fWidth(100.f)
	, m_fHeight(100.f)
	, m_eHorizon(HORIZON::LEFT)
	, m_nLineCount(0)
{
	SafeAddRef(m_pFontManager);
}

void CText::Free()
{
	CComponent::Free();
	SafeRelease(m_pFont);
	SafeRelease(m_pFontManager);

	m_vecLineWidth.clear();
	m_vecLineWidth.shrink_to_fit();
	
	m_Characters.clear();
	m_Characters.shrink_to_fit();
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
	D3DXMatrixScaling(&matScale, m_fSize, m_fSize, 1.f);

	D3DXVECTOR3 vStartPos = D3DXVECTOR3(0.f,0.f,0.f);
	D3DXVECTOR3 vAdded = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vDrawPos = D3DXVECTOR3(0.f, 0.f, 0.f);

	int nLineIndex = -1;
	for (UINT i = 0; i < m_nLength; ++i)
	{
		if (0 == i || nullptr == m_Characters[i])
		{
			++nLineIndex;
			switch (m_eHorizon)
			{
			case Engine::CText::LEFT:
				vStartPos.x = vPosition.x - m_fWidth * 0.5f;
				vStartPos.y = vPosition.y - nLineIndex * m_Characters[0]->GetHeight() * m_fSize;
				break;
			case Engine::CText::CENTER:
				break;
			case Engine::CText::RIGHT:
				vStartPos.x = vPosition.x + m_fWidth * 0.5f - m_vecLineWidth[nLineIndex] * m_fSize;
				vStartPos.y = vPosition.y - nLineIndex * m_Characters[0]->GetHeight() * m_fSize;
				break;
			default:
				break;
			}

			vAdded = D3DXVECTOR3(0.f, 0.f, 0.f);

			if (nullptr == m_Characters[i])
				continue;
		}

		vDrawPos = vStartPos + vAdded;

		D3DXMatrixTranslation(&matTrans, vDrawPos.x, vDrawPos.y, vDrawPos.z);
		matWorld = matScale * matTrans;
		m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_Characters[i]->Render();
		
		if (m_nLength - 1 > i && nullptr != m_Characters[i+1])
			vAdded.x += (m_Characters[i]->GetWidth() + m_Characters[i + 1]->GetWidth()) * 0.5f * m_fSize;
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
	if (_sText[0] == NULL)
		return S_OK;

	m_sText = _sText;

	//기존에 보관 중이던 Character 비움.
	m_Characters.clear();
	m_Characters.shrink_to_fit();

	//새로운 공간 할당.
	m_nLength = m_sText.length();

	m_Characters.reserve(m_nLength);

	m_vecLineWidth.clear();
	m_vecLineWidth.shrink_to_fit();

	m_nLineCount = 1;

	UINT nLineWidth = 0;

	for (UINT i = 0; i < m_nLength; ++i)
	{
		if (TEXT('\n') == m_sText[i] || m_nLength - 1 == i)
		{
			if (TEXT('\n') == m_sText[i])
				m_Characters.push_back(nullptr);
			else
			{
				m_Characters.push_back(m_pFont->GetCharacter(UINT(m_sText[i])));
				nLineWidth += m_Characters[i]->GetWidth();
			}
			m_vecLineWidth.push_back(nLineWidth);
			nLineWidth = 0;
			++m_nLineCount;
		}
		else
		{
			m_Characters.push_back(m_pFont->GetCharacter(UINT(m_sText[i])));
			nLineWidth += m_Characters[i]->GetWidth();
		}
	}

	return S_OK;
}

void CText::SetSize(const float _fSize)
{
	m_fSize = _fSize;
}

void CText::SetHorizon(const HORIZON _eHorizon)
{
	m_eHorizon = _eHorizon;
}

void CText::SetWidth(const float _fWidth)
{
	m_fWidth = _fWidth;
}

void CText::SetHeight(const float _fHeight)
{
	m_fHeight = _fHeight;
}
