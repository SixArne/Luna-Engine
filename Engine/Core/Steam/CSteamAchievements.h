#ifndef C_STEAM_ACHIEVEMENTS_H
#define C_STEAM_ACHIEVEMENTS_H

#include "Achievement.h"

#pragma warning(push)
#pragma warning(disable: 4389)
#pragma warning(disable: 4996)
#include <steam_api.h>
#pragma warning(pop)

#include "Singleton.h"

class CSteamAchievements : public Engine::Singleton<CSteamAchievements>
{
private:
	int64 m_iAppID; // Our current AppID
	Achievement_t* m_pAchievements; // Achievements data
	int m_iNumAchievements; // The number of Achievements
	bool m_bInitialized; // Have we called Request stats and received the callback?

public:
	CSteamAchievements();

	void Setup(Achievement_t* Achievements, int NumAchievements);

	bool RequestStats();
	bool SetAchievement(const char* ID);

	STEAM_CALLBACK(CSteamAchievements, OnUserStatsReceived, UserStatsReceived_t,
		m_CallbackUserStatsReceived);
	STEAM_CALLBACK(CSteamAchievements, OnUserStatsStored, UserStatsStored_t,
		m_CallbackUserStatsStored);
	STEAM_CALLBACK(CSteamAchievements, OnAchievementStored,
		UserAchievementStored_t, m_CallbackAchievementStored);

private:
	bool m_IsSetup{ false };
};

#endif