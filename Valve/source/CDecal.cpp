#include "cbase.h"
#include "soundent.h"
#include "client.h"
#include "decals.h"
#include "EnvMessage.h"
#include "player.h"
#include "gamerules.h"
#include "teamplay_gamerules.h"
#include "physics.h"
#include "isaverestore.h"
#include "activitylist.h"
#include "eventlist.h"
#include "eventqueue.h"
#include "ai_network.h"
#include "ai_schedule.h"
#include "ai_networkmanager.h"
#include "ai_utils.h"
#include "basetempentity.h"
#include "world.h"
#include "mempool.h"
#include "igamesystem.h"
#include "engine/IEngineSound.h"
#include "globals.h"
#include "engine/IStaticPropMgr.h"
#include "particle_parse.h"
#include "globalstate.h"
#include "tier0/memdbgon.h"

extern CBaseEntity	*g_pLastSpawn;
void InitBodyQue(void);
extern void W_Precache(void);
extern void ActivityList_Free( void );
extern CUtlMemoryPool g_EntityListPool;

#define SF_DECAL_NOTINDEATHMATCH		2048

class CDecal : public CPointEntity
{
public:
	DECLARE_CLASS( CDecal, CPointEntity );

	void	Spawn( void );
	bool	KeyValue( const char *szKeyName, const char *szValue );


	virtual void Activate();
	void	TriggerDecal( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void	InputActivate( inputdata_t &inputdata );

	DECLARE_DATADESC();

public:
	int		m_nTexture;
	bool	m_bLowPriority;

private:

	void	StaticDecal( void );
};

BEGIN_DATADESC( CDecal )

