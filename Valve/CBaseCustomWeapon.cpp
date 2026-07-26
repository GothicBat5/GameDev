#ifndef WEAPON_CUSTOM_H
#define WEAPON_CUSTOM_H

#include "basecombatweapon.h"
#include "weapon_custom.h"
#include "inkeys.h"
#include "player.h"

BEGIN_DATADESC(CBaseCustomWeapon)
    DEFINE_FIELD(m_flCustomReloadTime, FIELD_FLOAT),
    DEFINE_FIELD(m_iCustomDamage, FIELD_INTEGER),
END_DATADESC()

void CBaseCustomWeapon::Spawn()
{
    BaseClass::Spawn();

    m_flCustomReloadTime = 2.0f; // 2 seconds reload
    m_iCustomDamage = 25;        // Base damage

    // Set model (must be precached in Precache())
    SET_MODEL(edict(), "models/weapons/v_custom.mdl");
}

void CBaseCustomWeapon::Precache()
{
    BaseClass::Precache();

    precache_model("models/weapons/v_custom.mdl");
    precache_model("models/weapons/w_custom.mdl");
    precache_model("models/weapons/p_custom.mdl");
    precache_sound("weapons/custom_fire.wav");
}

bool CBaseCustomWeapon::Deploy()
{

    return BaseClass::Deploy();
}

void CBaseCustomWeapon::ItemPostFrame()
{

    if ((m_fInReload) && (m_pPlayer->m_flNextAttack <= UTIL_WeaponTimeBase()))
    {
        if (m_flNextAttack <= UTIL_WeaponTimeBase())
        {
            SecondaryAttack(); 
        }
    }

    if (m_pPlayer->m_afButtonPressed & IN_ATTACK)
    {
        if (m_iClip1 > 0)
        {
     
            m_iClip1--;
                
            EmitSound("weapons/custom_fire.wav");

            // Calculate spread and fire
            Vector vecSrc = m_pPlayer->GetGunPosition();
            Vector vecDir = m_pPlayer->GetAutoaimVector(AUTOAIM_5DEGREES);
            
            FireBullet(vecSrc, vecDir, 0.01f, (float)m_iCustomDamage, 8192.0f, 1);
            
            m_flNextAttack = UTIL_WeaponTimeBase() + 0.1f; 
        }
        else
        {
            // Dry fire
            PlayEmptySound();
            m_flNextAttack = UTIL_WeaponTimeBase() + 0.2f;
        }
    }

    BaseClass::ItemPostFrame();
}

void CBaseCustomWeapon::FireBullet(Vector vecSrc, 
Vector vecDirShooting, 
float flSpread, 
float flDamage, 
float flDistance, 
int iPenetration)

{
    // Simplified bullet trace logic
    CGameTrace tr;
    TRACE_MASK_SHOOT mask = MASK_SHOT;
    
    // Apply spread
    vecDirShooting.x += RANDOM_FLOAT(-flSpread, flSpread);
    vecDirShooting.y += RANDOM_FLOAT(-flSpread, flSpread);
    vecDirShooting.z += RANDOM_FLOAT(-flSpread, flSpread);
    vecDirShooting.NormalizeInPlace();

    UTIL_TraceLine(vecSrc, vecSrc + vecDirShooting * flDistance, mask, m_pPlayer, DMG_BULLET, &tr);

    if (tr.fStartSolid)
        return;

    // Apply damage
    CTakeDamageInfo info(m_pPlayer, m_pPlayer, vecDirShooting * flDamage, DMG_BULLET);
    
    // Trace hit
    if (tr.fHitWorld)
    {
        UTIL_MakeBubbles(tr.endPos, vecDirShooting, 0.1f); 
    }
    else
    {
        CBaseEntity* pEntity = tr.pEnt;
        if (pEntity && pEntity->IsAlive())
        {
            pEntity->TakeDamage(info);
        }
    }
    
    // Add effects (blood, sparks)
    UTIL_BloodDrips(tr.endPos, vecDirShooting, BLOOD_COLOR_RED, (int)flDamage);
}
