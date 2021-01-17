/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once
#include "PluginBase.h"
#include "RenderWare.h"
#include "CColModel.h"
#include "C2dEffect.h"
#include "eModelID.h"

enum RwModelInfoType
{
    RWMODEL_INFO_ATOMIC = 1,
    RWMODEL_INFO_CLUMP = 2
};

enum ModelInfoType : unsigned char
{
	MODEL_INFO_ATOMIC = 1,
	MODEL_INFO_TIME = 3,
	MODEL_INFO_WEAPON = 4,
	MODEL_INFO_CLUMP = 5,
	MODEL_INFO_VEHICLE = 6,
	MODEL_INFO_PED = 7,
	MODEL_INFO_LOD = 8
};

enum eModelInfoSpecialType : unsigned char {
    SWAY_IN_WIND_1 = 1,
    SWAY_IN_WIND_2 = 2,
    GLASS_TYPE_1 = 4,
    GLASS_TYPE_2 = 5,
    TAG = 6,
    CRANE = 9,
    BREAKABLE_STATUE = 11,
};

struct tTimeInfo
{
	unsigned char m_nTimeOn;
	unsigned char m_nTimeOff;
	signed short  m_wOtherTimeModel;
};

VALIDATE_SIZE(tTimeInfo, 4);

class CVehicleModelInfo;

// originally an abstract class
class  CBaseModelInfo {
public:
    CBaseModelInfo();  // constructor
public:
	unsigned int   m_nKey;
	unsigned short m_nRefCount;
	short          m_nTxdIndex;
	unsigned char  m_nAlpha; 
	unsigned char  m_n2dfxCount;
    short          m_n2dEffectIndex;
    short          m_nObjectInfoIndex;
	union{
        unsigned short m_nFlags;
        struct {
            unsigned char m_nFlagsUpperByte;
            unsigned char m_nFlagsLowerByte;
        };
		struct{
			/* https://code.google.com/p/mtasa-blue/source/browse/tags/1.3.4/MTA10/game_sa/CModelInfoSA.h */
			unsigned char bHasBeenPreRendered : 1; // we use this because we need to apply changes only once
			unsigned char bDrawLast: 1;
			unsigned char bAdditiveRender: 1;
			unsigned char bDontWriteZBuffer: 1;
			unsigned char bDontCastShadowsOn : 1;
			unsigned char bDoWeOwnTheColModel : 1;
			unsigned char bIsBackfaceCulled : 1;
			unsigned char bIsLod : 1;

			union{
				struct{
					unsigned char bIsRoad: 1;
					unsigned char bHasComplexHierarchy : 1;
					unsigned char bDontCollideWithFlyer : 1;
					unsigned char nSpecialType : 4;
					unsigned char bWetRoadReflection : 1;
				};
				struct{
					unsigned char : 2;
					unsigned char nCarmodId : 5;
					unsigned char : 1; // bUseCommonVehicleDictionary
				};
                struct {
                    unsigned char : 7;
                    unsigned char bTagDisabled : 1;
                };
			};
			
		};
	};
	CColModel        *m_pColModel; // 20
	float             m_fDrawDistance; // 24
    union {
        struct RwObject* m_pRwObject;
        struct RpClump* m_pRwClump;
        struct RpAtomic* m_pRwAtomic;
    };

public:
    static void InjectHooks();

	// vtable
    virtual ~CBaseModelInfo() { assert(0); }
	virtual class CAtomicModelInfo *AsAtomicModelInfoPtr();
    virtual class CDamagableModelInfo *AsDamageAtomicModelInfoPtr();
    virtual class CLodAtomicModelInfo*AsLodAtomicModelInfoPtr();
    virtual ModelInfoType GetModelType() = 0;
    virtual tTimeInfo *GetTimeInfo();
    virtual void Init();
    virtual void Shutdown();
    virtual void DeleteRwObject() = 0;
    virtual unsigned int GetRwModelType() = 0;
    virtual struct RwObject *CreateInstance() = 0;
    virtual struct RwObject *CreateInstance(RwMatrix *matrix) = 0;
    virtual void SetAnimFile(char const* filename);
    virtual void ConvertAnimFileIndex();
    virtual signed int GetAnimFileIndex();

    // vtable methods implementations
    class CAtomicModelInfo* AsAtomicModelInfoPtr_Reversed();
    class CDamagableModelInfo* AsDamageAtomicModelInfoPtr_Reversed();
    class CLodAtomicModelInfo* AsLodAtomicModelInfoPtr_Reversed();
    tTimeInfo* GetTimeInfo_Reversed();
    void Init_Reversed();
    void Shutdown_Reversed();
    void SetAnimFile_Reversed(char const* filename);
    void ConvertAnimFileIndex_Reversed();
    signed int GetAnimFileIndex_Reversed();

	//
	void SetTexDictionary(char const* txdName);
	void ClearTexDictionary();
	void AddTexDictionaryRef();
	void RemoveTexDictionaryRef();
	void AddRef();
	void RemoveRef();
	// initPairedModel defines if we need to set col model for time model
	void SetColModel(CColModel *colModel, bool bIsLodModel);
	void Init2dEffects();
	void DeleteCollisionModel();
	// index is a number of effect (max number is (m_n2dfxCount - 1))
	C2dEffect *Get2dEffect(int index);
	void Add2dEffect(C2dEffect *effect);

    // Those further ones are completely inlined in final version, not present at all in android version;
    inline CVehicleModelInfo* AsVehicleModelInfoPtr() { return reinterpret_cast<CVehicleModelInfo*>(this); }
    inline CColModel* GetColModel() { return m_pColModel; }

    inline bool GetIsDrawLast() { return bDrawLast; }
    inline bool HasBeenPreRendered() { return bHasBeenPreRendered; }
    inline bool HasComplexHierarchy() { return bHasComplexHierarchy; }
    inline bool IsBackfaceCulled() { return bIsBackfaceCulled; }
    inline bool IsLod() { return bIsLod; }
    inline bool IsRoad() { return bIsRoad; }
    inline void SetHasBeenPreRendered(int bPreRendered) { bHasBeenPreRendered = bPreRendered; }
    inline void SetIsLod(int bLod) { bIsLod = bLod; }
    inline void SetOwnsColModel(int bOwns) { bDoWeOwnTheColModel = bOwns; }
    inline void IncreaseAlpha() {
        if (m_nAlpha >= 239)
            m_nAlpha = 255;
        else
            m_nAlpha += 16;
    };

    inline bool IsSwayInWind1() { return nSpecialType == eModelInfoSpecialType::SWAY_IN_WIND_1; }      //0x0800
    inline bool IsSwayInWind2() { return nSpecialType == eModelInfoSpecialType::SWAY_IN_WIND_2; }      //0x1000
    inline bool SwaysInWind() { return IsSwayInWind1() || IsSwayInWind2(); }
    inline bool IsGlassType1() { return nSpecialType == eModelInfoSpecialType::GLASS_TYPE_1; }       //0x2000
    inline bool IsGlassType2() { return nSpecialType == eModelInfoSpecialType::GLASS_TYPE_2; }       //0x2800
    inline bool IsGlass() { return IsGlassType1() || IsGlassType2(); }        
	inline bool IsTagModel() { return nSpecialType == eModelInfoSpecialType::TAG; } //0x3000
    inline bool IsBreakableStatuePart() { return nSpecialType == eModelInfoSpecialType::BREAKABLE_STATUE; }
    inline bool IsCrane() { return nSpecialType == eModelInfoSpecialType::CRANE; }            //0x4800
};

VALIDATE_SIZE(CBaseModelInfo, 0x20);
