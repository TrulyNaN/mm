#include "global.h"
#include "vt.h"

#define DYNA_RAYCAST_FLOORS 1
#define DYNA_RAYCAST_WALLS 2
#define DYNA_RAYCAST_CEILINGS 4

u32 sWallFlags[32] = {
    0, 1, 3, 5, 8, 16, 32, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

u16 sSurfaceTypeSfx[] = {
    /* 0x00 */ NA_SE_PL_WALK_GROUND - SFX_FLAG,
    /* 0x01 */ NA_SE_PL_WALK_SAND - SFX_FLAG,
    /* 0x02 */ NA_SE_PL_WALK_CONCRETE - SFX_FLAG,
    /* 0x03 */ NA_SE_PL_WALK_DIRT - SFX_FLAG,
    /* 0x04 */ NA_SE_PL_WALK_WATER0 - SFX_FLAG,
    /* 0x05 */ NA_SE_PL_WALK_WATER1 - SFX_FLAG,
    /* 0x06 */ NA_SE_PL_WALK_WATER2 - SFX_FLAG,
    /* 0x07 */ NA_SE_PL_WALK_MAGMA - SFX_FLAG,
    /* 0x08 */ NA_SE_PL_WALK_GRASS - SFX_FLAG,
    /* 0x09 */ NA_SE_PL_WALK_GLASS - SFX_FLAG,
    /* 0x0A */ NA_SE_PL_WALK_LADDER - SFX_FLAG,
    /* 0x0B */ NA_SE_PL_WALK_GROUND - SFX_FLAG,
    /* 0x0C */ NA_SE_PL_WALK_ICE - SFX_FLAG,
    /* 0x0D */ NA_SE_PL_WALK_IRON - SFX_FLAG,
    /* 0x0E */ NA_SE_PL_WALK_SNOW - SFX_FLAG
};

u8 D_801B46C0[] = {
    /* 0x00 */ 1,
    /* 0x01 */ 1,
    /* 0x02 */ 0,
    /* 0x03 */ 1,
    /* 0x04 */ 0,
    /* 0x05 */ 0,
    /* 0x06 */ 0,
    /* 0x07 */ 0,
    /* 0x08 */ 0,
    /* 0x09 */ 0,
    /* 0x0A */ 0,
    /* 0x0B */ 0,
    /* 0x0C */ 0,
    /* 0x0D */ 0,
    /* 0x0E */ 1
};

s16 sSmallMemScenes[] = {
    SCENE_F01,
};

typedef struct {
    s16 sceneId;
    u32 memSize;
} BgCheckSceneMemEntry;

BgCheckSceneMemEntry sSceneMemList[] = {
    { SCENE_00KEIKOKU, 0xC800 },
};

BgCheckSceneSubdivisionEntry sSceneSubdivisionList[] = {
    { SCENE_00KEIKOKU, { 36, 1, 36 }, -1 },
    { SCENE_30GYOSON, { 40, 1, 40 }, -1 },
    { SCENE_31MISAKI, { 40, 1, 40 }, -1 },
};

BgSpecialSceneMaxObjects sCustomDynapolyMem[] = {
    { SCENE_21MITURINMAE, 1000, 600, 512 },
};

// TODO: All these bss variables are localized to one function and can
// likely be made into in-function static bss variables in the future
char D_801ED950[80];
char D_801ED9A0[80];
Vec3f D_801ED9F0[3]; // polyVerts
Vec3f D_801EDA18[3]; // polyVerts
MtxF D_801EDA40;
Vec3f D_801EDA80[3]; // polyVerts
char D_801EDAA8[80];
char D_801EDAF8[80];
Vec3f D_801EDB48[3]; // polyVerts
Vec3f D_801EDB70[3];
Plane D_801EDB98;

void BgCheck_GetStaticLookupIndicesFromPos(CollisionContext* colCtx, Vec3f* pos, Vec3i* sector);
f32 BgCheck_RaycastFloorDyna(DynaRaycast* dynaRaycast);
s32 BgCheck_SphVsDynaWall(CollisionContext* colCtx, u16 xpFlags, f32* outX, f32* outZ, Vec3f* pos, f32 radius,
                          CollisionPoly** outPoly, s32* outBgId, Actor* actor);
s32 BgCheck_CheckLineImpl(CollisionContext* colCtx, u16 xpFlags1, u16 xpFlags2, Vec3f* posA, Vec3f* posB,
                          Vec3f* posResult, CollisionPoly** outPoly, s32* outBgId, Actor* actor, f32 checkDist,
                          u32 bccFlags);
s32 BgCheck_CheckDynaCeiling(CollisionContext* colCtx, u16 xpFlags, f32* outY, Vec3f* pos, f32 checkDist,
                             CollisionPoly** outPoly, s32* outBgId, Actor* actor);
s32 BgCheck_CheckLineAgainstDyna(CollisionContext* colCtx, u16 xpFlags, Vec3f* posA, Vec3f* posB, Vec3f* posResult,
                                 CollisionPoly** outPoly, f32* distSq, s32* outBgId, Actor* actor, f32 checkDist,
                                 s32 bccFlags);
s32 BgCheck_SphVsFirstDynaPoly(CollisionContext* colCtx, u16 xpFlags, CollisionPoly** outPoly, s32* outBgId,
                               Vec3f* center, f32 radius, Actor* actor, u16 bciFlags);
void BgCheck_ResetPolyCheckTbl(SSNodeList* nodeList, s32 numPolys);
s32 BgCheck_PosInStaticBoundingBox(CollisionContext* colCtx, Vec3f* pos);

void SSNode_SetValue(SSNode* node, s16* polyIndex, u16 next) {
    node->polyId = *polyIndex;
    node->next = next;
}

void BgCheck_PolygonLinkedListResetHead(u16* head) {
    *head = 0xFFFF;
}

void BgCheck_ScenePolygonListsNodeInsert(SSNodeList* list, u16* head, s16* polyIndex) {
    u16 index;

    index = BgCheck_ScenePolygonListsReserveNode(list);
    BgCheck_PolygonLinkedListNodeInit(&list->tbl[index], polyIndex, *head);
    *head = index;
}

void BgCheck_PolygonLinkedListNodeInsert(DynaSSNodeList* list, u16* head, s16* polyIndex) {
    u16 index;

    index = BgCheck_AllocPolygonLinkedListNode(list);
    BgCheck_PolygonLinkedListNodeInit(&list->tbl[index], polyIndex, *head);
    *head = index;
}

void BgCheck_PolygonLinkedListInit(GlobalContext* globalCtx, DynaSSNodeList* list) {
    list->tbl = NULL;
    list->count = 0;
}

void BgCheck_PolygonLinkedListAlloc(GlobalContext* globalCtx, DynaSSNodeList* list, u32 numNodes) {
    list->tbl = (SSNode*)THA_AllocEndAlign(&globalCtx->state.heap, numNodes << 2, 0xfffffffe);
    list->max = numNodes;
    list->count = 0;
}

void BgCheck_PolygonLinkedListReset(DynaSSNodeList* list) {
    list->count = 0;
}

u16 BgCheck_AllocPolygonLinkedListNode(DynaSSNodeList* list) {
    u16 index;

    index = list->count++;
    if (list->max <= index) {
        return 0xffff;
    }

    return index;
}

void BgCheck_Vec3sToVec3f(Vec3s* src, Vec3f* dest) {
    dest->x = src->x;
    dest->y = src->y;
    dest->z = src->z;
}

void BgCheck_Vec3fToVec3s(Vec3s* dest, Vec3f* src) {
    dest->x = src->x;
    dest->y = src->y;
    dest->z = src->z;
}

f32 func_800BFD84(CollisionPoly* poly, f32 arg1, f32 arg2) {
    return (COLPOLY_GET_NORMAL(poly->normal.x * arg1 + poly->normal.z * arg2) + poly->dist) /
           COLPOLY_GET_NORMAL(-poly->normal.y);
}

/**
 * Unused
 */
s32 func_800BFDEC(CollisionPoly* polyA, CollisionPoly* polyB, u32* outVtxId0, u32* outVtxId1) {
    s32 vtxIdA[3];
    s32 vtxIdB[3];
    s32 i;
    s32 j;
    s32 count;

    *outVtxId0 = *outVtxId1 = 0;
    for (i = 0; i < ARRAY_COUNT(vtxIdA); i++) {
        vtxIdA[i] = COLPOLY_VTX_INDEX(polyA->vtxData[i]);
        vtxIdB[i] = COLPOLY_VTX_INDEX(polyB->vtxData[i]);
    }

    count = 0;
    for (i = 0; i < 2; i++) {
        for (j = i + 1; j < 3; j++) {
            if (vtxIdA[i] == vtxIdB[j]) {
                if (count == 0) {
                    *outVtxId0 = vtxIdA[i];
                } else if (count == 1) {
                    *outVtxId1 = vtxIdA[i];
                }
                count++;
            }
        }
    }
    return count;
}

s16 CollisionPoly_GetMinY(CollisionPoly* poly, Vec3s* vertices) {
    s16 minY;
    s32 a = COLPOLY_VTX_INDEX(poly->flags_vIA);
    s32 b = COLPOLY_VTX_INDEX(poly->flags_vIB);
    s32 c = poly->vIC;

    minY = vertices[a].y;
    if (vertices[b].y < minY) {
        minY = vertices[b].y;
    }
    if (minY < vertices[c].y) {
        return minY;
    }
    return vertices[c].y;
}

void CollisionPoly_GetNormalF(CollisionPoly* poly, f32* nx, f32* ny, f32* nz) {
    *nx = COLPOLY_GET_NORMAL(poly->normal.x);
    *ny = COLPOLY_GET_NORMAL(poly->normal.y);
    *nz = COLPOLY_GET_NORMAL(poly->normal.z);
}

/**
 * Compute transform matrix mapping +y (up) to the collision poly's normal
 */
void func_800C0094(CollisionPoly* poly, f32 tx, f32 ty, f32 tz, MtxF* dest) {
    f32 nx;
    f32 ny;
    f32 nz;
    s32 pad;
    f32 z_f14;
    f32 phi_f14;
    f32 phi_f12;
    f32 inv_z_f14;

    if (poly == NULL) {
        return;
    }
    CollisionPoly_GetNormalF(poly, &nx, &ny, &nz);

    z_f14 = sqrtf(SQ(ny) + SQ(nz));
    if (!IS_ZERO(z_f14)) {
        inv_z_f14 = 1.0f / z_f14;
        phi_f14 = ny * inv_z_f14;
        phi_f12 = nz * inv_z_f14;
    } else {
        phi_f14 = 1.0f;
        phi_f12 = 0.0f;
    }
    dest->xx = z_f14;
    dest->yx = (-nx) * phi_f14;
    dest->zx = (-nx) * phi_f12;
    dest->xy = nx;
    dest->yy = ny;
    dest->zy = nz;
    dest->xz = 0.0f;
    dest->yz = -phi_f12;
    dest->zz = phi_f14;
    dest->xw = tx;
    dest->yw = ty;
    dest->zw = tz;
    dest->wx = 0.0f;
    dest->wy = 0.0f;
    dest->wz = 0.0f;
    dest->ww = 1.0f;
}

f32 CollisionPoly_GetPointDistanceFromPlane(CollisionPoly* poly, Vec3f* point) {
    return COLPOLY_GET_NORMAL(poly->normal.x * point->x + poly->normal.y * point->y + poly->normal.z * point->z) +
           poly->dist;
}

void CollisionPoly_GetVertices(CollisionPoly* poly, Vec3s* vtxList, Vec3f* dest) {
    BgCheck_Vec3sToVec3f(&vtxList[COLPOLY_VTX_INDEX(poly->flags_vIA)], &dest[0]);
    BgCheck_Vec3sToVec3f(&vtxList[COLPOLY_VTX_INDEX(poly->flags_vIB)], &dest[1]);
    BgCheck_Vec3sToVec3f(&vtxList[poly->vIC], &dest[2]);
}

void CollisionPoly_GetVerticesByBgId(CollisionPoly* poly, s32 bgId, CollisionContext* colCtx, Vec3f* dest) {
    Vec3s* vtxList;

    if (poly == NULL || bgId > BG_ACTOR_MAX || dest == NULL) {
        if (dest != NULL) {
            // @bug: dest[2] x and y are not set to 0
            dest[0].x = dest[0].y = dest[0].z = dest[1].x = dest[1].y = dest[1].z = dest[2].z = 0.0f;
        }
    } else {
        if (bgId == BGCHECK_SCENE) {
            vtxList = colCtx->colHeader->vtxList;
        } else {
            vtxList = colCtx->dyna.vtxList;
        }
        CollisionPoly_GetVertices(poly, vtxList, dest);
    }
}

s32 CollisionPoly_CheckYIntersectApprox1(CollisionPoly* poly, Vec3s* vtxList, f32 x, f32 z, f32* yIntersect,
                                         f32 checkDist) {
    f32 nx;
    f32 ny;
    f32 nz;
    Vec3s* vA;
    Vec3s* vB;
    Vec3s* vC;

    vA = &vtxList[COLPOLY_VTX_INDEX(poly->flags_vIA)];
    Math_Vec3s_ToVec3f(&D_801ED9F0[0], vA);
    vB = &vtxList[COLPOLY_VTX_INDEX(poly->flags_vIB)];
    Math_Vec3s_ToVec3f(&D_801ED9F0[1], vB);
    vC = &vtxList[poly->vIC];
    Math_Vec3s_ToVec3f(&D_801ED9F0[2], vC);

    nx = COLPOLY_GET_NORMAL(poly->normal.x);
    ny = COLPOLY_GET_NORMAL(poly->normal.y);
    nz = COLPOLY_GET_NORMAL(poly->normal.z);

    return Math3D_TriChkPointParaYIntersectDist(&D_801ED9F0[0], &D_801ED9F0[1], &D_801ED9F0[2], nx, ny, nz, poly->dist,
                                                z, x, yIntersect, checkDist);
}

/**
 * Checks if point (`x`,`z`) is within `checkDist` of `poly`, computing `yIntersect` if true
 * Determinant max 0.0f (checks if on or within poly)
 */
s32 CollisionPoly_CheckYIntersect(CollisionPoly* poly, Vec3s* vtxList, f32 x, f32 z, f32* yIntersect, f32 checkDist) {
    Vec3s* sVerts;
    f32 nx;
    f32 ny;
    f32 nz;

    sVerts = &vtxList[COLPOLY_VTX_INDEX(poly->flags_vIA)];
    D_801EDA18[0].x = sVerts->x;
    D_801EDA18[0].y = sVerts->y;
    D_801EDA18[0].z = sVerts->z;
    sVerts = &vtxList[COLPOLY_VTX_INDEX(poly->flags_vIB)];
    D_801EDA18[1].x = sVerts->x;
    D_801EDA18[1].y = sVerts->y;
    D_801EDA18[1].z = sVerts->z;
    sVerts = &vtxList[(s32)poly->vIC];
    D_801EDA18[2].x = sVerts->x;
    D_801EDA18[2].y = sVerts->y;
    D_801EDA18[2].z = sVerts->z;

    if (!func_8017A304(&D_801EDA18[0], &D_801EDA18[1], &D_801EDA18[2], z, x, checkDist)) {
        return 0;
    }
    nx = COLPOLY_GET_NORMAL(poly->normal.x);
    ny = COLPOLY_GET_NORMAL(poly->normal.y);
    nz = COLPOLY_GET_NORMAL(poly->normal.z);
    return Math3D_TriChkPointParaYIntersectInsideTri2(&D_801EDA18[0], &D_801EDA18[1], &D_801EDA18[2], nx, ny, nz,
                                                      poly->dist, z, x, yIntersect, checkDist);
}

s32 CollisionPoly_CheckYIntersectApprox2(CollisionPoly* poly, Vec3s* vtxList, f32 x, f32 z, f32* yIntersect) {
    return CollisionPoly_CheckYIntersectApprox1(poly, vtxList, x, z, yIntersect, 1.0f);
}

s32 CollisionPoly_CheckXIntersectApprox(CollisionPoly* poly, Vec3s* vtxList, f32 y, f32 z, f32* xIntersect) {
    f32 nx;
    f32 ny;
    f32 nz;

    CollisionPoly_GetVertices(poly, vtxList, D_801EDA80);
    CollisionPoly_GetNormalF(poly, &nx, &ny, &nz);
    return Math3D_TriChkPointParaXIntersect(&D_801EDA80[0], &D_801EDA80[1], &D_801EDA80[2], nx, ny, nz, poly->dist, y,
                                            z, xIntersect);
}

s32 CollisionPoly_CheckZIntersectApprox(CollisionPoly* poly, Vec3s* vtxList, f32 x, f32 y, f32* zIntersect) {
    f32 nx;
    f32 ny;
    f32 nz;

    CollisionPoly_GetVertices(poly, vtxList, D_801EDB48);
    CollisionPoly_GetNormalF(poly, &nx, &ny, &nz);
    return Math3D_TriChkPointParaZIntersect(&D_801EDB48[0], &D_801EDB48[1], &D_801EDB48[2], nx, ny, nz, poly->dist, x,
                                            y, zIntersect);
}

#ifdef NON_MATCHING
s32 CollisionPoly_LineVsPoly(BgLineVsPolyTest* a0) {
    static Vec3f polyVerts[3]; // D_801EDB70
    static Plane plane;        // D_801EDB98
    f32 planeDistA;
    f32 planeDistB;
    f32 planeDistDelta;

    plane.originDist = a0->poly->dist;
    planeDistA = COLPOLY_GET_NORMAL(a0->poly->normal.x * a0->posA->x + a0->poly->normal.y * a0->posA->y +
                                    a0->poly->normal.z * a0->posA->z) +
                 plane.originDist;
    planeDistB = COLPOLY_GET_NORMAL(a0->poly->normal.x * a0->posB->x + a0->poly->normal.y * a0->posB->y +
                                    a0->poly->normal.z * a0->posB->z) +
                 plane.originDist;

    planeDistDelta = planeDistA - planeDistB;
    if ((planeDistA >= 0.0f && planeDistB >= 0.0f) || (planeDistA < 0.0f && planeDistB < 0.0f) ||
        ((a0->checkOneFace != 0) && (planeDistA < 0.0f && planeDistB > 0.0f)) || IS_ZERO(planeDistDelta)) {
        return false;
    }

    CollisionPoly_GetNormalF(a0->poly, &plane.normal.x, &plane.normal.y, &plane.normal.z);
    CollisionPoly_GetVertices(a0->poly, a0->vtxList, polyVerts);
    Math3D_Lerp(a0->posA, a0->posB, planeDistA / planeDistDelta, a0->planeIntersect);

    if ((fabsf(plane.normal.x) > 0.5f &&
         Math3D_TriChkPointParaXDist(&polyVerts[0], &polyVerts[1], &polyVerts[2], a0->planeIntersect->y,
                                     a0->planeIntersect->z, 0.0f, a0->checkDist, plane.normal.x)) ||
        (fabsf(plane.normal.y) > 0.5f &&
         Math3D_TriChkPointParaYDist(&polyVerts[0], &polyVerts[1], &polyVerts[2], a0->planeIntersect->z,
                                     a0->planeIntersect->x, 0.0f, a0->checkDist, plane.normal.y)) ||
        (fabsf(plane.normal.z) > 0.5f &&
         Math3D_TriChkLineSegParaZDist(&polyVerts[0], &polyVerts[1], &polyVerts[2], a0->planeIntersect->x,
                                       a0->planeIntersect->y, 0.0f, a0->checkDist, plane.normal.z))) {
        return true;
    }
    return false;
}
#else
s32 CollisionPoly_LineVsPoly(BgLineVsPolyTest* a0);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_bgcheck/CollisionPoly_LineVsPoly.s")
#endif

s32 CollisionPoly_SphVsPoly(CollisionPoly* poly, Vec3s* vtxList, Vec3f* center, f32 radius) {
    static Sphere16 sphere; // D_801EDBA8
    static TriNorm tri;     // D_801EDBB0
    Vec3f intersect;

    CollisionPoly_GetVertices(poly, vtxList, tri.vtx);
    CollisionPoly_GetNormalF(poly, &tri.plane.normal.x, &tri.plane.normal.y, &tri.plane.normal.z);
    tri.plane.originDist = poly->dist;
    sphere.center.x = center->x;
    sphere.center.y = center->y;
    sphere.center.z = center->z;
    sphere.radius = radius;
    return Math3D_ColSphereTri(&sphere, &tri, &intersect);
}

/**
 * Add poly to StaticLookup table
 * Table is sorted by poly's smallest y vertex component
 * `ssList` is the list to append a new poly to
 * `polyList` is the CollisionPoly lookup list
 * `vtxList` is the vertex lookup list
 * `polyId` is the index of the poly in polyList to insert into the lookup table
 */
void StaticLookup_AddPolyToSSList(CollisionContext* colCtx, SSList* ssList, CollisionPoly* polyList, Vec3s* vtxList,
                                  s16 polyId) {
    SSNode* curNode;
    SSNode* nextNode;
    s32 polyYMin;
    u16 newNodeId;
    s16 curPolyId;

    // if list is null
    if (ssList->head == SS_NULL) {
        SSNodeList_SetSSListHead(&colCtx->polyNodes, ssList, &polyId);
        return;
    }

    polyYMin = CollisionPoly_GetMinY(&polyList[polyId], vtxList);

    curNode = &colCtx->polyNodes.tbl[ssList->head];
    curPolyId = curNode->polyId;

    // if the poly being inserted has a lower y than the first poly
    if (polyYMin < vtxList[COLPOLY_VTX_INDEX(polyList[curPolyId].flags_vIA)].y &&
        polyYMin < vtxList[COLPOLY_VTX_INDEX(polyList[curPolyId].flags_vIB)].y &&
        polyYMin < vtxList[polyList[curPolyId].vIC].y) {
        SSNodeList_SetSSListHead(&colCtx->polyNodes, ssList, &polyId);
        return;
    }
    while (true) {
        // if at the end of the list
        if (curNode->next == SS_NULL) {
            s32 pad;

            newNodeId = SSNodeList_GetNextNodeIdx(&colCtx->polyNodes);
            SSNode_SetValue(&colCtx->polyNodes.tbl[newNodeId], &polyId, SS_NULL);
            curNode->next = newNodeId;
            break;
        }

        nextNode = &colCtx->polyNodes.tbl[curNode->next];
        curPolyId = nextNode->polyId;

        // if the poly being inserted is lower than the next poly
        if (polyYMin < vtxList[COLPOLY_VTX_INDEX(polyList[curPolyId].flags_vIA)].y &&
            polyYMin < vtxList[COLPOLY_VTX_INDEX(polyList[curPolyId].flags_vIB)].y &&
            polyYMin < vtxList[polyList[curPolyId].vIC].y) {
            newNodeId = SSNodeList_GetNextNodeIdx(&colCtx->polyNodes);
            SSNode_SetValue(&colCtx->polyNodes.tbl[newNodeId], &polyId, curNode->next);
            curNode->next = newNodeId;
            break;
        }
        curNode = nextNode;
    }
}

/**
 * Add CollisionPoly to StaticLookup list
 */
void StaticLookup_AddPoly(StaticLookup* lookup, CollisionContext* colCtx, CollisionPoly* polyList, Vec3s* vtxList,
                          s16 index) {
    if (polyList[index].normal.y > COLPOLY_SNORMAL(0.5f)) {
        StaticLookup_AddPolyToSSList(colCtx, &lookup->floor, polyList, vtxList, index);
    } else if (polyList[index].normal.y < COLPOLY_SNORMAL(-0.8f)) {
        StaticLookup_AddPolyToSSList(colCtx, &lookup->ceiling, polyList, vtxList, index);
    } else {
        StaticLookup_AddPolyToSSList(colCtx, &lookup->wall, polyList, vtxList, index);
    }
}

/**
 * Locates the closest static poly directly underneath `pos`, starting at list `ssList`
 * returns yIntersect of the closest poly, or `yIntersectMin`
 * stores the pointer of the closest poly to `outPoly`
 * if (flags & 1), ignore polys with a normal.y < 0 (from vertical walls to ceilings)
 */
f32 BgCheck_RaycastFloorStaticList(CollisionContext* colCtx, u16 xpFlags, SSList* ssList, CollisionPoly** outPoly,
                                   Vec3f* pos, f32 yIntersectMin, f32 checkDist, s32 flags, Actor* actor, s32 arg9) {
    SSNode* curNode;
    s32 polyId;
    f32 result;
    f32 yIntersect;
    CollisionPoly* colPoly;
    s32 pad;

    result = yIntersectMin;
    if (ssList->head == SS_NULL) {
        return result;
    }

    curNode = &colCtx->polyNodes.tbl[ssList->head];

    while (true) {
        polyId = curNode->polyId;
        colPoly = &colCtx->colHeader->polyList[polyId];

        if (((flags & 1) && (colPoly->normal.y < 0)) ||
            ((arg9 & 0x20) && ((colCtx->colHeader->surfaceTypeList[colPoly->type].data[0] >> 0x1E) & 1)) ||
            COLPOLY_VIA_FLAG_TEST(colPoly->flags_vIA, xpFlags) ||
            (COLPOLY_VIA_FLAG_TEST(colPoly->flags_vIB, 4) && ((actor != NULL && actor->category != ACTORCAT_PLAYER) ||
                                                              (actor == NULL && xpFlags != COLPOLY_IGNORE_CAMERA)))) {
            if (curNode->next == SS_NULL) {
                break;
            }
            curNode = &colCtx->polyNodes.tbl[curNode->next];
            continue;
        }

        if (pos->y < colCtx->colHeader->vtxList[COLPOLY_VTX_INDEX(colPoly->flags_vIA)].y &&
            pos->y < colCtx->colHeader->vtxList[COLPOLY_VTX_INDEX(colPoly->flags_vIB)].y &&
            pos->y < colCtx->colHeader->vtxList[colPoly->vIC].y) {
            break;
        }

        if (CollisionPoly_CheckYIntersect(colPoly, colCtx->colHeader->vtxList, pos->x, pos->z, &yIntersect,
                                          checkDist)) {
            // if poly is closer to pos without going over
            if (yIntersect < pos->y && result < yIntersect) {
                result = yIntersect;
                *outPoly = colPoly;
            }
        }

        if (curNode->next == SS_NULL) {
            break;
        }
        curNode = &colCtx->polyNodes.tbl[curNode->next];
    }
    return result;
}

/**
 * Locates the closest static poly directly underneath `pos` within `lookup`.
 * returns yIntersect of the closest poly, or `yIntersectMin`
 * stores the pointer of the closest poly to `outPoly`
 */
f32 BgCheck_RaycastFloorStatic(StaticLookup* lookup, CollisionContext* colCtx, u16 xpFlags, CollisionPoly** poly,
                               Vec3f* pos, u32 arg5, f32 checkDist, f32 yIntersectMin, Actor* actor) {
    s32 flag; // skip polys with normal.y < 0
    f32 yIntersect = yIntersectMin;

    if (arg5 & 4) {
        yIntersect = BgCheck_RaycastFloorStaticList(colCtx, xpFlags, &lookup->floor, poly, pos, yIntersect, checkDist,
                                                    0, actor, arg5);
    }

    if ((arg5 & 2) || (arg5 & 8)) {
        flag = 0;
        if (arg5 & 0x10) {
            flag = 1;
        }
        yIntersect = BgCheck_RaycastFloorStaticList(colCtx, xpFlags, &lookup->wall, poly, pos, yIntersect, checkDist,
                                                    flag, actor, arg5);
    }

    if (arg5 & 1) {
        flag = 0;
        if (arg5 & 0x10) {
            flag = 1;
        }
        yIntersect = BgCheck_RaycastFloorStaticList(colCtx, xpFlags, &lookup->ceiling, poly, pos, yIntersect, checkDist,
                                                    flag, actor, arg5);
    }

    return yIntersect;
}

/**
 * Compute wall displacement on `posX` and `posZ`
 * sets `wallPolyPtr` to `poly` if `wallPolyPtr` is NULL
 * returns true if `wallPolyPtr` was changed
 * `invXZlength` is 1 / sqrt( sq(poly.normal.x) + sq(poly.normal.z) )
 */
s32 BgCheck_ComputeWallDisplacement(CollisionContext* colCtx, CollisionPoly* poly, f32* posX, f32* posZ, f32 nx, f32 ny,
                                    f32 nz, f32 invXZlength, f32 planeDist, f32 radius, CollisionPoly** wallPolyPtr) {
    f32 displacement = (radius - planeDist) * invXZlength;

    *posX += displacement * nx;
    *posZ += displacement * nz;

    if (*wallPolyPtr == NULL) {
        *wallPolyPtr = poly;
        return true;
    } else {
        return false;
    }
}

/**
 * Performs collision detection on static poly walls within `lookup` on sphere `pos`, `radius`
 * returns true if a collision was detected
 * `outX` `outZ` return the displaced x,z coordinates,
 * `outPoly` returns the pointer to the nearest poly collided with, or NULL
 */
s32 BgCheck_SphVsStaticWall(StaticLookup* lookup, CollisionContext* colCtx, u16 xpFlags, f32* outX, f32* outZ,
                            Vec3f* pos, f32 radius, CollisionPoly** outPoly, Actor* actor) {
    Vec3f resultPos;
    f32 zTemp;
    f32 xTemp;
    f32 planeDist;
    f32 intersect;
    s32 result;
    CollisionPoly* curPoly;
    CollisionPoly* polyList;
    SSNode* curNode;
    f32 invNormalXZ;
    Vec3s* vtxA;
    Vec3s* vtxB;
    Vec3s* vtxC;
    s32 polyId;
    f32 normalXZ;
    f32 nx;
    f32 ny;
    f32 nz;
    Vec3s* vtxList;
    u16 pad;
    f32 temp_f16;
    f32 zMin;
    f32 zMax;
    f32 xMin;
    f32 xMax;

    result = false;
    if (lookup->wall.head == SS_NULL) {
        return result;
    }
    resultPos = *pos;

    polyList = colCtx->colHeader->polyList;
    vtxList = colCtx->colHeader->vtxList;
    curNode = &colCtx->polyNodes.tbl[lookup->wall.head];

    while (true) {
        polyId = curNode->polyId;
        curPoly = &polyList[polyId];
        vtxA = &vtxList[COLPOLY_VTX_INDEX(curPoly->flags_vIA)];
        vtxB = &vtxList[COLPOLY_VTX_INDEX(curPoly->flags_vIB)];
        vtxC = &vtxList[curPoly->vIC];

        if (pos->y < vtxA->y && pos->y < vtxB->y && pos->y < vtxC->y) {
            break;
        }

        nx = COLPOLY_GET_NORMAL(curPoly->normal.x);
        ny = COLPOLY_GET_NORMAL(curPoly->normal.y);
        nz = COLPOLY_GET_NORMAL(curPoly->normal.z);
        normalXZ = sqrtf(SQ(nx) + SQ(nz));
        planeDist = Math3D_DistPlaneToPos(nx, ny, nz, curPoly->dist, &resultPos);
        if (radius < fabsf(planeDist) || COLPOLY_VIA_FLAG_TEST(curPoly->flags_vIA, xpFlags) ||
            (COLPOLY_VIA_FLAG_TEST(curPoly->flags_vIB, 4) && ((actor != NULL && actor->category != ACTORCAT_PLAYER) ||
                                                              (actor == NULL && xpFlags != COLPOLY_IGNORE_CAMERA)))) {
            if (curNode->next == SS_NULL) {
                break;
            } else {
                curNode = &colCtx->polyNodes.tbl[curNode->next];
                continue;
            }
        }

        invNormalXZ = 1.0f / normalXZ;
        temp_f16 = fabsf(nz) * invNormalXZ;
        if (temp_f16 < 0.4f) {
            if (curNode->next == SS_NULL) {
                break;
            } else {
                curNode = &colCtx->polyNodes.tbl[curNode->next];
                continue;
            }
        }

        // compute curPoly zMin/zMax
        zTemp = vtxA->z;
        zMax = zMin = zTemp;
        zTemp = vtxB->z;

        if (zTemp < zMin) {
            zMin = zTemp;
        } else if (zMax < zTemp) {
            zMax = zTemp;
        }
        zTemp = vtxC->z;
        if (zTemp < zMin) {
            zMin = zTemp;
        } else if (zTemp > zMax) {
            zMax = zTemp;
        }

        zMin -= radius;
        zMax += radius;

        if (resultPos.z < zMin || resultPos.z > zMax) {
            if (curNode->next == SS_NULL) {
                break;
            } else {
                curNode = &colCtx->polyNodes.tbl[curNode->next];
                continue;
            }
        }
        if (CollisionPoly_CheckZIntersectApprox(curPoly, vtxList, resultPos.x, pos->y, &intersect)) {
            f32 test = intersect - resultPos.z;

            if (fabsf(test) <= radius / temp_f16) {
                if (test * nz <= 4.0f) {
                    if (BgCheck_ComputeWallDisplacement(colCtx, curPoly, &resultPos.x, &resultPos.z, nx, ny, nz,
                                                        invNormalXZ, planeDist, radius, outPoly)) {
                        result = true;
                    }
                }
            }
        }
        if (curNode->next == SS_NULL) {
            break;
        } else {
            curNode = &colCtx->polyNodes.tbl[curNode->next];
        }
    }

    curNode = &colCtx->polyNodes.tbl[lookup->wall.head];

    while (true) {
        polyId = curNode->polyId;
        curPoly = &polyList[polyId];
        vtxA = &vtxList[COLPOLY_VTX_INDEX(curPoly->flags_vIA)];
        vtxB = &vtxList[COLPOLY_VTX_INDEX(curPoly->flags_vIB)];
        vtxC = &vtxList[curPoly->vIC];

        if (pos->y < vtxA->y && pos->y < vtxB->y && pos->y < vtxC->y) {
            break;
        }

        nx = COLPOLY_GET_NORMAL(curPoly->normal.x);
        ny = COLPOLY_GET_NORMAL(curPoly->normal.y);
        nz = COLPOLY_GET_NORMAL(curPoly->normal.z);
        normalXZ = sqrtf(SQ(nx) + SQ(nz));
        planeDist = Math3D_DistPlaneToPos(nx, ny, nz, curPoly->dist, &resultPos);
        if (radius < fabsf(planeDist) || COLPOLY_VIA_FLAG_TEST(curPoly->flags_vIA, xpFlags) ||
            (COLPOLY_VIA_FLAG_TEST(curPoly->flags_vIB, 4) && ((actor != NULL && actor->category != ACTORCAT_PLAYER) ||
                                                              (actor == NULL && xpFlags != COLPOLY_IGNORE_CAMERA)))) {
            if (curNode->next == SS_NULL) {
                break;
            } else {
                curNode = &colCtx->polyNodes.tbl[curNode->next];
                continue;
            }
        }

        invNormalXZ = 1.0f / normalXZ;
        temp_f16 = fabsf(nx) * invNormalXZ;
        if (temp_f16 < 0.4f) {
            if (curNode->next == SS_NULL) {
                break;
            } else {
                curNode = &colCtx->polyNodes.tbl[curNode->next];
                continue;
            }
        }

        // compute curPoly xMin/xMax
        xTemp = vtxA->x;
        xMax = xMin = xTemp;
        xTemp = vtxB->x;

        if (xTemp < xMin) {
            xMin = xTemp;
        } else if (xMax < xTemp) {
            xMax = xTemp;
        }
        xTemp = vtxC->x;
        if (xTemp < xMin) {
            xMin = xTemp;
        } else if (xMax < xTemp) {
            xMax = xTemp;
        }

        xMin -= radius;
        xMax += radius;

        if (resultPos.x < xMin || xMax < resultPos.x) {
            if (curNode->next == SS_NULL) {
                break;
            } else {
                curNode = &colCtx->polyNodes.tbl[curNode->next];
                continue;
            }
        }
        if (CollisionPoly_CheckXIntersectApprox(curPoly, vtxList, pos->y, resultPos.z, &intersect)) {
            f32 test = intersect - resultPos.x;

            if (fabsf(test) <= radius / temp_f16) {
                if (test * nx <= 4.0f) {
                    if (BgCheck_ComputeWallDisplacement(colCtx, curPoly, &resultPos.x, &resultPos.z, nx, ny, nz,
                                                        invNormalXZ, planeDist, radius, outPoly)) {
                        result = true;
                    }
                }
            }
        }
        if (curNode->next == SS_NULL) {
            break;
        } else {
            curNode = &colCtx->polyNodes.tbl[curNode->next];
            continue;
        }
    }

    *outX = resultPos.x;
    *outZ = resultPos.z;
    return result;
}

/**
 * Tests for collision with a static poly ceiling
 * returns true if a collision occurs, else false
 * `outPoly` returns the poly collided with
 * `outY` returns the y coordinate needed to not collide with `outPoly`
 */
s32 BgCheck_CheckStaticCeiling(StaticLookup* lookup, u16 xpFlags, CollisionContext* colCtx, f32* outY, Vec3f* pos,
                               f32 checkHeight, CollisionPoly** outPoly, Actor* actor) {
    s32 result = false;
    CollisionPoly* curPoly;
    CollisionPoly* polyList;
    s32 curPolyId;
    f32 ceilingY;
    SSNode* curNode;
    Vec3s* vtxList;

    if (lookup->ceiling.head == SS_NULL) {
        return result;
    }
    polyList = colCtx->colHeader->polyList;
    vtxList = colCtx->colHeader->vtxList;
    curNode = &colCtx->polyNodes.tbl[lookup->ceiling.head];

    *outY = pos->y;

    while (true) {
        f32 intersectDist;
        f32 ny;

        curPolyId = curNode->polyId;
        curPoly = &polyList[curPolyId];
        if (COLPOLY_VIA_FLAG_TEST(colCtx->colHeader->polyList[curPolyId].flags_vIA, xpFlags) ||
            (COLPOLY_VIA_FLAG_TEST(colCtx->colHeader->polyList[curPolyId].flags_vIB, 4) &&
             ((actor != NULL && actor->category != ACTORCAT_PLAYER) ||
              (actor == NULL && xpFlags != COLPOLY_IGNORE_CAMERA)))) {
            if (curNode->next == SS_NULL) {
                break;
            } else {
                curNode = &colCtx->polyNodes.tbl[curNode->next];
                continue;
            }
        }

        if (CollisionPoly_CheckYIntersectApprox2(curPoly, vtxList, pos->x, pos->z, &ceilingY)) {
            intersectDist = ceilingY - *outY;
            ny = COLPOLY_GET_NORMAL(curPoly->normal.y);

            if (intersectDist > 0 && intersectDist < checkHeight && intersectDist * ny <= 0) {
                *outY = ceilingY - checkHeight;
                *outPoly = curPoly;
                result = true;
            }
        }
        if (curNode->next == SS_NULL) {
            break;
        } else {
            curNode = &colCtx->polyNodes.tbl[curNode->next];
            continue;
        }
    }
    return result;
}

/**
 * Tests if line `posA` to `posB` intersects with a static poly in list `ssList`. Uses polyCheckTbl
 * returns true if such a poly exists, else false
 * `outPoly` returns the pointer of the poly intersected
 * `posB` and `outPos` returns the point of intersection with `outPoly`
 * `outDistSq` returns the squared distance from `posA` to the point of intersect
 */
#ifdef NON_MATCHING
s32 BgCheck_CheckLineAgainstSSList(StaticLineTest* arg0) {
    CollisionContext* colCtx;
    s32 result;
    Vec3f polyIntersect; // sp7C
    SSNode* curNode;
    u8* checkedPoly;
    f32 minY;
    f32 distSq;
    BgLineVsPolyTest test; // sp50
    s16 polyId;
    CollisionPoly* polyList;

    result = false;
    if (arg0->ssList->head == SS_NULL) {
        return result;
    }
    colCtx = arg0->colCtx;
    curNode = &colCtx->polyNodes.tbl[arg0->ssList->head];
    polyList = colCtx->colHeader->polyList;
    test.vtxList = colCtx->colHeader->vtxList;
    test.posA = arg0->posA;
    test.posB = arg0->posB;
    test.planeIntersect = &polyIntersect; // reorder maybe
    test.checkOneFace = (arg0->bccFlags & BGCHECK_CHECK_ONE_FACE) != 0;
    test.checkDist = arg0->checkDist;

    while (true) {
        polyId = curNode->polyId;
        test.poly = &polyList[polyId];
        checkedPoly = &arg0->colCtx->polyNodes.polyCheckTbl[polyId];

        if (*checkedPoly == true ||
            (arg0->xpFlags2 != 0 && !COLPOLY_VIA_FLAG_TEST(test.poly->flags_vIA, arg0->xpFlags2)) ||
            COLPOLY_VIA_FLAG_TEST(test.poly->flags_vIA, arg0->xpFlags1) ||
            (COLPOLY_VIA_FLAG_TEST(test.poly->flags_vIB, 4) &&
             ((arg0->actor != NULL && arg0->actor->category != ACTORCAT_PLAYER) ||
              (arg0->actor == NULL && arg0->xpFlags1 != COLPOLY_IGNORE_CAMERA)))) {
            if (curNode->next == SS_NULL) {
                break;
            } else {
                curNode = &colCtx->polyNodes.tbl[curNode->next];
                continue;
            }
        }
        *checkedPoly = true;
        minY = CollisionPoly_GetMinY(test.poly, test.vtxList);
        if (test.posA->y < minY && test.posB->y < minY) {
            break;
        }
        if (CollisionPoly_LineVsPoly(&test)) {
            distSq = Math3D_Vec3fDistSq(test.posA, test.planeIntersect);
            if (distSq < arg0->outDistSq) {
                arg0->outDistSq = distSq;
                *arg0->outPos = *test.planeIntersect;
                *arg0->posB = *test.planeIntersect;
                *arg0->outPoly = test.poly;
                result = true;
            }
        }
        if (curNode->next == SS_NULL) {
            break;
        }
        curNode = &arg0->colCtx->polyNodes.tbl[curNode->next];
    }
    return result;
}
#else
s32 BgCheck_CheckLineAgainstSSList(StaticLineTest* arg0);
#pragma GLOBAL_ASM("asm/non_matchings/code/z_bgcheck/BgCheck_CheckLineAgainstSSList.s")
#endif

/**
 * Tests if line `posA` to `posB` intersects with a static poly in `lookup`. Uses polyCheckTbl
 * returns true if such a poly exists, else false
 * `outPoly` returns the pointer of the poly intersected
 * `posB` and `outPos` returns the point of intersection with `outPoly`
 * `outDistSq` returns the squared distance from `posA` to the point of intersect
 */
s32 BgCheck_CheckLineInSubdivision(StaticLineTest* arg0) {
    s32 result = false;

    if ((arg0->bccFlags & BGCHECK_CHECK_FLOOR) && arg0->lookup->floor.head != SS_NULL) {
        arg0->ssList = &arg0->lookup->floor;
        if (BgCheck_CheckLineAgainstSSList(arg0)) {
            result = true;
        }
    }

    if ((arg0->bccFlags & BGCHECK_CHECK_WALL) && arg0->lookup->wall.head != SS_NULL) {
        arg0->ssList = &arg0->lookup->wall;
        if (BgCheck_CheckLineAgainstSSList(arg0)) {
            result = true;
        }
    }

    if ((arg0->bccFlags & BGCHECK_CHECK_CEILING) && arg0->lookup->ceiling.head != SS_NULL) {
        arg0->ssList = &arg0->lookup->ceiling;
        if (BgCheck_CheckLineAgainstSSList(arg0)) {
            result = true;
        }
    }
    return result;
}

/**
 * Get first static poly intersecting sphere `center` `radius` from list `node`
 * returns true if any poly intersects the sphere, else returns false
 * `outPoly` returns the pointer of the first poly found that intersects
 */
s32 BgCheck_SphVsFirstStaticPolyList(SSNode* node, u16 xpFlags, CollisionContext* colCtx, Vec3f* center, f32 radius,
                                     CollisionPoly** outPoly, Actor* actor) {
    Vec3s* vtxList;
    CollisionPoly* polyList;
    CollisionPoly* curPoly;
    u16 nextId;
    s16 curPolyId;

    polyList = colCtx->colHeader->polyList;
    vtxList = colCtx->colHeader->vtxList;

    while (true) {
        curPolyId = node->polyId;
        curPoly = &polyList[curPolyId];
        if (COLPOLY_VIA_FLAG_TEST(colCtx->colHeader->polyList[curPolyId].flags_vIA, xpFlags) ||
            (COLPOLY_VIA_FLAG_TEST(colCtx->colHeader->polyList[curPolyId].flags_vIB, 4) &&
             ((actor != NULL && actor->category != ACTORCAT_PLAYER) ||
              (actor == NULL && xpFlags != COLPOLY_IGNORE_CAMERA)))) {
            if (node->next != SS_NULL) {
                node = &colCtx->polyNodes.tbl[node->next];
                continue;
            }
            break;
        }

        if (center->y + radius < vtxList[COLPOLY_VTX_INDEX(curPoly->flags_vIA)].y &&
            center->y + radius < vtxList[COLPOLY_VTX_INDEX(curPoly->flags_vIB)].y &&
            center->y + radius < vtxList[curPoly->vIC].y) {
            break;
        }

        if (CollisionPoly_SphVsPoly(curPoly, vtxList, center, radius)) {
            *outPoly = curPoly;
            return true;
        }
        if (node->next != SS_NULL) {
            node = &colCtx->polyNodes.tbl[node->next];
            { s32 pad; }

            continue;
        }
        break;
    }
    return false;
}

/**
 * Get first static poly intersecting sphere `center` `radius` within `lookup`
 * returns true if any poly intersects the sphere, else false
 * `outPoly` returns the first poly found that intersects
 */
s32 BgCheck_SphVsFirstStaticPoly(StaticLookup* lookup, u16 xpFlags, CollisionContext* colCtx, Vec3f* center, f32 radius,
                                 CollisionPoly** outPoly, u16 bciFlags, Actor* actor) {
    if (lookup->floor.head != SS_NULL && !(bciFlags & BGCHECK_IGNORE_FLOOR) &&
        BgCheck_SphVsFirstStaticPolyList(&colCtx->polyNodes.tbl[lookup->floor.head], xpFlags, colCtx, center, radius,
                                         outPoly, actor)) {
        return true;
    }

    if (lookup->wall.head != SS_NULL && !(bciFlags & BGCHECK_IGNORE_WALL) &&
        BgCheck_SphVsFirstStaticPolyList(&colCtx->polyNodes.tbl[lookup->wall.head], xpFlags, colCtx, center, radius,
                                         outPoly, actor)) {
        return true;
    }

    if (lookup->ceiling.head != SS_NULL && !(bciFlags & BGCHECK_IGNORE_CEILING) &&
        BgCheck_SphVsFirstStaticPolyList(&colCtx->polyNodes.tbl[lookup->ceiling.head], xpFlags, colCtx, center, radius,
                                         outPoly, actor)) {
        return true;
    }

    return false;
}

/**
 * Get StaticLookup from `pos`
 * Does not return NULL
 */
StaticLookup* BgCheck_GetNearestStaticLookup(CollisionContext* colCtx, StaticLookup* lookupTbl, Vec3f* pos) {
    Vec3i sector;
    s32 subdivAmountX;

    BgCheck_GetStaticLookupIndicesFromPos(colCtx, pos, &sector);
    subdivAmountX = colCtx->subdivAmount.x;
    return (sector.z * subdivAmountX) * colCtx->subdivAmount.y + lookupTbl + sector.x + sector.y * subdivAmountX;
}

/**
 * Get StaticLookup from `pos`
 * Returns NULL if just outside the mesh bounding box
 */
StaticLookup* BgCheck_GetStaticLookup(CollisionContext* colCtx, StaticLookup* lookupTbl, Vec3f* pos) {
    Vec3i sector;
    s32 subdivAmountX;

    if (!BgCheck_PosInStaticBoundingBox(colCtx, pos)) {
        return NULL;
    }
    BgCheck_GetStaticLookupIndicesFromPos(colCtx, pos, &sector);
    subdivAmountX = colCtx->subdivAmount.x;
    return (sector.z * subdivAmountX) * colCtx->subdivAmount.y + lookupTbl + sector.x + sector.y * subdivAmountX;
}

/**
 * Get StaticLookup subdivision indices from `pos`
 * `sector` returns the subdivision x,y,z indices containing or is nearest to `pos`
 */
void BgCheck_GetStaticLookupIndicesFromPos(CollisionContext* colCtx, Vec3f* pos, Vec3i* sector) {
    sector->x = (pos->x - colCtx->minBounds.x) * colCtx->subdivLengthInv.x;
    sector->y = (pos->y - colCtx->minBounds.y) * colCtx->subdivLengthInv.y;
    sector->z = (pos->z - colCtx->minBounds.z) * colCtx->subdivLengthInv.z;

    if (sector->x < 0) {
        sector->x = 0;
    } else if (sector->x >= colCtx->subdivAmount.x) {
        sector->x = colCtx->subdivAmount.x - 1;
    }

    if (sector->y < 0) {
        sector->y = 0;
    } else if (sector->y >= colCtx->subdivAmount.y) {
        sector->y = colCtx->subdivAmount.y - 1;
    }

    if (sector->z < 0) {
        sector->z = 0;
    } else if (sector->z >= colCtx->subdivAmount.z) {
        sector->z = colCtx->subdivAmount.z - 1;
    }
}

/**
 * Get negative bias subdivision indices
 * decrements indices if `pos` is within BGCHECK_SUBDIV_OVERLAP units of the negative subdivision boundary
 * `sx`, `sy`, `sz` returns the subdivision x, y, z indices
 */
void BgCheck_GetSubdivisionMinBounds(CollisionContext* colCtx, Vec3f* pos, s32* sx, s32* sy, s32* sz) {
    f32 dx = pos->x - colCtx->minBounds.x;
    f32 dy = pos->y - colCtx->minBounds.y;
    f32 dz = pos->z - colCtx->minBounds.z;

    *sx = dx * colCtx->subdivLengthInv.x;
    *sy = dy * colCtx->subdivLengthInv.y;
    *sz = dz * colCtx->subdivLengthInv.z;

    if (((s32)dx % (s32)colCtx->subdivLength.x < BGCHECK_SUBDIV_OVERLAP) && (*sx > 0)) {
        *sx -= 1;
    }

    if (((s32)dy % (s32)colCtx->subdivLength.y < BGCHECK_SUBDIV_OVERLAP) && (*sy > 0)) {
        *sy -= 1;
    }

    if (((s32)dz % (s32)colCtx->subdivLength.z < BGCHECK_SUBDIV_OVERLAP) && (*sz > 0)) {
        *sz -= 1;
    }
}

void BgCheck_CreateVertexFromVec3f(Vec3s* vertex, Vec3f* vector) {
    vertex->x = vector->x;
    vertex->y = vector->y;
    vertex->z = vector->z;
}

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800BFD84.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800BFDEC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_PolygonGetMinY.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_PolygonGetNormal.asm")

    CollisionPoly_GetNormalF(poly, &nx, &ny, &nz);
    dist = poly->dist;

    if (Math3D_TriChkLineSegParaYIntersect(&va, &vb, &vc, nx, ny, nz, dist, min->z, min->x, &intersect, min->y,
                                           max->y) ||
        Math3D_TriChkLineSegParaYIntersect(&va, &vb, &vc, nx, ny, nz, dist, max->z, min->x, &intersect, min->y,
                                           max->y) ||
        Math3D_TriChkLineSegParaYIntersect(&va, &vb, &vc, nx, ny, nz, dist, min->z, max->x, &intersect, min->y,
                                           max->y) ||
        Math3D_TriChkLineSegParaYIntersect(&va, &vb, &vc, nx, ny, nz, dist, max->z, max->x, &intersect, min->y,
                                           max->y)) {
        return true;
    }
    if (Math3D_TriChkLineSegParaZIntersect(&va, &vb, &vc, nx, ny, nz, dist, min->x, min->y, &intersect, min->z,
                                           max->z) ||
        Math3D_TriChkLineSegParaZIntersect(&va, &vb, &vc, nx, ny, nz, dist, min->x, max->y, &intersect, min->z,
                                           max->z) ||
        Math3D_TriChkLineSegParaZIntersect(&va, &vb, &vc, nx, ny, nz, dist, max->x, min->y, &intersect, min->z,
                                           max->z) ||
        Math3D_TriChkLineSegParaZIntersect(&va, &vb, &vc, nx, ny, nz, dist, max->x, max->y, &intersect, min->z,
                                           max->z)) {
        return true;
    }
    if (Math3D_TriChkLineSegParaXIntersect(&va, &vb, &vc, nx, ny, nz, dist, min->y, min->z, &intersect, min->x,
                                           max->x) ||
        Math3D_TriChkLineSegParaXIntersect(&va, &vb, &vc, nx, ny, nz, dist, min->y, max->z, &intersect, min->x,
                                           max->x) ||
        Math3D_TriChkLineSegParaXIntersect(&va, &vb, &vc, nx, ny, nz, dist, max->y, min->z, &intersect, min->x,
                                           max->x) ||
        Math3D_TriChkLineSegParaXIntersect(&va, &vb, &vc, nx, ny, nz, dist, max->y, max->z, &intersect, min->x,
                                           max->x)) {
        return true;
    }

    BgCheck_Vec3sToVec3f(&vtxList[COLPOLY_VTX_INDEX(poly->flags_vIA)], &va2);
    BgCheck_Vec3sToVec3f(&vtxList[COLPOLY_VTX_INDEX(poly->flags_vIB)], &vb2);
    BgCheck_Vec3sToVec3f(&vtxList[poly->vIC], &vc2);
    if (Math3D_LineVsCube(min, max, &va2, &vb2) || Math3D_LineVsCube(min, max, &vb2, &vc2) ||
        Math3D_LineVsCube(min, max, &vc2, &va2)) {
        return true;
    }
    return false;
}

