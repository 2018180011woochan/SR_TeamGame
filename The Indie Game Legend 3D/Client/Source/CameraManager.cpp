#include "stdafx.h"
#include "CameraManager.h"

IMPLEMENT_SINGLETON(CCameraManager)
HRESULT CCameraManager::SetCurrentMainCamera(const ECameraID & _eCameraID)
{
	//���� �� ȸ��
	if (m_eCurMainCameraID == _eCameraID)
	{
		//PrintLog(L"Warning", L"the same ID");
		return S_OK;
	}

	//��Ͼȵ� ī�޶� ����
	if (nullptr == m_pCameraList[_eCameraID])
	{
		PrintLog(L"Error", L"Element has Nullptr");
		return E_FAIL;
	}

	m_eCurMainCameraID = _eCameraID;
	if (m_ePreMainCameraID != m_eCurMainCameraID)
	{
		//���� ī�޶�� ���� ��� ó���� end���̴� ����ó��
		if(m_ePreMainCameraID != ECameraID::End)
			m_pCameraList[m_ePreMainCameraID]->SetEnable(false);

		m_pCameraList[m_eCurMainCameraID]->SetEnable(true);

		m_ePreMainCameraID = m_eCurMainCameraID;
	}
	return S_OK;
}

HRESULT CCameraManager::RegisteCamera(const ECameraID & _eCameraID, const CCamera * pCamera)
{
	if (nullptr != m_pCameraList[_eCameraID])
	{
		PrintLog(L"Warning", L"Element has an address");
		return E_FAIL;
	}
	if (nullptr == pCamera)
	{
		PrintLog(L"Warning", L"pCamera is Null");
		return E_FAIL;
	}
	m_pCameraList[_eCameraID] = const_cast<CCamera*>(pCamera);
	SafeAddRef(pCamera);
	return S_OK;
}

CCameraManager::CCameraManager()
	:m_eCurMainCameraID(ECameraID::End)
     ,m_ePreMainCameraID(ECameraID::End)
{
	ZeroMemory(m_pCameraList, sizeof(CCamera*) * (_uint)ECameraID::End);
}

void CCameraManager::Free()
{
	for (auto& pCamera : m_pCameraList)
	{
		SafeRelease(pCamera);
	}
}
