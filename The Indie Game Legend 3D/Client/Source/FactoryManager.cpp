#include "stdafx.h"
#include "FactoryManager.h"
#include "GameObject.h"
#include "Scene.h"
IMPLEMENT_SINGLETON(CFactoryManager)
HRESULT CFactoryManager::LoadDataFile(const TSTRING & _sFileName)
{
	auto CheckPosition = [](vector<D3DXVECTOR3> _vecList) {
		_vector vPos = vZero;
		if (_vecList.size() == 4)
		{
			
		}
		else
		{
			return _vecList[4];
		}
		PrintLog(L"Error", L"not _veclist count 4 ,5 by CFactoryManager::LoadDataFile");
		return vPos;
	};

	FILE* pFile = nullptr;

	TSTRING sPath = TEXT("../../Data/");
	TSTRING sFormat = TEXT(".obj");
	TSTRING sFullPath = sPath + _sFileName + sFormat;
	if (0 != _tfopen_s(&pFile, sFullPath.c_str(), TEXT("r")))
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to open %s file"), sFullPath.c_str());
		PrintLog(TEXT("Warning"), szBuf);
		return E_FAIL;
	}

	ClearData();

	TCHAR szWord[MAX_PATH + 1] = TEXT("");
	vector<D3DXVECTOR3> vecPosition;
	vecPosition.reserve(5);
	while (EOF != _ftscanf_s(pFile, TEXT("%s"), szWord, MAX_PATH))
	{
		if (0 == _tcscmp(TEXT("v"), szWord))
		{
			D3DXVECTOR3 Position;
			_ftscanf_s(pFile, TEXT("%f %f %f\n"), &Position.x, &Position.y, &Position.z);
			vecPosition.push_back(Position);
		}
		else if (0 == _tcscmp(TEXT("o"), szWord))
		{
			if (vecPosition.size() != 5)
			{
				int a = 0;
			}

			_int nRoomID;
			_ftscanf_s(pFile, TEXT("%d\n"), &nRoomID);
			m_vecSaveDataRoomID.push_back(nRoomID);
			m_vecSaveDataPosition.push_back(vecPosition[4]);
			vecPosition.clear();
		}
		else if (0 == _tcscmp(TEXT("usemtl"), szWord))
		{
			TSTRING Type;
			_ftscanf_s(pFile, TEXT("%s\n"), &szWord,MAX_PATH);
			Type = szWord;
			m_vecSaveDataType.push_back(Type);
		}
	}
	fclose(pFile);
	return S_OK;
}

HRESULT CFactoryManager::LoadScene(CScene* const _pScene)
{
	/*3데이터의 사이즈가 다르면 로드실패*/
	//if (m_vecSaveDataRoomID.size() != m_vecSaveDataPosition.size()  && m_vecSaveDataPosition.size()  != m_vecSaveDataType.size())
	//{
	//	PrintLog(L"Error", L"Failed Load Data");
	//	int a = m_vecSaveDataRoomID.size();
	//	int b = m_vecSaveDataPosition.size();
	//	int c = m_vecSaveDataType.size();
	//	return E_FAIL;
	//}

	CManagement* pManagement = CManagement::GetInstance();
	CGameObject* pGameObject = nullptr;

	_int nSize = m_vecSaveDataRoomID.size();
	for (size_t i = 0; i < m_vecSaveDataRoomID.size(); ++i)
	{
		AddObject(_pScene, i);
	/*	TSTRING TypeID = L"C" + m_vecSaveDataType[i];
		pGameObject = _pScene->AddGameObject(TypeID);

		if (nullptr == pGameObject)
		{
			PrintLog(L"Error", L"failed Add GameObject By factoryManager load");
			return E_FAIL;
		}
		pGameObject->Awake();
		((CTransform*)(pGameObject->GetComponent<CTransform>()))->Set_Position(m_vecSaveDataPosition[i]);
		pGameObject->SetTag(m_vecSaveDataRoomID[i]);*/
	}
	return S_OK;
}

void CFactoryManager::ClearData()
{
	m_vecSaveDataPosition.clear();
	m_vecSaveDataPosition.shrink_to_fit();
	m_vecSaveDataType.clear();
	m_vecSaveDataType.shrink_to_fit();
	m_vecSaveDataRoomID.clear();
	m_vecSaveDataRoomID.shrink_to_fit();
}

HRESULT CFactoryManager::AddObject(CScene* const _pScene , _uint _nIndex)
{
	TSTRING TypeID = L"C" + m_vecSaveDataType[_nIndex];
	CGameObject* pGameObject = nullptr;


	if (L"CRoomTriggerH" == TypeID)
	{
		pGameObject = _pScene->AddGameObject(L"CRoomTrigger");
		if (nullptr == pGameObject)
		{
			PrintLog(L"Error", L"failed Add GameObject By factoryManager load");
			return E_FAIL;
		}
		pGameObject->Awake();
		((CTransform*)(pGameObject->GetComponent<CTransform>()))->Set_Position(m_vecSaveDataPosition[_nIndex]);
		((CTransform*)(pGameObject->GetComponent<CTransform>()))->Set_Rotation(_vector(0, 90, 0));
		pGameObject->SetTag(m_vecSaveDataRoomID[_nIndex]);
	}
	else
	{
		pGameObject = _pScene->AddGameObject(TypeID);

		if (nullptr == pGameObject)
		{
			PrintLog(L"Error", L"failed Add GameObject By factoryManager load");
			return E_FAIL;
		}
		pGameObject->Awake();
		((CTransform*)(pGameObject->GetComponent<CTransform>()))->Set_Position(m_vecSaveDataPosition[_nIndex]);
		pGameObject->SetTag(m_vecSaveDataRoomID[_nIndex]);
	}
	return S_OK;

}

CFactoryManager::CFactoryManager()
{
}

void CFactoryManager::Free()
{
	ClearData();
}
