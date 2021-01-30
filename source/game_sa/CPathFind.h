/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "PluginBase.h"
#include "CompressedVector.h"
#include "CVector.h"
#include "CNodeAddress.h"

#define NUM_PATH_MAP_AREAS 64
#define NUM_PATH_INTERIOR_AREAS 8

enum ePathType
{
    PATH_TYPE_CARS = 0,
    PATH_TYPE_BOATS
};

class  CForbiddenArea
{
public:
	float x1, x2, y1, y2, z1, z2;
	bool bEnable;
	unsigned char type;
private:
	char _padding[2];
public:
};

VALIDATE_SIZE(CForbiddenArea, 0x1C);


class  CCarPathLinkAddress
{
public:
    unsigned short m_wCarPathLinkId : 10;
    unsigned short m_wAreaId : 6;

    inline bool IsValid() {
        return *reinterpret_cast<unsigned short*>(this) != 0xFFFF;
    }
};

VALIDATE_SIZE(CCarPathLinkAddress, 0x2);


class  CPathIntersectionInfo
{
public:
    unsigned char m_bRoadCross : 1;
    unsigned char m_bPedTrafficLight : 1;
};

VALIDATE_SIZE(CPathIntersectionInfo, 0x1);


class  CCarPathLink
{
public:
    struct {
        short x;
        short y;
    } m_posn;
    CNodeAddress m_address;
    char m_nDirX;
    char m_nDirY;
    char m_nPathNodeWidth;
    unsigned char m_nNumLeftLanes : 3;
    unsigned char m_nNumRightLanes : 3;
    unsigned char m_bTrafficLightDirection : 1; // 1 if the navi node has the same direction as the traffic light and 0 if the navi node points somewhere else
    unsigned char unk1 : 1;

    unsigned short m_nTrafficLightState : 2; // 1 - North-South, 2 - West-East cycle, enum: eTrafficLightsDirection
    unsigned short m_bTrainCrossing : 1;
};

VALIDATE_SIZE(CCarPathLink, 0xE);

class  CPathNode
{
public:
    void *ptr;
	void *ptr2;
    CompressedVector m_posn;
    unsigned short m_wSearchList; // search list id
    short  m_wBaseLinkId;
    short  m_wAreaId;
    short  m_wNodeId;
    unsigned char m_nPathWidth;
    unsigned char m_nFloodFill;

    unsigned int m_nNumLinks : 4;
    unsigned int m_nTrafficLevel : 2; // TrafficLevel uses 4 steps: 0 = full 1 = high 2 = medium 3 = low
    unsigned int m_bRoadBlocks : 1;
    unsigned int m_bWaterNode : 1;

    unsigned int m_bEmergencyVehiclesOnly : 1;
    unsigned int unk1 : 1;   // not used in paths data files
    unsigned int m_bDontWander : 1;
    unsigned int unk2 : 1;   // not used in paths data files
    unsigned int m_bNotHighway : 1;
    unsigned int m_bHighway : 1;
    unsigned int unk3 : 1;	 // not used in paths data files
    unsigned int unk4 : 1;	 // not used in paths data files

    unsigned int m_nSpawnProbability : 4;
    unsigned int m_nBehaviourType : 4; // 1 - roadblock
                                       // 2 - parking node

	CVector GetNodeCoors();
};

VALIDATE_SIZE(CPathNode, 0x1C);