	DEFINE_FIELD( m_nTexture, FIELD_INTEGER ),
	DEFINE_KEYFIELD( m_bLowPriority, FIELD_BOOLEAN, "LowPriority" ),
	DEFINE_FUNCTION( StaticDecal ),
	DEFINE_FUNCTION( TriggerDecal ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Activate", InputActivate ),

END_DATADESC()

LINK_ENTITY_TO_CLASS( infodecal, CDecal );


void CDecal::Spawn( void )
{
	if ( m_nTexture < 0 || 
		(gpGlobals->deathmatch && HasSpawnFlags( SF_DECAL_NOTINDEATHMATCH )) )
	{
		UTIL_Remove( this );
		return;
	} 
}

void CDecal::Activate()
{
	BaseClass::Activate();

	if ( !GetEntityName() )
	{
		StaticDecal();
	}
	else
	{
		SetThink ( &CDecal::SUB_DoNothing );
		SetUse(&CDecal::TriggerDecal);
	}
}

void CDecal::TriggerDecal ( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	trace_t		trace;
	int			entityIndex;

	UTIL_TraceLine( GetAbsOrigin() - Vector(5,5,5), GetAbsOrigin() + Vector(5,5,5), MASK_SOLID_BRUSHONLY, this, COLLISION_GROUP_NONE, &trace );

	entityIndex = trace.m_pEnt ? trace.m_pEnt->entindex() : 0;

	CBroadcastRecipientFilter filter;

	te->BSPDecal( filter, 0.0, 
		&GetAbsOrigin(), entityIndex, m_nTexture );

	SetThink( &CDecal::SUB_Remove );
	SetNextThink( gpGlobals->curtime + 0.1f );
}


void CDecal::InputActivate( inputdata_t &inputdata )
{
	TriggerDecal( inputdata.pActivator, inputdata.pCaller, USE_ON, 0 );
}


void CDecal::StaticDecal( void )
{
	class CTraceFilterValidForDecal : public CTraceFilterSimple
	{
	public:
		CTraceFilterValidForDecal(const IHandleEntity *passentity, int collisionGroup )
		 :	CTraceFilterSimple( passentity, collisionGroup )
		{
		}

		virtual bool ShouldHitEntity( IHandleEntity *pServerEntity, int contentsMask )
		{
			static const char *ppszIgnoredClasses[] = {
				"weapon_*",
				"item_*",
				"prop_ragdoll",
				"prop_dynamic",
				"prop_static",
				"prop_physics",
				"npc_bullseye",  // Tracker 15335
			};

			CBaseEntity *pEntity = EntityFromEntityHandle( pServerEntity );

			if ( pEntity->IsEffectActive( EF_NODRAW ) )
				return false;

			for ( int i = 0; i < ARRAYSIZE(ppszIgnoredClasses); i++ )
			{
				if ( pEntity->ClassMatches( ppszIgnoredClasses[i] ) )
					return false;
			}


			return CTraceFilterSimple::ShouldHitEntity( pServerEntity, contentsMask );
		}
	};

	trace_t trace;
	CTraceFilterValidForDecal traceFilter( this, COLLISION_GROUP_NONE );
	int entityIndex, modelIndex = 0;

	Vector position = GetAbsOrigin();
	UTIL_TraceLine( position - Vector(5,5,5), position + Vector(5,5,5),  MASK_SOLID, &traceFilter, &trace );

	bool canDraw = true;

	entityIndex = trace.m_pEnt ? (short)trace.m_pEnt->entindex() : 0;
	if ( entityIndex )
	{
		CBaseEntity *ent = trace.m_pEnt;
		if ( ent )
		{
			modelIndex = ent->GetModelIndex();
			VectorITransform( GetAbsOrigin(), ent->EntityToWorldTransform(), position );

			canDraw = ( modelIndex != 0 );
			if ( !canDraw )
			{
				Warning( "Suppressed StaticDecal which would have hit entity %i (class:%s, name:%s) with modelindex = 0\n",
					ent->entindex(),
					ent->GetClassname(),
					STRING( ent->GetEntityName() ) );
			}
		}
	}

	if ( canDraw )
	{
		engine->StaticDecal( position, m_nTexture, entityIndex, modelIndex, m_bLowPriority );
	}

	SUB_Remove();
}


bool CDecal::KeyValue( const char *szKeyName, const char *szValue )
{
	if (FStrEq(szKeyName, "texture"))
	{
		m_nTexture = UTIL_PrecacheDecal( szValue, true );
		
		// Found
		if (m_nTexture >= 0 )
			return true;
		Warning( "Can't find decal %s\n", szValue );
	}
	else
	{
		return BaseClass::KeyValue( szKeyName, szValue );
	}

	return true;
}

class CProjectedDecal : public CPointEntity
{
public:
	DECLARE_CLASS( CProjectedDecal, CPointEntity );

	void	Spawn( void );
	bool	KeyValue( const char *szKeyName, const char *szValue );
	virtual void Activate();
	void	TriggerDecal( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void	InputActivate( inputdata_t &inputdata );

	DECLARE_DATADESC();

public:
	int		m_nTexture;
	float	m_flDistance;

private:
	void	ProjectDecal( CRecipientFilter& filter );

	void	StaticDecal( void );
};

BEGIN_DATADESC( CProjectedDecal )

	DEFINE_FIELD( m_nTexture, FIELD_INTEGER ),
	DEFINE_KEYFIELD( m_flDistance, FIELD_FLOAT, "Distance" ),
	DEFINE_FUNCTION( StaticDecal ),
	DEFINE_FUNCTION( TriggerDecal ),

	DEFINE_INPUTFUNC( FIELD_VOID, "Activate", InputActivate ),

END_DATADESC()

LINK_ENTITY_TO_CLASS( info_projecteddecal, CProjectedDecal );

void CProjectedDecal::Spawn( void )
{
	if ( m_nTexture < 0 || 
		(gpGlobals->deathmatch && HasSpawnFlags( SF_DECAL_NOTINDEATHMATCH )) )
	{
		UTIL_Remove( this );
		return;
	} 
}

void CProjectedDecal::Activate()
{
	BaseClass::Activate();

	if ( !GetEntityName() )
	{
		StaticDecal();
	}
	else
	{
		SetThink ( &CProjectedDecal::SUB_DoNothing );
		SetUse(&CProjectedDecal::TriggerDecal);
	}
}

void CProjectedDecal::InputActivate( inputdata_t &inputdata )
{
	TriggerDecal( inputdata.pActivator, inputdata.pCaller, USE_ON, 0 );
}

void CProjectedDecal::ProjectDecal( CRecipientFilter& filter )
{
	te->ProjectDecal( filter, 0.0, 
		&GetAbsOrigin(), &GetAbsAngles(), m_flDistance, m_nTexture );
}

void CProjectedDecal::TriggerDecal ( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	CBroadcastRecipientFilter filter;

	ProjectDecal( filter );

	SetThink( &CProjectedDecal::SUB_Remove );
	SetNextThink( gpGlobals->curtime + 0.1f );
}

void CProjectedDecal::StaticDecal( void )
{
	CBroadcastRecipientFilter initFilter;
	initFilter.MakeInitMessage();

	ProjectDecal( initFilter );

	SUB_Remove();
}


bool CProjectedDecal::KeyValue( const char *szKeyName, const char *szValue )
{
	if (FStrEq(szKeyName, "texture"))
	{
		m_nTexture = UTIL_PrecacheDecal( szValue, true );
		
		// Found
		if (m_nTexture >= 0 )
			return true;
		Warning( "Can't find decal %s\n", szValue );
	}
	else
	{
		return BaseClass::KeyValue( szKeyName, szValue );
	}

	return true;
}
LINK_ENTITY_TO_CLASS( worldspawn, CWorld );

BEGIN_DATADESC( CWorld )

	DEFINE_FIELD( m_flWaveHeight, FIELD_FLOAT ),

	DEFINE_KEYFIELD( m_iszChapterTitle, FIELD_STRING, "chaptertitle" ),
	DEFINE_KEYFIELD( m_bStartDark, FIELD_BOOLEAN, "startdark" ),
	DEFINE_KEYFIELD( m_bDisplayTitle,	FIELD_BOOLEAN, "gametitle" ),
	DEFINE_FIELD( m_WorldMins, FIELD_VECTOR ),
	DEFINE_FIELD( m_WorldMaxs, FIELD_VECTOR ),
#ifdef _X360
	DEFINE_KEYFIELD( m_flMaxOccludeeArea, FIELD_FLOAT, "maxoccludeearea_x360" ),
	DEFINE_KEYFIELD( m_flMinOccluderArea, FIELD_FLOAT, "minoccluderarea_x360" ),
#else
	DEFINE_KEYFIELD( m_flMaxOccludeeArea, FIELD_FLOAT, "maxoccludeearea" ),
	DEFINE_KEYFIELD( m_flMinOccluderArea, FIELD_FLOAT, "minoccluderarea" ),
#endif
	DEFINE_KEYFIELD( m_flMaxPropScreenSpaceWidth, FIELD_FLOAT, "maxpropscreenwidth" ),
	DEFINE_KEYFIELD( m_flMinPropScreenSpaceWidth, FIELD_FLOAT, "minpropscreenwidth" ),
	DEFINE_KEYFIELD( m_iszDetailSpriteMaterial, FIELD_STRING, "detailmaterial" ),
	DEFINE_KEYFIELD( m_bColdWorld, FIELD_BOOLEAN, "coldworld" ),

END_DATADESC()


// SendTable stuff.
IMPLEMENT_SERVERCLASS_ST(CWorld, DT_WORLD)
	SendPropFloat	(SENDINFO(m_flWaveHeight), 8, SPROP_ROUNDUP,	0.0f,	8.0f),
	SendPropVector (SENDINFO(m_WorldMins),	-1,	SPROP_COORD),
	SendPropVector(SENDINFO(m_WorldMaxs),	-1,	SPROP_COORD),
	SendPropInt	(SENDINFO(m_bStartDark), 1, SPROP_UNSIGNED ),
	SendPropFloat	(SENDINFO(m_flMaxOccludeeArea), 0, SPROP_NOSCALE ),
	SendPropFloat	(SENDINFO(m_flMinOccluderArea), 0, SPROP_NOSCALE ),
	SendPropFloat	(SENDINFO(m_flMaxPropScreenSpaceWidth), 0, SPROP_NOSCALE ),
	SendPropFloat	(SENDINFO(m_flMinPropScreenSpaceWidth), 0, SPROP_NOSCALE ),
	SendPropStringT (SENDINFO(m_iszDetailSpriteMaterial) ),
	SendPropInt	(SENDINFO(m_bColdWorld), 1, SPROP_UNSIGNED ),
END_SEND_TABLE()

bool CWorld::KeyValue( const char *szKeyName, const char *szValue )
{
	if ( FStrEq(szKeyName, "skyname") )
	{
		ConVarRef skyname( "sv_skyname" );
		skyname.SetValue( szValue );
	}
	else if ( FStrEq(szKeyName, "newunit") )
	{
		if ( atoi(szValue) )
		{
			extern void Game_SetOneWayTransition();
			Game_SetOneWayTransition();
		}
	}
	else if ( FStrEq(szKeyName, "world_mins") )
	{
		Vector vec;
		sscanf(	szValue, "%f %f %f", &vec.x, &vec.y, &vec.z );
		m_WorldMins = vec;
	}
	else if ( FStrEq(szKeyName, "world_maxs") )
	{
		Vector vec;
		sscanf(	szValue, "%f %f %f", &vec.x, &vec.y, &vec.z ); 
		m_WorldMaxs = vec;
	}
	else
		return BaseClass::KeyValue( szKeyName, szValue );

	return true;
}
extern bool		g_fGameOver;
static CWorld *g_WorldEntity = NULL;

CWorld* GetWorldEntity()
{
	return g_WorldEntity;
}

CWorld::CWorld( )
{
	AddEFlags( EFL_NO_AUTO_EDICT_ATTACH | EFL_KEEP_ON_RECREATE_ENTITIES );
	NetworkProp()->AttachEdict( INDEXENT(RequiredEdictIndex()) );
	ActivityList_Init();
	EventList_Init();
	SetSolid( SOLID_BSP );
	SetMoveType( MOVETYPE_NONE );

	m_bColdWorld = false;
}

CWorld::~CWorld( )
{
	EventList_Free();
	ActivityList_Free();
	if ( g_pGameRules )
	{
		g_pGameRules->LevelShutdown();
		delete g_pGameRules;
		g_pGameRules = NULL;
	}
	g_WorldEntity = NULL;
}
void CWorld::DecalTrace( trace_t *pTrace, char const *decalName)
{
	int index = decalsystem->GetDecalIndexForName( decalName );
	if ( index < 0 )
		return;

	CBroadcastRecipientFilter filter;
	if ( pTrace->hitbox != 0 )
	{
		te->Decal( filter, 0.0f, &pTrace->endpos, &pTrace->startpos, 0, pTrace->hitbox, index );
	}
	else
	{
		te->WorldDecal( filter, 0.0, &pTrace->endpos, index );
	}
}

void CWorld::RegisterSharedActivities( void )
{
	ActivityList_RegisterSharedActivities();
}

void CWorld::RegisterSharedEvents( void )
{
	EventList_RegisterSharedEvents();
}


void CWorld::Spawn( void )
{
	SetLocalOrigin( vec3_origin );
	SetLocalAngles( vec3_angle );
	SetModelIndex( 1 );
	SetModelName( AllocPooledString( modelinfo->GetModelName( GetModel() ) ) );
	AddFlag( FL_WORLDBRUSH );
	g_EventQueue.Init();
	Precache( );
	GlobalEntity_Add( "is_console", STRING(gpGlobals->mapname), ( IsConsole() ) ? GLOBAL_ON : GLOBAL_OFF );
	GlobalEntity_Add( "is_pc", STRING(gpGlobals->mapname), ( !IsConsole() ) ? GLOBAL_ON : GLOBAL_OFF );
}

static const char *g_DefaultLightstyles[] =
{
	"m",
	// 1 FLICKER (first variety)
	"mmnmmommommnonmmonqnmmo",
	// 2 SLOW STRONG PULSE
	"abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba",
	// 3 CANDLE (first variety)
	"mmmmmaaaaammmmmaaaaaabcdefgabcdefg",
	// 4 FAST STROBE
	"mamamamamama",
	// 5 GENTLE PULSE 1
	"jklmnopqrstuvwxyzyxwvutsrqponmlkj",
	// 6 FLICKER (second variety)
	"nmonqnmomnmomomno",
	// 7 CANDLE (second variety)
	"mmmaaaabcdefgmmmmaaaammmaamm",
	// 8 CANDLE (third variety)
	"mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa",
	// 9 SLOW STROBE (fourth variety)
	"aaaaaaaazzzzzzzz",
	// 10 FLUORESCENT FLICKER
	"mmamammmmammamamaaamammma",
	// 11 SLOW PULSE NOT FADE TO BLACK
	"abcdefghijklmnopqrrqponmlkjihgfedcba",
	// 12 UNDERWATER LIGHT MUTATION
	// this light only distorts the lightmap - no contribution
	// is made to the brightness of affected surfaces
	"mmnnmmnnnmmnn",
};


const char *GetDefaultLightstyleString( int styleIndex )
{
	if ( styleIndex < ARRAYSIZE(g_DefaultLightstyles) )
	{
		return g_DefaultLightstyles[styleIndex];
	}
	return "m";
}

void CWorld::Precache( void )
{
	g_WorldEntity = this;
	g_fGameOver = false;
	g_pLastSpawn = NULL;
	ConVarRef stepsize( "sv_stepsize" );
	stepsize.SetValue( 18 );
	ConVarRef roomtype( "room_type" );
	roomtype.SetValue( 0 );

	// Set up game rules
	Assert( !g_pGameRules );
	if (g_pGameRules)
	{
		delete g_pGameRules;
	}

	InstallGameRules();
	Assert( g_pGameRules );
	g_pGameRules->Init();
	CSoundEnt::InitSoundEnt();
	CBaseEntity::SetAllowPrecache( true );
	IGameSystem::LevelInitPreEntityAllSystems( STRING( GetModelName() ) );

	// Create the player resource
	g_pGameRules->CreateStandardEntities();
	ActivityList_Free();
	RegisterSharedActivities();
	EventList_Free();
	RegisterSharedEvents();
	InitBodyQue();
	SENTENCEG_Init();
	PrecacheStandardParticleSystems();     
	W_Precache ();	// get weapon precaches
	ClientPrecache();
	g_pGameRules->Precache();
	// precache all temp ent stuff
	CBaseTempEntity::PrecacheTempEnts();
	g_Language.SetValue( LANGUAGE_ENGLISH );	// TODO use VGUI to get current language

	if ( g_Language.GetInt() == LANGUAGE_GERMAN )
	{
		PrecacheModel( "models/germangibs.mdl" );
	}
	else
	{
		PrecacheModel( "models/gibs/hgibs.mdl" );
	}
	PrecacheScriptSound( "BaseEntity.EnterWater" );
	PrecacheScriptSound( "BaseEntity.ExitWater" );

	for ( int i = 0; i < ARRAYSIZE(g_DefaultLightstyles); i++ )
	{
		engine->LightStyle( i, GetDefaultLightstyleString(i) );
	}

	engine->LightStyle(63, "a");
	CAI_NetworkManager::InitializeAINetworks();
	g_AI_SchedulesManager.LoadAllSchedules();
	g_pGameRules->InitDefaultAIRelationships();
	CBaseCombatCharacter::InitInteractionSystem();
	// Call all registered precachers.
	CPrecacheRegister::Precache();	

	if ( m_iszChapterTitle != NULL_STRING )
	{
		DevMsg( 2, "Chapter title: %s\n", STRING(m_iszChapterTitle) );
		CMessage *pMessage = (CMessage *)CBaseEntity::Create( "env_message", vec3_origin, vec3_angle, NULL );
		if ( pMessage )
		{
			pMessage->SetMessage( m_iszChapterTitle );
			m_iszChapterTitle = NULL_STRING;
			pMessage->AddSpawnFlags( SF_MESSAGE_ONCE );
			pMessage->SetThink( &CMessage::SUB_CallUseToggle );
			pMessage->SetNextThink( gpGlobals->curtime + 1.0f );
		}
	}

	g_iszFuncBrushClassname = AllocPooledString("func_brush");
}
float GetRealTime()
{
	return engine->Time();
}

bool CWorld::GetDisplayTitle() const
{
	return m_bDisplayTitle;
}

bool CWorld::GetStartDark() const
{
	return m_bStartDark;
}

void CWorld::SetDisplayTitle( bool display )
{
	m_bDisplayTitle = display;
}

void CWorld::SetStartDark( bool startdark )
{
	m_bStartDark = startdark;
}

bool CWorld::IsColdWorld( void )
{
	return m_bColdWorld;
}
