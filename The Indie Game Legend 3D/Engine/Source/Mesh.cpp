#include "..\Header\Mesh.h"
#include "GraphicDevice.h"
USING(Engine)

CMesh::CMesh()
{
	m_pDevice = CGraphicDevice::GetInstance()->GetDevice();
	SafeAddRef(m_pDevice);
}

CMesh::CMesh(const CMesh & _rOther)
	: m_pDevice(_rOther.m_pDevice)
{
	for (CGroup* pGroup : _rOther.m_Groups)
	{
		m_Groups.emplace_back(pGroup->Clone());
	}
	SafeAddRef(m_pDevice);
}

void CMesh::Free()
{
	for (CGroup* pGroup : m_Groups)
	{
		SafeRelease(pGroup);
	}
	m_Groups.clear();
	m_Groups.shrink_to_fit();
	SafeRelease(m_pDevice);
}

HRESULT CMesh::LoadMesh(const TSTRING & _sFileName)
{
	FILE* pFile = nullptr;

	TSTRING sPath = TEXT("../Resources/Mesh/");
	TSTRING sFormat = TEXT(".obj");
	TSTRING sFullPath = sPath + _sFileName + sFormat;

	if (0 != _tfopen_s(&pFile, sFullPath.c_str(), TEXT("r")))
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to open %s file"), sFullPath.c_str());
		PrintLog(TEXT("Warning"), szBuf);
		return E_FAIL;
	}
	TCHAR szWord[MAX_PATH + 1] = TEXT("");
	vector<D3DXVECTOR3> vecPosition;
	vector<D3DXVECTOR3> vecUV;
	vector<D3DXVECTOR3> vecNormal;
	unordered_map<TSTRING, MATERIAL> materials;

	while (EOF != _ftscanf_s(pFile, TEXT("%s"), szWord, MAX_PATH))
	{
		if (0 == _tcscmp(TEXT("mtllib"), szWord))
		{
			_ftscanf_s(pFile, TEXT("%s"), szWord, MAX_PATH);
			sFullPath = sPath + szWord;
			if (FAILED(LoadMaterial(sFullPath, materials)))
			{
				fclose(pFile);
				return E_FAIL;
			}
		}
		else if (0 == _tcscmp(TEXT("v"), szWord))
		{
			D3DXVECTOR3 Position;
			_ftscanf_s(pFile, TEXT("%f %f %f\n"), &Position.x, &Position.y, &Position.z);
			vecPosition.push_back(Position);
		}
		else if (0 == _tcscmp(TEXT("vt"), szWord))
		{
			D3DXVECTOR3 UV;
			_ftscanf_s(pFile, TEXT("%f %f %f\n"), &UV.x, &UV.y, &UV.z);
			vecUV.push_back(UV);
		}
		else if (0 == _tcscmp(TEXT("vn"), szWord))
		{
			D3DXVECTOR3 Normal;
			_ftscanf_s(pFile, TEXT("%f %f %f\n"), &Normal.x, &Normal.y, &Normal.z);
			vecNormal.push_back(Normal);
		}
	}

	//파일 위치 지정자를 파일 시작 위치로 이동.
	fseek(pFile, 0, SEEK_SET);

	CGroup* pGroup = nullptr;
	MATERIAL tMaterial;
	while (EOF != _ftscanf_s(pFile, TEXT("%s"), szWord, MAX_PATH))
	{
		if (0 == _tcscmp(TEXT("usemtl"), szWord))
		{
			//재질 키 값 읽어옴.
			_ftscanf_s(pFile, TEXT("%s"), szWord, MAX_PATH);
			auto iter = materials.find(szWord);
			if (materials.end() == iter)
			{
				PrintLog(TEXT("Warning"), TEXT("Failed to find material"));
				fclose(pFile);
				return E_FAIL;
			}
			//재질 설정.
			ZeroMemory(&tMaterial, sizeof(MATERIAL));
			tMaterial.pTexture = iter->second.pTexture;
			tMaterial.tMaterial = iter->second.tMaterial;
		}
		else if (0 == _tcscmp(TEXT("f"), szWord))
		{
			fseek(pFile, -1, SEEK_CUR);
			vector<VERTEX> vecVertices;
			VERTEX	tVertices[3];
			UINT	nPositionIndices[3];
			UINT	nUVIndices[3];
			UINT	nNormalIndices[3];
			UINT	nFaceCount = 0;
			while (true)
			{
				if (EOF == _ftscanf_s(
					pFile,
					TEXT("%s %d/%d/%d %d/%d/%d %d/%d/%d\n")
					, szWord, MAX_PATH
					, &nPositionIndices[0], &nUVIndices[0], &nNormalIndices[0]
					, &nPositionIndices[1], &nUVIndices[1], &nNormalIndices[1]
					, &nPositionIndices[2], &nUVIndices[2], &nNormalIndices[2]
				))
					break;

				if (0 != _tcscmp(TEXT("f"), szWord))
				{
					fseek(pFile, -long(_tcslen(szWord) + 1), SEEK_CUR);
					break;
				}

				for (int i = 0; i < 3; ++i)
				{
					tVertices[i].Position = vecPosition[nPositionIndices[i] - 1];
					tVertices[i].Normal = vecNormal[nNormalIndices[i] - 1];
					tVertices[i].UV.x = 1 - vecUV[nUVIndices[i] - 1].x;
					tVertices[i].UV.y = 1 - vecUV[nUVIndices[i] - 1].y;
					vecVertices.push_back(tVertices[i]);
				}

				++nFaceCount;
			}

			LPDIRECT3DVERTEXBUFFER9 pVertexBuffer;
			//버텍스 버퍼 생성.
			m_pDevice->CreateVertexBuffer(
				sizeof(VERTEX) * vecVertices.size(),
				0,
				VERTEX::FVF,
				D3DPOOL_MANAGED,
				&pVertexBuffer,
				0
			);
			//버텍스 버퍼 데이터 설정.
			VERTEX* pVertices = nullptr;
			pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0);
			memcpy(pVertices, &vecVertices[0], sizeof(VERTEX) * vecVertices.size());
			pVertexBuffer->Unlock();

			pGroup = CGroup::Create(m_pDevice, tMaterial, pVertexBuffer, nFaceCount);

			//그룹 저장.
			m_Groups.emplace_back(pGroup);

			vecVertices.clear();
			vecVertices.shrink_to_fit();
		}
	}

	vecPosition.clear();
	vecNormal.clear();
	vecUV.clear();
	vecPosition.shrink_to_fit();
	vecNormal.shrink_to_fit();
	vecUV.shrink_to_fit();
	materials.clear();
	fclose(pFile);

	return S_OK;
}

