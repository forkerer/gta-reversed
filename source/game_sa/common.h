/*
Plugin-SDK (Grand Theft Auto San Andreas) header file
Authors: GTA Community. See more here
https://github.com/DK22Pac/plugin-sdk
Do not delete this comment block. Respect others' work!
*/
#pragma once

#include <string>

#include "PluginBase.h"
#include "CVector.h"
#include "CEntity.h"
#include "CPlayerPed.h"
#include "CVehicle.h"
#include "CWeaponInfo.h"
#include "CAnimBlendAssociation.h"
#include "CAnimBlendClumpData.h"

const char gta_empty_string[4] = {0, 0, 0, 0};

#define DEFAULT_SCREEN_WIDTH (640)
#define DEFAULT_SCREEN_HEIGHT (448)
#define DEFAULT_SCREEN_HEIGHT_PAL (512)
#define DEFAULT_SCREEN_HEIGHT_NTSC (448)
#define DEFAULT_ASPECT_RATIO (4.0f/3.0f)
#define DEFAULT_VIEWWINDOW (0.7f)

// game uses maximumWidth/Height, but this probably won't work
// with RW windowed mode
#define SCREEN_WIDTH ((float)RsGlobal.maximumWidth)
#define SCREEN_HEIGHT ((float)RsGlobal.maximumHeight)
#define SCREEN_ASPECT_RATIO (CDraw::ms_fAspectRatio)
#define SCREEN_VIEWWINDOW (Tan(DEGTORAD(CDraw::GetScaledFOV() * 0.5f)))

// This scales from PS2 pixel coordinates to the real resolution
#define SCREEN_STRETCH_X(a)   ((a) * (float) SCREEN_WIDTH / DEFAULT_SCREEN_WIDTH)
#define SCREEN_STRETCH_Y(a)   ((a) * (float) SCREEN_HEIGHT / DEFAULT_SCREEN_HEIGHT)
#define SCREEN_STRETCH_FROM_RIGHT(a)  (SCREEN_WIDTH - SCREEN_STRETCH_X(a))
#define SCREEN_STRETCH_FROM_BOTTOM(a) (SCREEN_HEIGHT - SCREEN_STRETCH_Y(a))

// This scales from PS2 pixel coordinates while optionally maintaining the aspect ratio
#define SCREEN_SCALE_X(a) SCREEN_SCALE_AR(SCREEN_STRETCH_X(a))
#define SCREEN_SCALE_Y(a) SCREEN_STRETCH_Y(a)
#define SCREEN_SCALE_FROM_RIGHT(a) (SCREEN_WIDTH - SCREEN_SCALE_X(a))
#define SCREEN_SCALE_FROM_BOTTOM(a) (SCREEN_HEIGHT - SCREEN_SCALE_Y(a))

#define ASPECT_RATIO_SCALE
#ifdef ASPECT_RATIO_SCALE
#define SCREEN_SCALE_AR(a) ((a) * DEFAULT_ASPECT_RATIO / SCREEN_ASPECT_RATIO)
#else
#define SCREEN_SCALE_AR(a) (a)
#endif

extern int gDefaultTaskTime;

extern char *gString; // char gString[200]

extern float &GAME_GRAVITY; // default 0.0080000004

extern char(&PC_Scratch)[16384];

void InjectCommonHooks();
                            // returns player coors
CVector FindPlayerCoors(int playerId);
// returns player speed
CVector const& FindPlayerSpeed(int playerId);
// returns player ped or player vehicle if he's driving
CEntity * FindPlayerEntity(int playerId);
// gets player coords
CVector const& FindPlayerCentreOfWorld(int playerId);
// gets player coords with skipping sniper shift
CVector const& FindPlayerCentreOfWorld_NoSniperShift(int playerId);
// returns player coords with skipping interior shift
CVector FindPlayerCentreOfWorld_NoInteriorShift(int playerId);
// returns player angle in radians
float FindPlayerHeading(int playerId);
// returns Z coord for active player
float FindPlayerHeight();
// returns player ped
CPlayerPed * FindPlayerPed(int playerId = -1);
// returns player vehicle
CAutomobile * FindPlayerVehicle(int playerId, bool bIncludeRemote);
// 2 players are playing
bool InTwoPlayersMode();

// matrix mul
CVector* Multiply3x3(CVector* out, CMatrix* m, CVector* in);
CVector Multiply3x3(CMatrix* matrix, CVector* vector);
// vector by matrix mult, resulting in a vector where each component is the dot product of the in vector and a matrix direction
CVector Multiply3x3(CVector* vector, CMatrix* matrix);


// returns player wanted
CWanted * FindPlayerWanted(int playerId = -1);

const unsigned int rwVENDORID_ROCKSTAR = 0x0253F2;
extern unsigned int &ClumpOffset;

#define RpClumpGetAnimBlendClumpData(clump) (*(CAnimBlendClumpData **)(((unsigned int)(clump) + ClumpOffset)))

constexpr float TWO_PI = 6.28318530718f;
constexpr float PI = 3.14159265358979323846f;
constexpr float PI_2 = PI / 2.0f;

