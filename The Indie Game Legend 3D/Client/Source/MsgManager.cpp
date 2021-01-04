#include "stdafx.h"
#include "MsgManager.h"
#include "SoundMgr.h"
#include "BigBullet.h"
#include "LightMananger.h"
#include "Focus.h"

IMPLEMENT_SINGLETON(CMsgManager)
POINT CMsgManager::GetClientCenterPoint()
{
	POINT pt = { (_int)m_vClinetCenterPos.x,(_int)m_vClinetCenterPos.y };
	return pt;
}

/*WndProc size �޽��� ȣ��� Ŭ��ũ�� ����Ǿ����� �缳��*/
void CMsgManager::ReSizeClient()
{
	GetClientRect(g_hWnd, &m_tClinetRect);
	m_vClinetCenterPos = _vector(
		(float)((m_tClinetRect.right - m_tClinetRect.left) / 2),
		(float)((m_tClinetRect.bottom - m_tClinetRect.top) / 2), 0.f);
}

void CMsgManager::Freeze(const float* _fTimeDeleta)
{
	if (m_bFreezeTrigger)
	{
		float* fTime = const_cast<float*>(_fTimeDeleta);
		*fTime = 0.f;
	}
}

void CMsgManager::AirStrikeFire()
{
	static mt19937 engine((_uint)time(NULL));
	float fInterval = 10.f;
	static uniform_real_distribution<> distribution(-fInterval, fInterval);    // ���� ����	
	static auto RandomValue = bind(distribution, engine);
	_vector vFirePos = _vector(m_vAirStrikePos.x + (float)RandomValue(), 100.f, m_vAirStrikePos.z + (float)RandomValue());
	CGameObject* pBullet =  CManagement::GetInstance()->Instantiate<CBigBullet>(m_nSceneID);
	((CBigBullet*)pBullet)->Fire(vFirePos,_vector(0, -1, 0));
}

void CMsgManager::UpdateSkillTime(const float & _fTimeDelta)
{
	if (m_bFreezeTrigger)
	{
		m_fSkillDurationTime += _fTimeDelta;
		if (m_fSkillDuration < m_fSkillDurationTime)
		{
			m_bFreezeTrigger = false;
			m_bSkillCooldown = false;

			m_fSkillDurationTime = 0.f;
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::PlayerSkill);
		}
	}
	else if (m_bAutoAimTrigger)
	{
		m_fSkillDurationTime += _fTimeDelta;
		if (m_fSkillDuration < m_fSkillDurationTime)
		{
			m_bAutoAimTrigger = false;
			m_bSkillCooldown = false;

			m_fSkillDurationTime = 0.f;
			CSoundMgr::GetInstance()->StopSound(CSoundMgr::PlayerSkill);
		}
	}
	else if (m_bAirStrikeTrigger)
	{
		m_fAirStrikeTime += _fTimeDelta;
		if(m_bAirStrikeSceneTrigger)
			WargingLight(_fTimeDelta);

		if (m_bAirStrikeSceneTrigger  && m_fAirStrikeCutSceneDuration < m_fAirStrikeTime)
		{
			m_bAirStrikeSceneTrigger = false;
			m_fAirStrikeTime = 0.f;

			//light Reset
			D3DLIGHT9* pLight;
		
			for (int i = CLightMananger::World1; i < CLightMananger::World5; ++i)
			{
				pLight = CLightMananger::GetInstance()->GetLight(CLightMananger::LightID(i));
				pLight->Diffuse = CLightMananger::GetInstance()->GetSaveColor(CLightMananger::LightID(i));
				CLightMananger::GetInstance()->SetLight(CLightMananger::LightID(i));
			}

			//focus off
			CGameObject* pFocus = CManagement::GetInstance()->FindGameObjectOfType<CFocus>(m_nSceneID);
			pFocus->SetEnable(false);

		}
		//�߻簣��
		else if (m_bAirStrikeSceneTrigger == false && m_fAirStrikeDelay < m_fAirStrikeTime)
		{
			++m_nAirStrikeCount;
			AirStrikeFire();
			m_fAirStrikeTime = 0.f;
			if (m_nAirStrikeCount >= AirStrikeBulletCount)
			{
				m_bAirStrikeTrigger  = false;
				CSoundMgr::GetInstance()->StopSound( CSoundMgr::PlayerSkill);
				m_bSkillCooldown = false;

			}
		}
	}
}