/**
 * Initialize StaticLookup Table
 * returns size of table, in bytes
 */
u32 BgCheck_InitStaticLookup(CollisionContext* colCtx, GlobalContext* globalCtx, StaticLookup* lookupTbl) {
    Vec3s* vtxList;
    CollisionPoly* polyList;
    s32 polyMax;
    s32 polyIdx;
    s32 sx;
    s32 sy;
    s32 sz;
    // subdivMin indices
    s32 sxMin;
    s32 syMin;
    s32 szMin;
    // subdivMax indices
    s32 sxMax;
    s32 syMax;
    s32 szMax;
    // subdiv min/max bounds for adding a poly
    Vec3f curSubdivMin;
    Vec3f curSubdivMax;
    CollisionHeader* colHeader = colCtx->colHeader;
    s32 pad[2];
    StaticLookup* iLookup;
    StaticLookup* jLookup;
    StaticLookup* lookup;
    s32 sp98;
    f32 subdivLengthX;
    f32 subdivLengthY;
    f32 subdivLengthZ;

    for (iLookup = lookupTbl;
         iLookup < (colCtx->subdivAmount.x * colCtx->subdivAmount.y * colCtx->subdivAmount.z + lookupTbl); iLookup++) {
        iLookup->floor.head = SS_NULL;
        iLookup->wall.head = SS_NULL;
        iLookup->ceiling.head = SS_NULL;
    }

    polyMax = colHeader->numPolygons;
    vtxList = colHeader->vtxList;
    polyList = colHeader->polyList;
    sp98 = colCtx->subdivAmount.x * colCtx->subdivAmount.y;
    subdivLengthX = colCtx->subdivLength.x + (2 * BGCHECK_SUBDIV_OVERLAP);
    subdivLengthY = colCtx->subdivLength.y + (2 * BGCHECK_SUBDIV_OVERLAP);
    subdivLengthZ = colCtx->subdivLength.z + (2 * BGCHECK_SUBDIV_OVERLAP);

    for (polyIdx = 0; polyIdx < polyMax; polyIdx++) {
        BgCheck_GetPolySubdivisionBounds(colCtx, vtxList, polyList, &sxMin, &syMin, &szMin, &sxMax, &syMax, &szMax,
                                         polyIdx);
        iLookup = szMin * sp98 + lookupTbl;
        curSubdivMin.z = (colCtx->subdivLength.z * szMin + colCtx->minBounds.z) - BGCHECK_SUBDIV_OVERLAP;
        curSubdivMax.z = curSubdivMin.z + subdivLengthZ;

        for (sz = szMin; sz < szMax + 1; sz++) {
            jLookup = (colCtx->subdivAmount.x * syMin) + iLookup;
            curSubdivMin.y = (colCtx->subdivLength.y * syMin + colCtx->minBounds.y) - BGCHECK_SUBDIV_OVERLAP;
            curSubdivMax.y = curSubdivMin.y + subdivLengthY;

            for (sy = syMin; sy < syMax + 1; sy++) {
                lookup = sxMin + jLookup;
                curSubdivMin.x = (colCtx->subdivLength.x * sxMin + colCtx->minBounds.x) - BGCHECK_SUBDIV_OVERLAP;
                curSubdivMax.x = curSubdivMin.x + subdivLengthX;

                for (sx = sxMin; sx < sxMax + 1; sx++) {
                    if (BgCheck_PolyIntersectsSubdivision(&curSubdivMin, &curSubdivMax, polyList, vtxList, polyIdx)) {
                        StaticLookup_AddPoly(lookup, colCtx, polyList, vtxList, polyIdx);
                    }
                    curSubdivMin.x += colCtx->subdivLength.x;
                    curSubdivMax.x += colCtx->subdivLength.x;
                    lookup++;
                }
                curSubdivMin.y += colCtx->subdivLength.y;
                curSubdivMax.y += colCtx->subdivLength.y;
                jLookup += colCtx->subdivAmount.x;
            }
            curSubdivMin.z += colCtx->subdivLength.z;
            curSubdivMax.z += colCtx->subdivLength.z;
            iLookup += sp98;
        }
    }
    return colCtx->polyNodes.count * sizeof(SSNode);
}