constexpr float DegreesToRadians(float angleInDegrees) {
    return angleInDegrees * PI / 180.0f;
}

template <typename T>
T clamp(T value, T low, T high)
{
    return std::min(std::max(value, low), high);
}


inline const float lerp(float fMin, float fMax, float fProgress) {
    return fMin * (1.0F - fProgress) + fMax * fProgress;
}

inline const float invLerp(float fMin, float fMax, float fVal) {
    return (fVal - fMin) / (fMax - fMin);
}

AnimBlendFrameData *RpAnimBlendClumpFindFrame(RpClump *clump, char *name);

char *MakeUpperCase(char *dest, char *src);

class CEventGroup* GetEventGlobalGroup();
// dummy function
void CreateDebugFont();
// dummy function
void DestroyDebugFont();
// dummy function
void ObrsPrintfString(char const* arg0, short arg1, short arg2);
// dummy function
void FlushObrsPrintfs();
void DefinedState();
void DefinedState2d();
RpAtomic* GetFirstAtomicCallback(RpAtomic* atomic, void* data);
RpAtomic* GetFirstAtomic(RpClump* clump);
RpAtomic* Get2DEffectAtomicCallback(RpAtomic* atomic, void* data);
RpAtomic* Get2DEffectAtomic(RpClump* clump);
RwObject* GetFirstObjectCallback(RwObject* object, void* data);
RwObject* GetFirstObject(RwFrame* frame);
RwFrame* GetFirstFrameCallback(RwFrame* frame, void* data);
RwFrame* GetFirstChild(RwFrame* frame);
RwTexture* GetFirstTextureCallback(RwTexture* texture, void* data);
RwTexture* GetFirstTexture(RwTexDictionary* txd);
RpHAnimHierarchy* GetAnimHierarchyFromSkinClump(RpClump* clump);
RpHAnimHierarchy* GetAnimHierarchyFromFrame(RwFrame* frame);
RpHAnimHierarchy* GetAnimHierarchyFromClump(RpClump* clump);
RpAtomic* AtomicRemoveAnimFromSkinCB(RpAtomic* atomic, void* data);
bool RpAtomicConvertGeometryToTL(RpAtomic* atomic);
bool RpAtomicConvertGeometryToTS(RpAtomic* atomic);
bool RpClumpConvertGeometryToTL(RpClump* clump);
bool RpClumpConvertGeometryToTS(RpClump* clump);
RpMaterial* forceLinearFilteringMatTexturesCB(RpMaterial* material, void* data);
bool SetFilterModeOnAtomicsTextures(RpAtomic* atomic, RwTextureFilterMode filtering);
RpAtomic* forceLinearFilteringAtomicsCB(RpAtomic* atomic, void* data);
bool SetFilterModeOnClumpsTextures(RpClump* clump, RwTextureFilterMode filtering);
bool RpGeometryReplaceOldMaterialWithNewMaterial(RpGeometry* geometry, RpMaterial* oldMaterial, RpMaterial* newMaterial);
RwTexture* RwTexDictionaryFindHashNamedTexture(RwTexDictionary* txd, unsigned int hash);
RpClump* RpClumpGetBoundingSphere(RpClump* clump, RwSphere* bound, bool arg2);
void SkinGetBonePositions(RpClump* clump);
void SkinSetBonePositions(RpClump* clump);
void SkinGetBonePositionsToTable(RpClump* clump, RwV3d* table);
void SetLightsWithTimeOfDayColour(RpWorld* world);
// dummy function
void LightsEnable(int arg0);
RpWorld* LightsDestroy(RpWorld* world);
// lighting = [0.0f;1.0f]
void WorldReplaceNormalLightsWithScorched(RpWorld* world, float lighting);
void WorldReplaceScorchedLightsWithNormal(RpWorld* world);
void AddAnExtraDirectionalLight(RpWorld* world, float x, float y, float z, float red, float green, float blue);
void RemoveExtraDirectionalLights(RpWorld* world);
// lighting = [0.0f;1.0f]
void SetAmbientAndDirectionalColours(float lighting);
// lighting = [0.0f;1.0f]
void SetFlashyColours(float lighting);
// lighting = [0.0f;1.0f]
void SetFlashyColours_Mild(float lighting);
// lighting = [0.0f;1.0f], unused
void SetBrightMarkerColours(float lighting);
void ReSetAmbientAndDirectionalColours();
void DeActivateDirectional();
void ActivateDirectional();
void SetAmbientColoursToIndicateRoadGroup(int arg0);
void SetFullAmbient();
void SetAmbientColours();
void SetAmbientColours(RwRGBAReal* color);
void SetDirectionalColours(RwRGBAReal* color);
// lighting = [0.0f;1.0f]
void SetLightColoursForPedsCarsAndObjects(float lighting);
void SetLightsForInfraredVisionHeatObjects();
void StoreAndSetLightsForInfraredVisionHeatObjects();
void RestoreLightsForInfraredVisionHeatObjects();
void SetLightsForInfraredVisionDefaultObjects();
void SetLightsForNightVision();

