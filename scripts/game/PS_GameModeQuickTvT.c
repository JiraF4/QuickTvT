class PS_GameModeQuickTvTClass: PS_GameModeCoopClass
{
};

class PS_GameModeQuickTvT : PS_GameModeCoop
{
	static const string m_QuickTvTConfigFilePath = "$profile:PS_QuickTvT_Config.json";
	protected ref PS_QuickTvTMissionsConfig m_QuickTvTMissionsConfig;
	
	[Attribute("15000", UIWidgets.EditBox, "", "", category: "Reforger Lobby")]
	int m_iPreviewTime;
	
	[Attribute("60000", UIWidgets.EditBox, "", "", category: "Reforger Lobby")]
	int m_iSlotsTime;
	
	[Attribute("60000", UIWidgets.EditBox, "", "", category: "Reforger Lobby")]
	int m_iBriefingTime;
	
	[Attribute("960000", UIWidgets.EditBox, "", "", category: "Reforger Lobby")]
	int m_iGameTime;
	
	[Attribute("30000", UIWidgets.EditBox, "", "", category: "Reforger Lobby")]
	int m_iDebriefingTime;
	
	protected int m_iStepTime;
	
	protected static int m_iMissionNum = 0;
	
	int GetStepTime()
		return m_iStepTime;
	
	override void OnGameStart()
	{
		super.OnGameStart();
		m_iMissionNum++;
		
		m_QuickTvTMissionsConfig = new PS_QuickTvTMissionsConfig();
		SCR_JsonLoadContext configLoadContext = new SCR_JsonLoadContext();
		configLoadContext.LoadFromFile(m_QuickTvTConfigFilePath);
		configLoadContext.ReadValue("", m_QuickTvTMissionsConfig);
		
		m_iStepTime = m_iPreviewTime;
	}
	
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		if (m_iStepTime > 0)
		{
			m_iStepTime -= timeSlice * 1000;
			
			if (m_iStepTime <= 0)	
				AdvanceGameState(GetState());
		}
	}
	
	override void OnGameStateChanged()
	{
		super.OnGameStateChanged();
		
		SCR_EGameModeState state = GetState();
		switch (state) 
		{
			case SCR_EGameModeState.PREGAME:
				m_iStepTime = m_iPreviewTime;
				break;
			case SCR_EGameModeState.SLOTSELECTION:
				m_iStepTime = m_iSlotsTime;
				break;
			case SCR_EGameModeState.CUTSCENE:
				break;
			case SCR_EGameModeState.BRIEFING:
				m_iStepTime = m_iBriefingTime;
				break;
			case SCR_EGameModeState.GAME:
				if (Replication.IsServer())
					GetGame().GetCallqueue().CallLater(CheckAlive, 3000, true);
				m_iStepTime = m_iGameTime;
				break;
			case SCR_EGameModeState.DEBRIEFING:
				GetGame().GetCallqueue().Remove(CheckAlive);
				m_iStepTime = m_iDebriefingTime;
				break;
			case SCR_EGameModeState.POSTGAME:
				ChangeToNextMission();
				break;
		}
	}
	
	void CheckAlive()
	{
		PS_PlayableManager playableManager = PS_PlayableManager.GetInstance();
		
		FactionKey checkFaction = "";
		array<PS_PlayableComponent> playables = playableManager.GetPlayablesSorted();
		foreach (PS_PlayableComponent playable : playables)
		{
			SCR_CharacterDamageManagerComponent characterDamageManagerComponent = playable.GetCharacterDamageManagerComponent();
			EDamageState damageState = characterDamageManagerComponent.GetState();
			if (damageState == EDamageState.DESTROYED)
				continue;
			
			FactionAffiliationComponent factionAffiliationComponent = playable.GetFactionAffiliationComponent();
			Faction faction = factionAffiliationComponent.GetDefaultAffiliatedFaction();
			FactionKey factionKey = faction.GetFactionKey();
			if (checkFaction != "" && checkFaction != factionKey)
			{
				return;
			}
			checkFaction = factionKey;
		}
			
		AdvanceGameState(SCR_EGameModeState.GAME);
		GetGame().GetCallqueue().Remove(CheckAlive);
	}
	
	void ChangeToNextMission()
	{
		if (m_iMissionNum >= m_QuickTvTMissionsConfig.Missions.Count())
		{
			m_iMissionNum = 0;
		}
		PS_QuickTvTMission mission = m_QuickTvTMissionsConfig.Missions[m_iMissionNum];
		GameStateTransitions.RequestScenarioChangeTransition(mission.MissionConfig, "");
	}
};

class PS_QuickTvTMissionsConfig: JsonApiStruct
{
	ref array<ref PS_QuickTvTMission> Missions = {};
	
	void PS_QuickTvTMissionsConfig()
	{
		RegV("Missions");
	}
}


class PS_QuickTvTMission: JsonApiStruct
{
	string MissionConfig;
	int MinPlayers;
	int MaxPlayers;
	
	void PS_QuickTvTMission()
	{
		RegV("MissionConfig");
		RegV("MinPlayers");
		RegV("MaxPlayers");
	}
}





