/**
 * Returns whether the current scene should reserve less memory for it's collision lookup
 */
s32 BgCheck_IsSmallMemScene(GlobalContext* globalCtx) {
    s16* i;

    for (i = sSmallMemScenes; i < sSmallMemScenes + ARRAY_COUNT(sSmallMemScenes); i++) {
        if (globalCtx->sceneNum == *i) {
            return true;
        }
    }
    return false;
}

/**
 * Get custom scene memSize
 */
s32 BgCheck_TryGetCustomMemsize(s32 sceneId, u32* memSize) {
    s32 i;

    for (i = 0; i < ARRAY_COUNT(sSceneMemList); i++) {
        if (sceneId == sSceneMemList[i].sceneId) {
            *memSize = sSceneMemList[i].memSize;
            return true;
        }
    }
    return false;
}

/**
 * Compute subdivLength for scene mesh lookup, for a single dimension
 */
void BgCheck_SetSubdivisionDimension(f32 min, s32 subdivAmount, f32* max, f32* subdivLength, f32* subdivLengthInv) {
    f32 length = (*max - min);

    *subdivLength = (s32)(length / subdivAmount) + 1;
    *subdivLength = CLAMP_MIN(*subdivLength, BGCHECK_SUBDIV_MIN);
    *subdivLengthInv = 1.0f / *subdivLength;

    *max = *subdivLength * subdivAmount + min;
}