HRESULT CMesh::LoadMaterial(const TSTRING & _sFullPath, unordered_map<TSTRING, MATERIAL>& _materials)
{
	FILE* pFile = nullptr;

	if (0 != _tfopen_s(&pFile, _sFullPath.c_str(), TEXT("r")))
	{
		TCHAR szBuf[128] = TEXT("");
		_stprintf_s(szBuf, 128, TEXT("Failed to open %s file."), _sFullPath.c_str());
		PrintLog(TEXT("Warning"), szBuf);
		return E_FAIL;
	}

	TCHAR szWord[MAX_PATH + 1] = TEXT("");
	TSTRING sMaterialKey = TEXT("");
	MATERIAL tMaterial;

	while (EOF != _ftscanf_s(pFile, TEXT("%s"), szWord, MAX_PATH))
	{
		if (0 == _tcscmp(TEXT("newmtl"), szWord))
		{
			//재질 구조체 초기화.
			ZeroMemory(&tMaterial, sizeof(MATERIAL));
			//재질 키 값 설정.
			_ftscanf_s(pFile, TEXT("%s"), szWord, MAX_PATH);
			sMaterialKey = szWord;
		}
		else if (0 == _tcscmp(TEXT("Kd"), szWord))
		{
			_ftscanf_s(pFile, TEXT("%f %f %f\n"), &tMaterial.tMaterial.Diffuse.r, &tMaterial.tMaterial.Diffuse.g, &tMaterial.tMaterial.Diffuse.b);
		}
		else if (0 == _tcscmp(TEXT("Ks"), szWord))
		{
			_ftscanf_s(pFile, TEXT("%f %f %f\n"), &tMaterial.tMaterial.Specular.r, &tMaterial.tMaterial.Specular.g, &tMaterial.tMaterial.Specular.b);
		}
		else if (0 == _tcscmp(TEXT("illum"), szWord))
		{
			_ftscanf_s(pFile, TEXT("%f\n"), &tMaterial.tMaterial.Power);
			//재질 추가.
			_materials.emplace(sMaterialKey, tMaterial);
		}
		else if (0 == _tcscmp(TEXT("map_Kd"), szWord))
		{
			//파일 확장자 포함한 이름 읽어옴
			_ftscanf_s(pFile, TEXT("%s"), szWord, MAX_PATH);
			//파일 명으로 경로 찾기. Root 폴더는 Resources.
			TSTRING sFilePath = CFileInfo::FilePath(szWord);
			tMaterial.pTexture = CTexture::Create();
			if (nullptr == tMaterial.pTexture)
			{
				fclose(pFile);
				return E_FAIL;
			}
			if (FAILED(tMaterial.pTexture->Load(sFilePath)))
			{
				TCHAR szBuf[MAX_PATH] = TEXT("");
				_stprintf_s(szBuf, MAX_PATH, TEXT("Failed to Create Texure From File %s"), szWord);
			}
		}
	}
	fclose(pFile);
	return S_OK;
}

HRESULT CMesh::Render()
{
	for (CGroup* pGroup : m_Groups)
	{
		if (FAILED(pGroup->Render()))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CMesh::SetTexture(const UINT _nGroupIndex, CTexture* const _pTexture)
{
	if (m_Groups.size() <= _nGroupIndex)
		return E_FAIL;

	return m_Groups[_nGroupIndex]->SetTexture(_pTexture);
}

CMesh * CMesh::Create(const TSTRING& _sFileName)
{
	CMesh* pInstance = new CMesh;
	if (FAILED(pInstance->LoadMesh(_sFileName)))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

CMesh * CMesh::Clone()
{
	CMesh* pClone = new CMesh(*this);

	return pClone;
}
