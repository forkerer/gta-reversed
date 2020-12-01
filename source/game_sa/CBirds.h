/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "PluginBase.h"
#include "CVector.h"
#include "eBirdsBiome.h"

/*
    researched by LINK/2012
    for 'The Birds Update' mod
*/

#pragma pack(push, 1)
struct CBirdColor {
public:
    char cRed;
    char cGreen;
    char cBlue;
};
#pragma pack(pop)
VALIDATE_SIZE(CBirdColor, 0x3);

#pragma pack(push, 1)
class  CBird {
public:
    CVector     m_vecPosn;            // Bird position
    CVector     m_vecCurrentVelocity; // Velocity in the current frame
    CVector     m_vecTargetVelocity;  // Velocity (constant if the bird is not doing curves)
    float       m_fAngle;             // Bird angle
    int         field_28;             // Always 0
    int         m_nWingStillness;     // How still are the wings
    float       m_fSize;              // Bird size
    float       m_fMaxBirdDistance;   // When the bird is in this distance from the camera, it will disappear.
                                  // Used also in rendering process to determine the alpha level of the bird.
    char        field_38;             // Always 1, if different than 1 or 2 the bird will not be rendered
    CBirdColor  m_BodyColor;          // Body color
    CBirdColor  m_WingsColor;         // Wing tips color
    bool        m_bCreated;           // This flags indicates if in this index there's a bird created
    bool        m_bMustDoCurves;      // If this flag is true the bird will do curves
private:
    char _pad41[3];
public:
};
#pragma pack(pop)

VALIDATE_SIZE(CBird, 0x44);

class  CBirds {
public:
    static bool &bHasBirdBeenShot;
    static unsigned int &uiNumberOfBirds;
    static CBird *aBirds;
    static CVector &vecBirdShotAt;

    static void Init();
    static int CreateNumberOfBirds(CVector pos, CVector destination, int count, eBirdsBiome biome, bool checkObstacles);
    static void Shutdown();
    static void Update();
    static void Render();
    static void HandleGunShot(CVector const* pointA, CVector const* pointB);

    static void InjectHooks();
};

constexpr extern int32_t MAX_BIRDS = 6; // default: 6
extern float *BIRD_CREATION_COORS_X; // { 0.0f, -1.0f, 2.0f, -3.0f, 1.0f, -2.0f }
extern float *BIRD_CREATION_COORS_Y; // { 0.0f, -1.0f, -2.0f, 1.0f, 1.0f, -2.0f }
extern float *BIRD_CREATION_COORS_Z; // { 0.0f, 0.5f, 1.0f, 0.7f, 2.0f, 1.2f }
extern float *BIRD_CREATION_COORS_U; // { 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.5f }
extern float *BIRD_CREATION_COORS_V; // { 0.5f, 0.5f, 0.75f, 0.75f, 1.0f, 1.0f, 1.0f, 0.5f }