// 'data' is unused
RpAtomic* RemoveRefsCB(RpAtomic* atomic, void* _IGNORED_ data);
void RemoveRefsForAtomic(RpClump* clump);

bool IsGlassModel(CEntity* pEntity);

CAnimBlendClumpData* RpAnimBlendAllocateData(RpClump* clump);
CAnimBlendAssociation* RpAnimBlendClumpAddAssociation(RpClump* clump, CAnimBlendAssociation* association, unsigned int flags, float startTime, float blendAmount);
CAnimBlendAssociation* RpAnimBlendClumpExtractAssociations(RpClump* clump);
void RpAnimBlendClumpFillFrameArray(RpClump* clump, AnimBlendFrameData** frameData);
AnimBlendFrameData* RpAnimBlendClumpFindBone(RpClump* clump, unsigned int id);
AnimBlendFrameData* RpAnimBlendClumpFindFrame(RpClump* clump, char const* name);
AnimBlendFrameData* RpAnimBlendClumpFindFrameFromHashKey(RpClump* clump, unsigned int key);
CAnimBlendAssociation* RpAnimBlendClumpGetAssociation(RpClump* clump, bool arg1, CAnimBlendHierarchy* hierarchy);
CAnimBlendAssociation* RpAnimBlendClumpGetAssociation(RpClump* clump, char const* name);
CAnimBlendAssociation* RpAnimBlendClumpGetAssociation(RpClump* clump, unsigned int animId);
CAnimBlendAssociation* RpAnimBlendClumpGetFirstAssociation(RpClump* clump);
CAnimBlendAssociation* RpAnimBlendClumpGetFirstAssociation(RpClump* clump, unsigned int flags);
CAnimBlendAssociation* RpAnimBlendClumpGetMainAssociation(RpClump* clump, CAnimBlendAssociation** pAssociation, float* blendAmount);
CAnimBlendAssociation* RpAnimBlendClumpGetMainAssociation_N(RpClump* clump, int n);
CAnimBlendAssociation* RpAnimBlendClumpGetMainPartialAssociation(RpClump* clump);
CAnimBlendAssociation* RpAnimBlendClumpGetMainPartialAssociation_N(RpClump* clump, int n);
unsigned int RpAnimBlendClumpGetNumAssociations(RpClump* clump);
unsigned int RpAnimBlendClumpGetNumNonPartialAssociations(RpClump* clump);
unsigned int RpAnimBlendClumpGetNumPartialAssociations(RpClump* clump);
void RpAnimBlendClumpGiveAssociations(RpClump* clump, CAnimBlendAssociation* association);
void RpAnimBlendClumpInit(RpClump* clump);
bool RpAnimBlendClumpIsInitialized(RpClump* clump);
void RpAnimBlendClumpPauseAllAnimations(RpClump* clump);
void RpAnimBlendClumpRemoveAllAssociations(RpClump* clump);
void RpAnimBlendClumpRemoveAssociations(RpClump* clump, unsigned int flags);
void RpAnimBlendClumpSetBlendDeltas(RpClump* clump, unsigned int flags, float delta);
void RpAnimBlendClumpUnPauseAllAnimations(RpClump* clump);
void RpAnimBlendClumpUpdateAnimations(RpClump* clump, float step, bool onScreen);
RtAnimAnimation* RpAnimBlendCreateAnimationForHierarchy(RpHAnimHierarchy* hierarchy);
char* RpAnimBlendFrameGetName(RwFrame* frame);
void RpAnimBlendFrameSetName(RwFrame* frame, char* name);
CAnimBlendAssociation* RpAnimBlendGetNextAssociation(CAnimBlendAssociation* association);
CAnimBlendAssociation* RpAnimBlendGetNextAssociation(CAnimBlendAssociation* association, unsigned int flags);
void RpAnimBlendKeyFrameInterpolate(void* voidOut, void* voidIn1, void* voidIn2, float time, void* customData);
bool RpAnimBlendPluginAttach();

void AsciiToGxtChar(char const *src, char *dst);

/**
* Writes given raster to PNG file using RtPNGImageWrite
*/
void Render2dStuff();
void WriteRaster(RwRaster * pRaster, char const * pszPath);

/* Convert UTF-8 string to Windows Unicode. Free pointer using delete[] */
std::wstring UTF8ToUnicode(const std::string &str);
/* Convert Windows Unicode to UTF-8. Free pointer using delete[] */
std::string UnicodeToUTF8(const std::wstring &str);

extern int WindowsCharset;

extern unsigned short &uiTempBufferIndicesStored;
extern unsigned short &uiTempBufferVerticesStored;
constexpr int32_t TOTAL_TEMP_BUFFER_INDICES = 4096;
extern RxVertexIndex(&aTempBufferIndices)[TOTAL_TEMP_BUFFER_INDICES]; // size 4096
constexpr int32_t TOTAL_TEMP_BUFFER_VERTICES = 1024;
extern RxObjSpace3DVertex(&aTempBufferVertices)[TOTAL_TEMP_BUFFER_VERTICES]; // size 1024 - after this there are 2 more arrays like this, both sized 512
