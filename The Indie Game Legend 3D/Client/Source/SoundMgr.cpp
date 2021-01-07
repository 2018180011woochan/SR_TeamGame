#include "stdafx.h"
#include "SoundMgr.h"

IMPLEMENT_SINGLETON(CSoundMgr)
CSoundMgr::CSoundMgr()
{
}

void CSoundMgr::Initialize()
{
	m_pSystem = nullptr;
	m_wstrNowBGM = L"";
	FMOD_System_Create(&m_pSystem);

	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);
	LoadSound(L"../Sound");
}

void CSoundMgr::Play(const wstring& pSoundKey, CHANNELID eID)
{
	map<wstring, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter) 
	{
		return pSoundKey == iter.first;
	});

	if (iter == m_mapSound.end()) 
	{
#ifdef _DEBUG
		cout << "not exist " << pSoundKey.c_str() << " file" << endl;
#endif // _DEBUG
		return;
	}

	FMOD_BOOL bPlay = FALSE; 
	FMOD_Channel_Stop(m_pChannelArr[eID]);
	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
	}
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlayContinue(const wstring & pSoundKey, CHANNELID eID)
{
	map<wstring, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter) {
		return pSoundKey == iter.first;
	});

	if (iter == m_mapSound.end())
	{
#ifdef _DEBUG
		cout << "not exist " << pSoundKey.c_str() << " file" << endl;
#endif // _DEBUG
		return;
	}

	FMOD_BOOL bPlay = FALSE;
	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
	}
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlayBGM(const wstring& _strSoundKey)
{
	map<wstring, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return _strSoundKey ==  iter.first;
	});

	if (iter == m_mapSound.end())
		return;

	if (m_wstrNowBGM == _strSoundKey)
		return;

	m_wstrNowBGM = _strSoundKey;

	FMOD_Channel_Stop(m_pChannelArr[BGM]);
	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::StopAll()
{
	for (int i = 0 ; i < MAXCHANNEL ; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::SetVolume(CHANNELID eID,float _Volume)
{
	_Volume = (_Volume > 1.f) ? 1.f : (_Volume < 0.f) ? 0.f : _Volume;
	FMOD_Channel_SetVolume(m_pChannelArr[eID], _Volume);
}
void CSoundMgr::SetPitch(CHANNELID _eID, float _fPitch)
{
	_fPitch = (_fPitch > 1.f) ? 1.f : (_fPitch < 0.f) ? 0.f : _fPitch;
	//FMOD_ChannelGroup_SetPitch(m_pChannelArr[_eID], _fPitch);

}
void CSoundMgr::Pause(CHANNELID _eID, const FMOD_BOOL & _Bool)
{
	FMOD_Channel_SetPaused(m_pChannelArr[_eID], _Bool);
}
//아래거로 바꾸긴 했는데 혹시 모르니 코드는 남겨놓음
void CSoundMgr::LoadSoundFile(char FilePath[])
{
	_finddata_t fd; 

	char szFilePath[MAX_PATH] = "";
	char szCurPath[128] ="";
	char szFullPath[128] = ""; 

	// ../Sound/*.*
	strcpy_s(szFilePath, FilePath);
	strcpy_s(szFilePath, "/*.*");

	// ../Sound/
	strcpy_s(szCurPath, FilePath);
	strcpy_s(szCurPath, "/");


	long handle = _findfirst(szFilePath, &fd);

	if (handle == 0)
		return; 

	int iResult = 0; 

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath); 
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr; 
			
		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);


		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1; 

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		iResult = _findnext(handle, &fd);
	}

	FMOD_System_Update(m_pSystem);
	_findclose(handle);
}

HRESULT CSoundMgr::LoadSound(const TSTRING & wstrPath)
{
	WIN32_FIND_DATA tFindData;
	TSTRING tCurPath = wstrPath + TEXT("/*.*");
	HANDLE hFind;
	hFind = FindFirstFile(tCurPath.c_str(), &tFindData);
	BOOL bContinue = false;
	if (hFind == INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return E_FAIL;
	}
	do
	{
		if (TEXT('.') == tFindData.cFileName[0])
		{
			bContinue = FindNextFile(hFind, &tFindData);
			continue;
		}
		else if (FILE_ATTRIBUTE_DIRECTORY == tFindData.dwFileAttributes)
		{
			LoadSound(wstrPath + TEXT('/') + tFindData.cFileName);
		}
		else
		{
			if (FILE_ATTRIBUTE_SYSTEM == tFindData.dwFileAttributes)
			{
				bContinue = FindNextFile(hFind, &tFindData);
				continue;
			};

			TSTRING tCurFullPath = wstrPath + L"/" + tFindData.cFileName;
			TSTRING tFileName = tFindData.cFileName;
			FMOD_SOUND* pSound = nullptr;
			FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, (const char*)tCurFullPath.c_str(), FMOD_HARDWARE | FMOD_UNICODE, 0, &pSound);

			if (eRes == FMOD_OK)
			{
				m_mapSound.emplace(tFileName, pSound);
			}
		}
			bContinue = FindNextFile(hFind, &tFindData);
	} while (bContinue);

	FMOD_System_Update(m_pSystem);
	FindClose(hFind);
	return S_OK;
}

void CSoundMgr::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