s32 BgCheck_GetSpecialSceneMaxObjects(GlobalContext* globalCtx, s32* maxNodes, s32* maxPolygons, s32* maxVertices) {
    s32 i;

    for (i = 0; i < ARRAY_COUNT(sCustomDynapolyMem); i++) {
        if (globalCtx->sceneNum == sCustomDynapolyMem[i].sceneId) {
            *maxNodes = sCustomDynapolyMem[i].maxNodes;
            *maxPolygons = sCustomDynapolyMem[i].maxPolygons;
            *maxVertices = sCustomDynapolyMem[i].maxVertices;
            return true;
        }
    }
    return false;
}

/**
 * Allocate CollisionContext
 */
void BgCheck_Allocate(CollisionContext* colCtx, GlobalContext* globalCtx, CollisionHeader* colHeader) {
    u32 tblMax;
    u32 memSize;
    u32 lookupTblMemSize;
    SSNodeList* nodeList;
    s32 customNodeListMax;

    customNodeListMax = -1;
    colCtx->colHeader = colHeader;
    colCtx->flags = 0;

    if (BgCheck_IsSmallMemScene(globalCtx)) {
        colCtx->memSize = 0xF000;
        colCtx->dyna.polyNodesMax = 1000;
        colCtx->dyna.polyListMax = 512;
        colCtx->dyna.vtxListMax = 512;
        colCtx->subdivAmount.x = 16;
        colCtx->subdivAmount.y = 4;
        colCtx->subdivAmount.z = 16;
    } else {
        u32 customMemSize;
        s32 useCustomSubdivisions;
        s32 i;

        if (BgCheck_TryGetCustomMemsize(globalCtx->sceneNum, &customMemSize)) {
            colCtx->memSize = customMemSize;
        } else {
            colCtx->memSize = 0x23000;
        }
        colCtx->dyna.polyNodesMax = 1000;
        colCtx->dyna.polyListMax = 544;
        colCtx->dyna.vtxListMax = 512;
        BgCheck_GetSpecialSceneMaxObjects(globalCtx, &colCtx->dyna.polyNodesMax, &colCtx->dyna.polyListMax,
                                          &colCtx->dyna.vtxListMax);
        useCustomSubdivisions = false;

        for (i = 0; i < ARRAY_COUNT(sSceneSubdivisionList); i++) {
            if (globalCtx->sceneNum == sSceneSubdivisionList[i].sceneId) {
                colCtx->subdivAmount.x = sSceneSubdivisionList[i].subdivAmount.x;
                colCtx->subdivAmount.y = sSceneSubdivisionList[i].subdivAmount.y;
                colCtx->subdivAmount.z = sSceneSubdivisionList[i].subdivAmount.z;
                useCustomSubdivisions = true;
                customNodeListMax = sSceneSubdivisionList[i].nodeListMax;
            }
        }
        if (useCustomSubdivisions == false) {
            colCtx->subdivAmount.x = 16;
            colCtx->subdivAmount.y = 4;
            colCtx->subdivAmount.z = 16;
        }
    }
    colCtx->lookupTbl = THA_AllocEndAlign(
        &globalCtx->state.heap,
        colCtx->subdivAmount.x * sizeof(StaticLookup) * colCtx->subdivAmount.y * colCtx->subdivAmount.z, ~1);
    if (colCtx->lookupTbl == NULL) {
        Fault_AddHungupAndCrash("../z_bgcheck.c", 3955);
    }
    colCtx->minBounds.x = colCtx->colHeader->minBounds.x;
    colCtx->minBounds.y = colCtx->colHeader->minBounds.y;
    colCtx->minBounds.z = colCtx->colHeader->minBounds.z;
    colCtx->maxBounds.x = colCtx->colHeader->maxBounds.x;
    colCtx->maxBounds.y = colCtx->colHeader->maxBounds.y;
    colCtx->maxBounds.z = colCtx->colHeader->maxBounds.z;
    BgCheck_SetSubdivisionDimension(colCtx->minBounds.x, colCtx->subdivAmount.x, &colCtx->maxBounds.x,
                                    &colCtx->subdivLength.x, &colCtx->subdivLengthInv.x);
    BgCheck_SetSubdivisionDimension(colCtx->minBounds.y, colCtx->subdivAmount.y, &colCtx->maxBounds.y,
                                    &colCtx->subdivLength.y, &colCtx->subdivLengthInv.y);
    BgCheck_SetSubdivisionDimension(colCtx->minBounds.z, colCtx->subdivAmount.z, &colCtx->maxBounds.z,
                                    &colCtx->subdivLength.z, &colCtx->subdivLengthInv.z);
    memSize = colCtx->subdivAmount.x * sizeof(StaticLookup) * colCtx->subdivAmount.y * colCtx->subdivAmount.z +
              colCtx->colHeader->numPolygons * sizeof(u8) + colCtx->dyna.polyNodesMax * sizeof(SSNode) +
              colCtx->dyna.polyListMax * sizeof(CollisionPoly) + colCtx->dyna.vtxListMax * sizeof(Vec3s) +
              sizeof(CollisionContext);
    if (customNodeListMax > 0) {
        // tblMax is set without checking if customNodeListMax will result in a memory overflow
        // this is a non-issue as long as sSceneSubdivisionList.nodeListMax is -1
        tblMax = customNodeListMax;
    } else {
        if (colCtx->memSize < memSize) {
            Fault_AddHungupAndCrash("../z_bgcheck.c", 4011);
        }
        tblMax = (colCtx->memSize - memSize) / sizeof(SSNode);
    }

    SSNodeList_Init(&colCtx->polyNodes);
    SSNodeList_Alloc(globalCtx, &colCtx->polyNodes, tblMax, colCtx->colHeader->numPolygons);

    lookupTblMemSize = BgCheck_InitStaticLookup(colCtx, globalCtx, colCtx->lookupTbl);

    DynaPoly_Init(globalCtx, &colCtx->dyna);
    DynaPoly_Alloc(globalCtx, &colCtx->dyna);
}

