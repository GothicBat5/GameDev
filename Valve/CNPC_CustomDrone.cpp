#include "cbase.h"
#include "ai_basenpc.h"

class CNPC_CustomDrone : public CAI_BaseNPC
{
	DECLARE_CLASS( CNPC_CustomDrone, CAI_BaseNPC );
	DECLARE_DATADESC();

public:
	CNPC_CustomDrone();
	~CNPC_CustomDrone();

	virtual void Spawn() override;
	virtual void Precache() override;
	virtual void Think() override;

private:
	float m_flNextBuzzTime;
};

LINK_ENTITY_TO_CLASS( npc_custom_drone, CNPC_CustomDrone );

BEGIN_DATADESC( CNPC_CustomDrone )
	DEFINE_FIELD( m_flNextBuzzTime, FIELD_TIME ),
END_DATADESC()

CNPC_CustomDrone::CNPC_CustomDrone()
{
	m_flNextBuzzTime = 0.0f;
}

CNPC_CustomDrone::~CNPC_CustomDrone()
{
}

void CNPC_CustomDrone::Precache()
{
	PrecacheModel( "models/props_combine/combine_drone.mdl" );
	BaseClass::Precache();
}

void CNPC_CustomDrone::Spawn()
{
	Precache();
	SetModel( "models/props_combine/combine_drone.mdl" );
	BaseClass::Spawn();

	SetSolid( SOLID_BBOX );
	SetMoveType( MOVETYPE_FLY );
	SetHealth( 50 );
	
	m_NPCState = NPC_STATE_IDLE;
	SetThink( &CNPC_CustomDrone::Think );
	SetNextThink( gpGlobals->curtime + 0.1f );
}

void CNPC_CustomDrone::Think()
{
	// Simple hovering + buzz sound every 2 seconds
	if ( gpGlobals->curtime > m_flNextBuzzTime )
	{
		EmitSound( "NPC_CombineDrone.Idle" );
		m_flNextBuzzTime = gpGlobals->curtime + 2.0f;
	}

	// Make it hover up/down
	Vector newPos = GetAbsOrigin();
	newPos.z += sin(gpGlobals->curtime * 3.0f) * 0.5f;
	SetAbsOrigin( newPos );

	SetNextThink( gpGlobals->curtime + 0.05f );
}