void CMsgManager::WargingLight(const float& _fTimeDelta)
{
	D3DLIGHT9* pLight;
	m_fSirenLightColor += m_fSirenLightChangValue * _fTimeDelta;

	if (m_fSirenLightColor > 150.f)
		m_fSirenLightChangValue *= -1;
	else if (m_fSirenLightColor < 0)
		m_fSirenLightChangValue *= -1;

	m_fSirenLightColor = CLAMP(m_fSirenLightColor, 0.f, 150.f);
	cout << (int)m_fSirenLightColor << endl;
	D3DXCOLOR color = D3DCOLOR_XRGB(230, (int)m_fSirenLightColor, (int)m_fSirenLightColor);
	for (int i = CLightMananger::World1; i < CLightMananger::World5; ++i)
	{
		pLight = CLightMananger::GetInstance()->GetLight(CLightMananger::LightID(i));
		pLight->Diffuse = color;
		CLightMananger::GetInstance()->SetLight(CLightMananger::LightID(i));
	}
}

void CMsgManager::FreezingStart(const float & _fTime)
{
	if (m_bSkillCooldown == false)
	{
		m_bSkillCooldown = true;
		m_bFreezeTrigger = true;
		m_fSkillDuration = _fTime;
		CSoundMgr::GetInstance()->Play(L"sfxClack.wma", CSoundMgr::PlayerSkill);
	}
}

void CMsgManager::AutoAimStart(const float & _fTime)
{
	if (m_bSkillCooldown == false)
	{
		m_bSkillCooldown = true;
		m_bAutoAimTrigger = true;
		m_fSkillDuration = _fTime;
		CSoundMgr::GetInstance()->Play(L"sfxAutoAim.mp3", CSoundMgr::PlayerSkill);
	}
}

void CMsgManager::AirStrikeSetting(const _uint& _nSceneID, const _vector& _vPosition)
{
	m_vAirStrikePos = _vPosition;
	m_nSceneID = _nSceneID;
	m_bAirStrikeTrigger = true;
	m_bAirStrikeSceneTrigger = true;
	m_bAirStrikeReady = false;
	m_nAirStrikeCount = 0;
	m_fAirStrikeTime = 0.f;
	m_fSirenLightColor = 255.f;
	CSoundMgr::GetInstance()->Play(L"sfxSiren4.MP3", CSoundMgr::PlayerSkill);
}

void CMsgManager::AirStrikeReady()
{
	if (m_bSkillCooldown == false)
	{
		m_bSkillCooldown = true;
		m_bAirStrikeReady = true;
	}
}

CMsgManager::CMsgManager()
	:m_fSkillDuration(0.f)
	,m_fSkillDurationTime(0.f)
	, m_bFreezeTrigger(false)
	, m_bAutoAimTrigger(false)
	, m_bAirStrikeTrigger(false)
	, m_bAirStrikeReady(false)
	, m_nAirStrikeCount(0)
	, m_fAirStrikeDelay(0.1f)
	, m_fAirStrikeTime(0.f)
	, m_fAirStrikeCutSceneDuration(8.0f)
	, m_bAirStrikeSceneTrigger(false)
	, m_fSirenLightChangValue(400.f)
	, m_bSkillCooldown(false)

{
	ZeroMemory(&m_tClinetRect, sizeof(RECT));
}

CMsgManager::~CMsgManager()
{
}

void CMsgManager::Free()
{
}
 