/**
 * Enables CollisionContext wide flags
 * Only used to reverse water flow in Great Bay Temple
 */
void BgCheck_SetContextFlags(CollisionContext* colCtx, u32 flags) {
    colCtx->flags |= flags;
}

/**
 * Disables CollisionContext wide flags
 * Only used to revert the reverse water flow in Great Bay Temple
 */
void BgCheck_UnsetContextFlags(CollisionContext* colCtx, u32 flags) {
    colCtx->flags &= ~flags;
}

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C01B8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_CreateTriNormFromPolygon.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C02C0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C0340.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C0474.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C0668.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C06A8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C074C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C07F0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_PolygonCollidesWithSphere.asm")

/**
 * Tests if a line from `posA` to `posB` intersects with a poly
 * returns true if it does, else false
 * `posB`? `posResult` returns the point of intersection
 * `outPoly` returns the pointer to the intersected poly, while `outBgId` returns the entity the poly belongs to
 */
s32 BgCheck_CheckLineImpl(CollisionContext* colCtx, u16 xpFlags1, u16 xpFlags2, Vec3f* posA, Vec3f* posB,
                          Vec3f* posResult, CollisionPoly** outPoly, s32* outBgId, Actor* actor, f32 checkDist,
                          u32 bccFlags) {
    StaticLookup* lookupTbl;
    StaticLookup* iLookup;
    s32 subdivMin[3];
    s32 subdivMax[3];
    s32 i;
    s32 result;
    Vec3f posBTemp;
    StaticLineTest checkLine;
    Vec3f sectorMin;
    Vec3f sectorMax;
    s32 temp_lo;
    StaticLookup* lookup;
    s32 j;

    lookupTbl = colCtx->lookupTbl;
    posBTemp = *posB;

    *outBgId = BGCHECK_SCENE;

    BgCheck_ResetPolyCheckTbl(&colCtx->polyNodes, colCtx->colHeader->numPolygons);
    BgCheck_GetStaticLookupIndicesFromPos(colCtx, posA, (Vec3i*)&subdivMin);
    BgCheck_GetStaticLookupIndicesFromPos(colCtx, &posBTemp, (Vec3i*)&subdivMax);
    *posResult = *posB;
    checkLine.outDistSq = 1.0e38f;
    *outPoly = NULL;

    checkLine.colCtx = colCtx;
    checkLine.xpFlags2 = xpFlags2;
    checkLine.posA = posA;
    checkLine.posB = &posBTemp;
    checkLine.outPoly = outPoly;
    checkLine.xpFlags1 = xpFlags1;
    checkLine.outPos = posResult;
    checkLine.checkDist = checkDist;
    checkLine.bccFlags = bccFlags;
    checkLine.actor = actor;
    result = false;

    if (subdivMin[0] != subdivMax[0] || subdivMin[1] != subdivMax[1] || subdivMin[2] != subdivMax[2]) {
        for (i = 0; i < 3; i++) {
            if (subdivMax[i] < subdivMin[i]) {
                j = subdivMax[i];
                subdivMax[i] = subdivMin[i];
                subdivMin[i] = j;
            }
        }
        temp_lo = colCtx->subdivAmount.x * colCtx->subdivAmount.y;
        iLookup = lookupTbl + subdivMin[2] * temp_lo;
        sectorMin.z = subdivMin[2] * colCtx->subdivLength.z + colCtx->minBounds.z;
        sectorMax.z = colCtx->subdivLength.z + sectorMin.z;

        for (i = subdivMin[2]; i < subdivMax[2] + 1; i++) {
            StaticLookup* jLookup = iLookup + subdivMin[1] * colCtx->subdivAmount.x;
            s32 pad;

            sectorMin.y = subdivMin[1] * colCtx->subdivLength.y + colCtx->minBounds.y;
            sectorMax.y = colCtx->subdivLength.y + sectorMin.y;

            for (j = subdivMin[1]; j < subdivMax[1] + 1; j++) {
                s32 k;

                lookup = jLookup + subdivMin[0];
                sectorMin.x = subdivMin[0] * colCtx->subdivLength.x + colCtx->minBounds.x;
                sectorMax.x = colCtx->subdivLength.x + sectorMin.x;

                for (k = subdivMin[0]; k < subdivMax[0] + 1; k++) {
                    if (Math3D_LineVsCube(&sectorMin, &sectorMax, posA, &posBTemp)) {
                        checkLine.lookup = lookup;

                        if (BgCheck_CheckLineInSubdivision(&checkLine)) {
                            result = true;
                        }
                    }
                    lookup++;
                    sectorMin.x += colCtx->subdivLength.x;
                    sectorMax.x += colCtx->subdivLength.x;
                }
                jLookup += colCtx->subdivAmount.x;
                sectorMin.y += colCtx->subdivLength.y;
                sectorMax.y += colCtx->subdivLength.y;
            }
            iLookup += temp_lo;
            sectorMin.z += colCtx->subdivLength.z;
            sectorMax.z += colCtx->subdivLength.z;
        }
    } else if (BgCheck_PosInStaticBoundingBox(colCtx, posA) == false) {
        return false;
    } else {
        checkLine.lookup = BgCheck_GetNearestStaticLookup(colCtx, lookupTbl, posA);
        result = BgCheck_CheckLineInSubdivision(&checkLine);
        if (result) {
            checkLine.outDistSq = Math3D_Vec3fDistSq(posResult, posA);
        }
    }
    if ((bccFlags & BGCHECK_CHECK_DYNA) &&
        BgCheck_CheckLineAgainstDyna(colCtx, xpFlags1, posA, &posBTemp, posResult, outPoly, &checkLine.outDistSq,
                                     outBgId, actor, checkDist, bccFlags)) {
        result = true;
    }
    return result;
}

