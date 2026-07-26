#ifndef STEAM_API_H
#define STEAM_API_H
#ifdef _WIN32
#pragma once
#endif

#include "steam_api_common.h"
#include "isteamclient.h"
#include "isteamuser.h"
#include "isteamfriends.h"
#include "isteamutils.h"
#include "isteammatchmaking.h"
#include "isteamuserstats.h"
#include "isteamapps.h"
#include "isteamnetworking.h"
#include "isteamremotestorage.h"
#include "isteamscreenshots.h"
#include "isteammusic.h"
#include "isteammusicremote.h"
#include "isteamhttp.h"
#include "isteamcontroller.h"
#include "isteamugc.h"
#include "isteamapplist.h"
#include "isteamhtmlsurface.h"
#include "isteaminventory.h"
#include "isteamvideo.h"
#include "isteamparentalsettings.h"
#include "isteaminput.h"
#include "isteamremoteplay.h"
#include "isteamnetworkingmessages.h"
#include "isteamnetworkingsockets.h"
#include "isteamnetworkingutils.h"

S_API bool S_CALLTYPE SteamAPI_Init();
S_API void S_CALLTYPE SteamAPI_Shutdown();
S_API bool S_CALLTYPE SteamAPI_RestartAppIfNecessary( uint32 unOwnAppID );
S_API void S_CALLTYPE SteamAPI_ReleaseCurrentThreadMemory();
S_API void S_CALLTYPE SteamAPI_WriteMiniDump( uint32 uStructuredExceptionCode, void* pvExceptionInfo, uint32 uBuildID );
S_API void S_CALLTYPE SteamAPI_SetMiniDumpComment( const char *pchMsg );
S_API bool S_CALLTYPE SteamAPI_IsSteamRunning();
S_API const char *SteamAPI_GetSteamInstallPath();
S_API void SteamAPI_SetTryCatchCallbacks( bool bTryCatchCallbacks );

#if defined( VERSION_SAFE_STEAM_API_INTERFACES )

S_API bool S_CALLTYPE SteamAPI_InitSafe();
#endif

#if defined(USE_BREAKPAD_HANDLER) || defined(STEAM_API_EXPORTS)

S_API void S_CALLTYPE SteamAPI_UseBreakpadCrashHandler( char const *pchVersion, char const *pchDate, 
char const *pchTime, bool bFullMemoryDumps, 
void *pvContext, PFNPreMinidumpCallback m_pfnPreMinidumpCallback );

S_API void S_CALLTYPE SteamAPI_SetBreakpadAppID( uint32 unAppID );
#endif

S_API void S_CALLTYPE SteamAPI_ManualDispatch_Init();
S_API void S_CALLTYPE SteamAPI_ManualDispatch_RunFrame( HSteamPipe hSteamPipe );
S_API bool S_CALLTYPE SteamAPI_ManualDispatch_GetNextCallback( HSteamPipe hSteamPipe, CallbackMsg_t *pCallbackMsg );
S_API void S_CALLTYPE SteamAPI_ManualDispatch_FreeLastCallback( HSteamPipe hSteamPipe );
S_API bool S_CALLTYPE SteamAPI_ManualDispatch_GetAPICallResult( HSteamPipe hSteamPipe, SteamAPICall_t hSteamAPICall, void *pCallback, int cubCallback, int iCallbackExpected, bool *pbFailed );

#ifndef STEAM_API_EXPORTS

inline bool CSteamAPIContext::Init()
{
	m_pSteamClient = ::SteamClient();
	if ( !m_pSteamClient )
		return false;

	m_pSteamUser = ::SteamUser();
	if ( !m_pSteamUser )
		return false;

	m_pSteamFriends = ::SteamFriends();
	if ( !m_pSteamFriends )
		return false;

	m_pSteamUtils = ::SteamUtils();
	if ( !m_pSteamUtils )
		return false;

	m_pSteamMatchmaking = ::SteamMatchmaking();
	if ( !m_pSteamMatchmaking )
		return false;

	m_pSteamGameSearch = ::SteamGameSearch();
	if ( !m_pSteamGameSearch )
		return false;

#if !defined(IOSALL)
	m_pSteamMatchmakingServers = ::SteamMatchmakingServers();
	if ( !m_pSteamMatchmakingServers )
		return false;
#endif

	m_pSteamUserStats = ::SteamUserStats();
	if ( !m_pSteamUserStats )
		return false;

	m_pSteamApps = ::SteamApps();
	if ( !m_pSteamApps )
		return false;

	m_pSteamNetworking = ::SteamNetworking();
	if ( !m_pSteamNetworking )
		return false;

	m_pSteamRemoteStorage = ::SteamRemoteStorage();
	if ( !m_pSteamRemoteStorage )
		return false;

	m_pSteamScreenshots = ::SteamScreenshots();
	if ( !m_pSteamScreenshots )
		return false;

	m_pSteamHTTP = ::SteamHTTP();
	if ( !m_pSteamHTTP )
		return false;

	m_pController = ::SteamController();
	if ( !m_pController )
		return false;

	m_pSteamUGC = ::SteamUGC();
	if ( !m_pSteamUGC )
		return false;

	m_pSteamAppList = ::SteamAppList();
	if ( !m_pSteamAppList )
		return false;

	m_pSteamMusic = ::SteamMusic();
	if ( !m_pSteamMusic )
		return false;

	m_pSteamMusicRemote = ::SteamMusicRemote();
	if ( !m_pSteamMusicRemote )
		return false;

#if !defined( ANDROID ) && !defined( IOSALL)
	if ( !m_pSteamHTMLSurface )
	return false;
#endif

	m_pSteamInventory = ::SteamInventory();
	if ( !m_pSteamInventory )
		return false;

	m_pSteamVideo = ::SteamVideo();
	if ( !m_pSteamVideo )
		return false;

	m_pSteamParentalSettings = ::SteamParentalSettings();
	if ( !m_pSteamParentalSettings )
		return false;

	m_pSteamInput = ::SteamInput();
	if ( !m_pSteamInput )
		return false;

	return true;
}
#endif
#endif // STEAM_API_H