class CPathFind
{
public:
	CNodeAddress info;
	CPathNode *m_apNodesSearchLists[512];
	CPathNode *m_pPathNodes[NUM_PATH_MAP_AREAS + NUM_PATH_INTERIOR_AREAS];
    // Use CPathFind::GetCarPathLink to access
	CCarPathLink *m_pNaviNodes[NUM_PATH_MAP_AREAS + NUM_PATH_INTERIOR_AREAS];
	CNodeAddress *m_pNodeLinks[NUM_PATH_MAP_AREAS + NUM_PATH_INTERIOR_AREAS];
	unsigned char *m_pLinkLengths[NUM_PATH_MAP_AREAS + NUM_PATH_INTERIOR_AREAS];
	CPathIntersectionInfo *m_pPathIntersections[NUM_PATH_MAP_AREAS + NUM_PATH_INTERIOR_AREAS];
	CCarPathLinkAddress *m_pNaviLinks[NUM_PATH_MAP_AREAS];
	void *field_EA4[NUM_PATH_MAP_AREAS];
	unsigned int m_dwNumNodes[NUM_PATH_MAP_AREAS + NUM_PATH_INTERIOR_AREAS];
	unsigned int m_dwNumVehicleNodes[NUM_PATH_MAP_AREAS + NUM_PATH_INTERIOR_AREAS];
	unsigned int m_dwNumPedNodes[NUM_PATH_MAP_AREAS + NUM_PATH_INTERIOR_AREAS];
	unsigned int m_dwNumCarPathLinks[NUM_PATH_MAP_AREAS + NUM_PATH_INTERIOR_AREAS];
	unsigned int m_dwNumAddresses[NUM_PATH_MAP_AREAS + NUM_PATH_INTERIOR_AREAS];
	int field_1544[2048];
	unsigned int m_dwTotalNumNodesInSearchList;
	CNodeAddress char3548[8];
	unsigned int m_dwNumForbiddenAreas;
	CForbiddenArea m_aForbiddenAreas[64];
	bool m_bForbiddenForScriptedCarsEnabled;
	char _padding[3];
	float m_fForbiddenForScrCarsX1;
	float m_fForbiddenForScrCarsX2;
	float m_fForbiddenForScrCarsY1;
	float m_fForbiddenForScrCarsY2;

    void Init();
    bool TestCrossesRoad(CNodeAddress startNodeAddress, CNodeAddress targetNodeAddress);
    bool TestForPedTrafficLight(CNodeAddress startNodeAddress, CNodeAddress targetNodeAddress);
    CVector* TakeWidthIntoAccountForWandering(CVector* outPosition, CNodeAddress nodeAddress, unsigned short randomSeed);
    void FindNextNodeWandering(int pathType, float x, float y, float z, CNodeAddress* startNodeAddress, CNodeAddress* targetNodeAddress, unsigned int dir, int8_t* outDir);
	void DoPathSearch(unsigned char pathType, CVector origin, CNodeAddress originAddr, 
		CVector target, CNodeAddress *pResultNodes, short *pNodesCount, int maxNodesToFind, float *pDistance, 
		float maxSearchDistance, CNodeAddress *targetAddr, float maxUnkLimit, bool oneSideOnly, 
		CNodeAddress forbiddenNodeAddr, bool includeNodesWithoutLinks, bool waterPath);

    void SetLinksBridgeLights(float fXMin, float fXMax, float fYMin, float fYMax, bool bTrainCrossing);

	CPathNode *GetPathNode(CNodeAddress address);
    int LoadPathFindData(RwStream *stream, int index);
    void UnLoadPathFindData(int index);
    std::int32_t LoadSceneForPathNodes(CVector point);
    bool IsWaterNodeNearby(CVector position, float radius);
    static void AllocatePathFindInfoMem() {} // yes, it's empty

    // pathLink is the same as the returned pointer(at least on success)
    // pathLink should be a pointer to CNodeAddress on the stack or somewhere, and
    // the result is stored in it
    CNodeAddress* FindNodeClosestToCoors(CNodeAddress* pathLink, float X, float Y, float Z, int _nodeType, float maxDistance,
        unsigned short unk2, int unk3, unsigned short unk4, unsigned short bBoatsOnly, int unk6);

    inline CCarPathLink& GetCarPathLink(CCarPathLinkAddress const& address) { return m_pNaviNodes[address.m_wAreaId][address.m_wCarPathLinkId]; }
};

VALIDATE_SIZE(CPathFind, 0x3C80);

extern  CPathFind& ThePaths;