/**
 * Get bccFlags
 */
u32 BgCheck_GetBccFlags(s32 checkWall, s32 checkFloor, s32 checkCeil, s32 checkOneFace, s32 checkDyna) {
    u32 result = 0;

    if (checkWall) {
        result = BGCHECK_CHECK_WALL;
    }
    if (checkFloor) {
        result |= BGCHECK_CHECK_FLOOR;
    }
    if (checkCeil) {
        result |= BGCHECK_CHECK_CEILING;
    }
    if (checkOneFace) {
        result |= BGCHECK_CHECK_ONE_FACE;
    }
    if (checkDyna) {
        result |= BGCHECK_CHECK_DYNA;
    }
    return result;
}

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_ScenePolygonListsInsert.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C0E74.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C10FC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C1238.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C12A4.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C1B68.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C1D7C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C2008.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C20F4.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C2310.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C2460.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C2514.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C25E0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_GetPolyMinSubdivisions.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_GetPolyMaxSubdivisions.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_GetPolyMinMaxSubdivisions.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C2BE0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_SplitScenePolygonsIntoSubdivisions.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_GetIsDefaultSpecialScene.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_GetSpecialSceneMaxMemory.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_CalcSubdivisionSize.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_GetSpecialSceneMaxObjects.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_Init.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C3C00.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C3C14.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_GetActorMeshHeader.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C3C94.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C3D50.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C3F40.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C3FA0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C4000.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C4058.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C40B4.asm")

