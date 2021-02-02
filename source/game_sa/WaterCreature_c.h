#pragma once

#include "ListItem_c.h"
#include "CObject.h"

class WaterCreature_c : public ListItem_c
{
public:
    WaterCreature_c();
    ~WaterCreature_c();
public:
    CObject* m_pObject;
    uint8_t m_nCreatureType; // see eWaterCreatureType
    uint8_t m_bDeleted;
    uint8_t m_ucForcedSwimSpeed;
    unsigned __int8 pad;
    int16_t m_wUnkn1;
    int16_t m_wUnkn2;
    float m_fMaxAngle;
    float m_fUnkn2;
    WaterCreature_c* m_pFollowedCreature;
    CVector m_vecOffsetFromFollowed;
    float m_fHeading;
    bool m_bChangedDir;

public:
    bool Init(int ID, CVector* pVecPos, WaterCreature_c* pParent, float fWaterLevel, float fWaterDepth);
    void Exit();
    void Update(float fTimeStep);
};
VALIDATE_SIZE(WaterCreature_c, 0x34);
