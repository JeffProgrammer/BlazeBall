//------------------------------------------------------------------------------
// Copyright (c) 2014 Glenn Smith
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#ifndef interior_h
#define interior_h

#include "types.h"
#include "texture.h"

typedef enum {
	BitmapTypePNG,
	BitmapTypeJPEG,
} BitmapType;

typedef struct {
	Point3F point0;
	Point3F point1;
	Point3F point2;

	Point3F normal;
	ColorF color;

	Texture *texture;
} Triangle;

static U32 gNumCoordBins = 16;

typedef struct {
	U16 normalIndex;
	F32 planeDistance;
} Plane;

typedef struct {
	PlaneF planeX;
	PlaneF planeY;
} TexGenEq;

typedef struct {
	U16 planeIndex;
	U16 frontIndex;
	U16 backIndex;
} BSPNode;

typedef struct {
	U32 surfaceIndex;
	U16 surfaceCount;
} BSPSolidLeaf;

typedef struct {
	U32 windingStart;
	U32 windingCount;
} WindingIndex;

typedef struct {
	U16 portalStart;
	U16 portalCount;
	U32 surfaceStart;
	U32 surfaceCount;
} Zone;

typedef struct {
	U16 planeIndex;
	U16 triFanCount;
	U32 triFanStart;
	U16 zoneFront;
	U16 zoneBack;
} Portal;

typedef struct {
	U16 finalWord;
	F32 texGenXDistance;
	F32 texGenYDistance;
} LightMapF;

typedef struct {
	PNG lightMap;
	U8 keepLightMap;
} LightMap;

typedef struct {
	U32 windingStart;
	U8 windingCount;
	U16 planeIndex;
	U16 textureIndex;
	U32 texGenIndex;
	U8 surfaceFlags;
	U32 fanMask;
	LightMapF lightMap;
	U16 lightCount;
	U32 lightStateInfoStart;
	U8 mapOffsetX;
	U8 mapOffsetY;
	U8 mapSizeX;
	U8 mapSizeY;
} Surface;

typedef struct {
	U32 windingStart;
	U16 planeIndex;
	U8 surfaceFlags;
	U8 windingCount;
} NullSurface;

typedef struct {
	U32 nameIndex;
	U32 stateIndex;
	U16 stateCount;
	U16 flags;
	U32 duration;
} AnimatedLight;

typedef struct {
	U8 red;
	U8 green;
	U8 blue;
	U32 activeTime;
	U32 dataIndex;
	U16 dataCount;
} LightState;

typedef struct {
	U32 surfaceIndex;
	U32 mapIndex;
	U16 lightStateIndex;
} StateData;

typedef struct {
	U32 hullStart;
	U16 hullCount;
	F32 minX;
	F32 maxX;
	F32 minY;
	F32 maxY;
	F32 minZ;
	F32 maxZ;
	U32 surfaceStart;
	U16 surfaceCount;
	U32 planeStart;
	U32 polyListPlaneStart;
	U32 polyListPointStart;
	U32 polyListStringStart;
} ConvexHull;

typedef struct {
	U32 binStart;
	U32 binCount;
} CoordBin;

typedef struct {
	S32 T;
	S32 N;
	S32 B;
} TexMatrix;

typedef struct {
	U32 interiorFileVersion;
	U32 detailLevel;
	U32 minPixels;
	BoxF boundingBox;
	SphereF boundingSphere;
	U8 hasAlarmState;
	U32 numLightStateEntries;

	U32 numNormals;
	Point3F *normal;

	U32 numPlanes;
	Plane *plane;

	U32 numPoints;
	Point3F *point;

	U32 numPointVisibilities;
	U8 *pointVisibility;

	U32 numTexGenEqs;
	TexGenEq *texGenEq;

	U32 numBSPNodes;
	BSPNode *BSPNode;

	U32 numBSPSolidLeaves;
	BSPSolidLeaf *BSPSolidLeaf;

	U32 materialListVersion;
	U32 numMaterials;
	String *material;
	Texture **texture;

	U32 numWindings;
	U32 *index;

	U32 numWindingIndices;
	WindingIndex *windingIndex;

	U32 numZones;
	Zone *zone;

	U32 numZoneSurfaces;
	U16 *zoneSurface;

	U32 numZonePortalList;
	U16 *zonePortalList;

	U32 numPortals;
	Portal *portal;

	U32 numSurfaces;
	Surface *surface;

	U32 numNormalLMapIndices;
	U8 *normalLMapIndex;

	U32 numAlarmLMapIndices;
	U8 *alarmLMapIndex;

	U32 numNullSurfaces;
	NullSurface *nullSurface;

	U32 numLightMaps;
	LightMap *lightMap;

	U32 numSolidLeafSurfaces;
	U32 *solidLeafSurface;

	U32 numAnimatedLights;
	AnimatedLight *animatedLight;

	U32 numLightStates;
	LightState *lightState;

	U32 numStateDatas;
	StateData *stateData;

	U32 numStateDataBuffers;
	U8 *stateDataBuffer;

	U32 flags;

	U32 numNameBuffers;
	U8 *nameBufferCharacter;

	U32 numSubObjects;
	//SubObject *subObject;

	U32 numConvexHulls;
	ConvexHull *convexHull;

	U32 numConvexHullEmitStrings;
	U8 *convexHullEmitStringCharacter;

	U32 numHullIndices;
	U32 *hullIndex;

	U32 numHullPlaneIndices;
	U16 *hullPlaneIndex;

	U32 numHullEmitStringIndices;
	U32 *hullEmitStringIndex;

	U32 numHullSurfaceIndices;
	U32 *hullSurfaceIndex;

	U32 numPolyListPlanes;
	U16 *polyListPlaneIndex;

	U32 numPolyListPoints;
	U32 *polyListPointIndex;

	U32 numPolyListStrings;
	U8 *polyListStringCharacter;

	//U32 numCoordBins = gNumCoordBins * gNumCoordBins
	CoordBin *coordBin;

	U32 numCoordBinIndices;
	U16 *coordBinIndex;

	U32 coordBinMode;
	ColorI baseAmbientColor;
	ColorI alarmAmbientColor;

	U32 numTexNormals;
	Point3F *texNormal;

	U32 numTexMatrices;
	TexMatrix *texMatrix;

	U32 numTexMatIndices;
	U32 *texMatIndex;

	U32 extendedLightMapData;
	U32 lightMapBorderSize;
} Interior;

/**
 Reads an Interior from a FILE
 @arg file - The FILE to read from (updates position)
 @arg directory - The base directory for images
 @return An Interior
 */
Interior *interior_read_file(FILE *file, String directory);

bool interior_write_file(FILE *file, Interior *interior);

/**
 Frees the Interior and all memory contained within it
 @arg interior - The Interior to release
 */
void interior_release(Interior *interior);

/**
 Exports an interior into a Wavefront .obj file
 @arg interior - The Interior to export from
 @arg file - The FILE to export to
 */
void interior_export_obj(Interior *interior, FILE *file);

/**
 Renders an interior with OpenGL at an offset
 @arg interior - The Interior to render
 @arg offset - The offset from the origin from which to render
 */
void interior_render(Interior *interior, Point3F offset);

#endif