f32 func_800C411C(CollisionContext* colCtx, CollisionPoly** arg1, s32* arg2, Actor* actor, Vec3f* pos) {
    return func_800C3D50(0, colCtx, 2, arg1, arg2, pos, actor, 28, 1.0f, 0);
}

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C4188.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C41E4.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C4240.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C42A8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C4314.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C43CC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C4488.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C44F0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C455C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C45C4.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C4C74.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C4CD8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C4D3C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C4DA4.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C4E10.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C4F38.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C4F84.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C4FD4.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C5464.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C54AC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C5538.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C55C4.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C5650.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C56E0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C576C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C57F8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C583C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C58C8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C5954.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C5A20.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C5A64.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_ScenePolygonListsInit.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_ScenePolygonListsAlloc.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C5B80.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_ScenePolygonListsReserveNode.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_ActorMeshParamsInit.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_SetActorMeshParams.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_AreActorMeshParamsEqual.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_ActorMeshPolyListsHeadsInit.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_ActorMeshPolyListsInit.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_ActorMeshVerticesIndexInit.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_ActorMeshWaterboxesIndexInit.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_ActorMeshInit.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_ActorMeshInitFromActor.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_HasActorMeshChanged.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_PolygonsInit.asm")

    dynaActor = DynaPoly_GetActor(dynaRaycast->colCtx, *dynaRaycast->bgId);
    if ((result != BGCHECK_Y_MIN) && (dynaActor != NULL) && (dynaRaycast->globalCtx != NULL)) {
        pauseState = dynaRaycast->globalCtx->pauseCtx.state != 0;
        if (!pauseState) {
            pauseState = dynaRaycast->globalCtx->pauseCtx.debugState != 0;
        }
        if (!pauseState && (dynaRaycast->colCtx->dyna.bgActorFlags[*dynaRaycast->bgId] & 2)) {
            curTransform = &dynaRaycast->dyna->bgActors[*dynaRaycast->bgId].curTransform;
            polyMin = &dynaRaycast->dyna
                           ->polyList[(u16)dynaRaycast->dyna->bgActors[*dynaRaycast->bgId].dynaLookup.polyStartIndex];
            polyIndex = *dynaRaycast->resultPoly - polyMin;
            poly = &dynaRaycast->dyna->bgActors[*dynaRaycast->bgId].colHeader->polyList[polyIndex];

            SkinMatrix_SetScaleRotateYRPTranslate(&srpMtx, curTransform->scale.x, curTransform->scale.y,
                                                  curTransform->scale.z, curTransform->rot.x, curTransform->rot.y,
                                                  curTransform->rot.z, curTransform->pos.x, curTransform->pos.y,
                                                  curTransform->pos.z);

            vtxList = dynaRaycast->dyna->bgActors[*dynaRaycast->bgId].colHeader->vtxList;

            for (i2 = 0; i2 < 3; i2++) {
                Math_Vec3s_ToVec3f(&vtx, &vtxList[COLPOLY_VTX_INDEX(poly->vtxData[i2])]);
                SkinMatrix_Vec3fMtxFMultXYZ(&srpMtx, &vtx, &polyVtx[i2]);
            }
            Math3D_SurfaceNorm(&polyVtx[0], &polyVtx[1], &polyVtx[2], &polyNorm);
            magnitude = Math3D_Vec3fMagnitude(&polyNorm);

            if (!IS_ZERO(magnitude)) {
                polyNorm.x *= 1.0f / magnitude;
                polyNorm.y *= 1.0f / magnitude;
                polyNorm.z *= 1.0f / magnitude;
                polyDist = -DOTXYZ(polyNorm, polyVtx[0]);
                if (Math3D_TriChkPointParaYIntersectInsideTri(
                        &polyVtx[0], &polyVtx[1], &polyVtx[2], polyNorm.x, polyNorm.y, polyNorm.z, polyDist,
                        dynaRaycast->pos->z, dynaRaycast->pos->x, &intersect, dynaRaycast->checkDist)) {
                    if (fabsf(intersect - result) < 1.0f) {

                        result = intersect;
                    }
                }
            }
        }
    }
    return result;
}
#else
#pragma GLOBAL_ASM("asm/non_matchings/code/z_bgcheck/BgCheck_RaycastFloorDyna.s")
#endif

/**
 * Performs collision detection on a BgActor's wall polys on sphere `pos`, `radius`
 * returns true if a collision was detected
 * `outX` `outZ` return the displaced x,z coordinates
 * `outPoly` returns the pointer to the nearest poly collided with, or NULL
 * `outBgId` returns `bgId` if the poly SurfaceType's wall damage flag is not set, else ?
 */
