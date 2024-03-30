class PS_GameModeQuickTvTClass: PS_GameModeCoopClass
{
};

class PS_GameModeQuickTvT : PS_GameModeCoop
{
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
	
	int GetStepTime()
		return m_iStepTime;
	
	override void OnGameStart()
	{
		super.OnGameStart();
		
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
				m_iStepTime = m_iGameTime;
				break;
			case SCR_EGameModeState.DEBRIEFING:
				m_iStepTime = m_iDebriefingTime;
				break;
			case SCR_EGameModeState.POSTGAME:
				GameStateTransitions.RequestScenarioChangeTransition("{997EFE3C0E793E31}Missions/QuickTvT_TestWorld.conf", "");
				break;
		}
	}
};