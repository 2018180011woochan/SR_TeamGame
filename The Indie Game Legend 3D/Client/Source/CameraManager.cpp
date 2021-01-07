#include "stdafx.h"
#include "CameraManager.h"

IMPLEMENT_SINGLETON(CCameraManager)
HRESULT CCameraManager::SetCurrentMainCamera(const ECameraID& _eCameraID)
{
	//���� �� ȸ��
	if (m_eCurMainCameraID == _eCameraID)
	{
		//PrintLog(L"Warning", L"the same ID");
		return S_OK;
	}

	//��Ͼȵ� ī�޶� ����
	if (nullptr == m_vecCameras[_eCameraID])
	{
		PrintLog(L"Error", L"Element has Nullptr");
		return E_FAIL;
	}

	m_eCurMainCameraID = _eCameraID;
	if (m_ePreMainCameraID != m_eCurMainCameraID)
	{
		//���� ī�޶�� ���� ��� ó���� end���̴� ����ó��
		if(m_ePreMainCameraID != ECameraID::End)
			m_vecCameras[m_ePreMainCameraID]->SetEnable(false);

		m_vecCameras[m_eCurMainCameraID]->SetEnable(true);

		m_ePreMainCameraID = m_eCurMainCameraID;
	}
	return S_OK;
}

HRESULT CCameraManager::RegisteCamera(const ECameraID & _eCameraID, const CCamera * pCamera)
{
	if (nullptr != m_vecCameras[_eCameraID])
	{
		PrintLog(L"Warning", L"Element has an address");
		return E_FAIL;
	}
	if (nullptr == pCamera)
	{
		PrintLog(L"Warning", L"pCamera is Null");
		return E_FAIL;
	}
	m_vecCameras[_eCameraID] =  const_cast<CCamera*>(pCamera);
	m_vecCameras[_eCameraID]->SetEnable(false);
	SafeAddRef(m_vecCameras[_eCameraID]);
	return S_OK;
}

HRESULT CCameraManager::GetCurCameraDesc(CAMERA_DESC*& _rDesc)
{
	if (m_eCurMainCameraID == ECameraID::End || nullptr == m_vecCameras[m_eCurMainCameraID])
	{
		PrintLog(L"Warning", L"Element has an address");
		return E_FAIL;
	}
	_rDesc = const_cast<CAMERA_DESC*>(&m_vecCameras[m_eCurMainCameraID]->Get_Camera());
	return S_OK;
}

CCameraManager::CCameraManager()
	:m_eCurMainCameraID(ECameraID::End)
     ,m_ePreMainCameraID(ECameraID::End)
{
	for (int i = 0; i < ECameraID::End; ++i)
	{
		m_vecCameras.emplace_back(nullptr);
	}
}

void CCameraManager::Free()
{
	for (auto& pCamera : m_vecCameras)
	{
		SafeRelease(pCamera);
	}
	m_vecCameras.clear();
	m_vecCameras.shrink_to_fit();
}