s32 BgCheck_SphVsDynaWallInBgActor(CollisionContext* colCtx, u16 xpFlags, DynaCollisionContext* dyna, SSList* ssList,
                                   f32* outX, f32* outZ, CollisionPoly** outPoly, s32* outBgId, Vec3f* pos, f32 radius,
                                   s32 bgId, Actor* actor) {
    f32 temp;
    f32 intersect;
    s32 result = false;
    CollisionPoly* poly;
    SSNode* curNode;
    f32 nx;
    f32 ny;
    f32 nz;
    Vec3f resultPos;
    s16 polyId;
    f32 zTemp;
    f32 xTemp;
    f32 normalXZ;
    f32 invNormalXZ;
    f32 planeDist;
    f32 temp_f18;
    f32 zMin;
    f32 zMax;
    f32 xMin;
    f32 xMax;

    if (ssList->head == SS_NULL) {
        return result;
    }

    resultPos = *pos;
    curNode = &dyna->polyNodes.tbl[ssList->head];

    while (true) {
        polyId = curNode->polyId;
        poly = &dyna->polyList[polyId];
        CollisionPoly_GetNormalF(poly, &nx, &ny, &nz);
        normalXZ = sqrtf(SQ(nx) + SQ(nz));

        planeDist = Math3D_DistPlaneToPos(nx, ny, nz, poly->dist, &resultPos);
        if (radius < fabsf(planeDist) || COLPOLY_VIA_FLAG_TEST(poly->flags_vIA, xpFlags) ||
            ((COLPOLY_VIA_FLAG_TEST(poly->flags_vIB, 4)) && ((actor != NULL && actor->category != ACTORCAT_PLAYER) ||
                                                             (actor == NULL && xpFlags != COLPOLY_IGNORE_CAMERA)))) {
            if (curNode->next == SS_NULL) {
                break;
            } else {
                curNode = &dyna->polyNodes.tbl[curNode->next];
                continue;
            }
        }
        invNormalXZ = 1.0f / normalXZ;
        temp_f18 = fabsf(nz) * invNormalXZ;
        if (temp_f18 < 0.4f) {
            if (curNode->next == SS_NULL) {
                break;
            } else {
                curNode = &dyna->polyNodes.tbl[curNode->next];
                continue;
            }
        }

        // compute poly zMin/zMax
        zTemp = dyna->vtxList[COLPOLY_VTX_INDEX(poly->flags_vIA)].z;
        zMax = zMin = zTemp;

        zTemp = dyna->vtxList[COLPOLY_VTX_INDEX(poly->flags_vIB)].z;
        if (zTemp < zMin) {
            zMin = zTemp;
        } else if (zTemp > zMax) {
            zMax = zTemp;
        }

        zTemp = dyna->vtxList[poly->vIC].z;
        if (zTemp < zMin) {
            zMin = zTemp;
        } else if (zMax < zTemp) {
            zMax = zTemp;
        }

        zMin -= radius;
        zMax += radius;
        if (resultPos.z < zMin || zMax < resultPos.z) {
            if (curNode->next == SS_NULL) {
                break;
            } else {
                curNode = &dyna->polyNodes.tbl[curNode->next];
                continue;
            }
        }
        if (CollisionPoly_CheckZIntersectApprox(poly, dyna->vtxList, resultPos.x, pos->y, &intersect)) {
            s32 pad;

            if (fabsf(intersect - resultPos.z) <= radius / temp_f18) {
                if ((intersect - resultPos.z) * nz <= 4.0f) {
                    if (BgCheck_ComputeWallDisplacement(colCtx, poly, &resultPos.x, &resultPos.z, nx, ny, nz,
                                                        invNormalXZ, planeDist, radius, outPoly)) {
                        *outBgId = bgId;
                    }
                    result = true;
                }
            }
        }
        if (curNode->next == SS_NULL) {
            break;
        }
        curNode = &dyna->polyNodes.tbl[curNode->next];
    }

    curNode = &dyna->polyNodes.tbl[ssList->head];
    while (true) {
        polyId = curNode->polyId;
        poly = &dyna->polyList[polyId];
        CollisionPoly_GetNormalF(poly, &nx, &ny, &nz);
        normalXZ = sqrtf(SQ(nx) + SQ(nz));

        planeDist = Math3D_DistPlaneToPos(nx, ny, nz, poly->dist, &resultPos);
        if (radius < fabsf(planeDist) || COLPOLY_VIA_FLAG_TEST(poly->flags_vIA, xpFlags) ||
            (COLPOLY_VIA_FLAG_TEST(poly->flags_vIB, 4) && ((actor != NULL && actor->category != ACTORCAT_PLAYER) ||
                                                           (actor == NULL && xpFlags != COLPOLY_IGNORE_CAMERA)))) {
            if (curNode->next == SS_NULL) {
                break;
            } else {
                curNode = &dyna->polyNodes.tbl[curNode->next];
                continue;
            }
        }

        invNormalXZ = 1.0f / normalXZ;
        temp_f18 = fabsf(nx) * invNormalXZ;
        if (temp_f18 < 0.4f) {
            if (curNode->next == SS_NULL) {
                break;
            } else {
                curNode = &dyna->polyNodes.tbl[curNode->next];
                continue;
            }
        }

        // compute poly xMin/xMax
        xTemp = dyna->vtxList[COLPOLY_VTX_INDEX(poly->flags_vIA)].x;
        xMax = xMin = xTemp;
        xTemp = dyna->vtxList[COLPOLY_VTX_INDEX(poly->flags_vIB)].x;

        if (xTemp < xMin) {
            xMin = xTemp;
        } else if (xMax < xTemp) {
            xMax = xTemp;
        }
        xTemp = dyna->vtxList[poly->vIC].x;
        if (xTemp < xMin) {
            xMin = xTemp;
        } else if (xMax < xTemp) {
            xMax = xTemp;
        }

        xMin -= radius;
        xMax += radius;
        if (resultPos.x < xMin || xMax < resultPos.x) {
            if (curNode->next == SS_NULL) {
                break;
            } else {
                curNode = &dyna->polyNodes.tbl[curNode->next];
                continue;
            }
        }

        if (CollisionPoly_CheckXIntersectApprox(poly, dyna->vtxList, pos->y, resultPos.z, &intersect)) {
            f32 xIntersectDist = intersect - resultPos.x;

            if (fabsf(xIntersectDist) <= radius / temp_f18) {
                if (xIntersectDist * nx <= 4.0f) {
                    if (BgCheck_ComputeWallDisplacement(colCtx, poly, &resultPos.x, &resultPos.z, nx, ny, nz,
                                                        invNormalXZ, planeDist, radius, outPoly)) {
                        *outBgId = bgId;
                    }
                    result = true;
                }
            }
        }
        if (curNode->next == SS_NULL) {
            break;
        }
        curNode = &dyna->polyNodes.tbl[curNode->next];
    }
    *outX = resultPos.x;
    *outZ = resultPos.z;
    return result;
}

/**
 * Performs collision detection on all dyna poly walls using sphere `pos`, `radius`
 * returns true if a collision was detected
 * `outX` `outZ` return the displaced x,z coordinates
 * `outPoly` returns the pointer to the nearest poly collided with, or NULL
 * `outBgId` returns the index of the BgActor that owns `outPoly`
 * If `actor` is not NULL, an BgActor bound to that actor will be ignored
 */
s32 BgCheck_SphVsDynaWall(CollisionContext* colCtx, u16 xpFlags, f32* outX, f32* outZ, Vec3f* pos, f32 radius,
                          CollisionPoly** outPoly, s32* outBgId, Actor* actor) {
    Vec3f resultPos;
    s32 result;
    f32 r;
    f32 dz;
    f32 dx;
    BgActor* bgActor;
    s32 i;

    result = false;
    resultPos = *pos;

    for (i = 0; i < BG_ACTOR_MAX; i++) {
        if (!(colCtx->dyna.bgActorFlags[i] & 1) || (colCtx->dyna.bgActorFlags[i] & 2)) {
            continue;
        }
        if ((colCtx->dyna.bgActors + i)->actor == actor) {
            continue;
        }
        bgActor = &colCtx->dyna.bgActors[i];

        if (bgActor->minY > resultPos.y || bgActor->maxY < resultPos.y) {
            continue;
        }

        bgActor->boundingSphere.radius += (s16)radius;

        r = bgActor->boundingSphere.radius;
        dx = bgActor->boundingSphere.center.x - resultPos.x;
        dz = bgActor->boundingSphere.center.z - resultPos.z;
        if (SQ(r) < (SQ(dx) + SQ(dz)) || (!Math3D_XYInSphere(&bgActor->boundingSphere, resultPos.x, resultPos.y) &&
                                          !Math3D_YZInSphere(&bgActor->boundingSphere, resultPos.y, resultPos.z))) {
            bgActor->boundingSphere.radius -= (s16)radius;
            continue;
        }
        bgActor->boundingSphere.radius -= (s16)radius;
        if (BgCheck_SphVsDynaWallInBgActor(colCtx, xpFlags, &colCtx->dyna,
                                           &(colCtx->dyna.bgActors + i)->dynaLookup.wall, outX, outZ, outPoly, outBgId,
                                           &resultPos, radius, i, actor)) {
            resultPos.x = *outX;
            resultPos.z = *outZ;
            result = true;
        }
    }
    return result;
}

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_WaterboxListInit.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_WaterboxListAlloc.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_ActorMeshUpdateParams.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_IsActorMeshIndexValid.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_DynaInit.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_DynaAlloc.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_AddActorMesh.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_GetActorOfMesh.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C62BC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C6314.asm")

/**
 * SEGMENT_TO_VIRTUAL all active BgActor CollisionHeaders
 */
void BgCheck_InitCollisionHeaders(CollisionContext* colCtx, GlobalContext* globalCtx) {
    DynaCollisionContext* dyna = &colCtx->dyna;
    s32 i;
    u16 flag;

    for (i = 0; i < BG_ACTOR_MAX; i++) {
        flag = dyna->bgActorFlags[i];
        if ((flag & 1) && !(flag & 2)) {
            Actor_SetObjectDependency(globalCtx, dyna->bgActors[i].actor);
            CollisionHeader_SegmentedToVirtual(dyna->bgActors[i].colHeader);
        }
    }
}

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C63C4.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C641C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C6474.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_RemoveActorMesh.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C6554.asm")

/**
 * CamData return camera setting
 */
u16 func_800C9728(CollisionContext* colCtx, u32 camId, s32 bgId) {
    u16 result;
    CollisionHeader* colHeader;
    CamData* camData;

    colHeader = BgCheck_GetCollisionHeader(colCtx, bgId);
    if (colHeader == NULL) {
        return 0;
    }
    camData = colHeader->cameraDataList;
    result = camData[camId].setting;
    return result;
}

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_AddActorMeshToLists.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_ResetFlagsIfLoadedActor.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_Update.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C756C.asm")

    camData = colHeader->cameraDataList;
    if (camData == NULL) {
        return 0;
    }
    return camData[camId].numData;
}

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C76EC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C7974.asm")

/**
 * CamData get data
 */
Vec3s* func_800C98CC(CollisionContext* colCtx, s32 camId, s32 bgId) {
    CollisionHeader* colHeader = BgCheck_GetCollisionHeader(colCtx, bgId);
    CamData* cameraDataList;

    if (colHeader == NULL) {
        return NULL;
    }
    cameraDataList = colHeader->cameraDataList;
    if (cameraDataList == NULL) {
        return NULL;
    }
    return Lib_SegmentedToVirtual(cameraDataList[camId].data);
}

/**
 * SurfaceType Get data
 */
Vec3s* SurfaceType_GetCamPosData(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId) {
    CollisionHeader* colHeader = BgCheck_GetCollisionHeader(colCtx, bgId);
    CamData* camData;
    SurfaceType* surfaceTypes;

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C8A60.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C8BD0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C8DC0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C8EEC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C90AC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C921C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9380.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_RelocateMeshHeaderPointers.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_RelocateMeshHeader.asm")

u32 func_800C9B18(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId) {
    return SurfaceType_GetData(colCtx, poly, bgId, 0) >> 26 & 0xF;
}

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9640.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/BgCheck_GetPolygonAttributes.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9704.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9728.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9770.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C97F8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9844.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C98CC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9924.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C99AC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C99D4.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C99FC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9A24.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9A4C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9A7C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9AB0.asm")

/**
 * SurfaceType Get Conveyor Surface Type
 * Return type 0 (CONVEYOR_WATER) if 'poly' is a surface that will only move player underwater
 * Return type 1 (CONVEYOR_FLOOR) if `poly` is a surface that must be stood on to move player
 */
s32 SurfaceType_GetConveyorType(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId) {
    u32 flags;

    if (BgCheck_GetCollisionHeader(colCtx, bgId) == NULL) {
        return CONVEYOR_FLOOR;
    }
    if (poly == NULL) {
        return CONVEYOR_WATER;
    }
    flags = poly->flags_vIB & 0x2000;
    return !!flags;
}

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9B40.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9B68.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9B90.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9BB8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9BDC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9C24.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9C74.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9C9C.asm")

// boolean
s32 WaterBox_GetSurface1(GlobalContext* globalCtx, CollisionContext* colCtx, f32 x, f32 z, f32* ySurface,
                         WaterBox** outWaterBox) {
    return WaterBox_GetSurface1_2(globalCtx, colCtx, x, z, ySurface, outWaterBox);
}

// boolean
s32 WaterBox_GetSurface1_2(GlobalContext* globalCtx, CollisionContext* colCtx, f32 x, f32 z, f32* ySurface,
                           WaterBox** outWaterBox) {
    s32 bgId;
    return WaterBox_GetSurfaceImpl(globalCtx, colCtx, x, z, ySurface, outWaterBox, &bgId);
}

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9D14.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9D50.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9D8C.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9DDC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9E18.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9E40.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9E88.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800C9EBC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800CA1AC.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800CA1E8.asm")

    result = camData[camId].setting;
    return result;
}

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800CA568.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800CA634.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800CA648.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800CA6B8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800CA6D8.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800CA6F0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800CA9D0.asm")

#pragma GLOBAL_ASM("./asm/non_matchings/code/z_bgcheck/func_800CAA14.asm")
