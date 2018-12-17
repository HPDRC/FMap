/******************************************************************************
 * $Id: maptree.h,v 1.9 2005/06/14 16:03:35 dan Exp $
 *
 * Project:  MapServer
 * Purpose:  .qix spatial index declarations.
 * Author:   Steve Lime and the MapServer team.
 *
 ******************************************************************************
 * Copyright (c) 1996-2005 Regents of the University of Minnesota.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies of this Software or works derived from this Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 *
 * $Log: maptree.h,v $
 * Revision 1.9  2005/06/14 16:03:35  dan
 * Updated copyright date to 2005
 *
 * Revision 1.8  2005/02/18 03:06:47  dan
 * Turned all C++ (//) comments into C comments (bug 1238)
 *
 * Revision 1.7  2004/10/21 04:30:56  frank
 * Added standardized headers.  Added MS_CVSID().
 *
 */

#ifndef MAPTREE_H
#define MAPTREE_H

#include <stdio.h>
#include ".\shapefil.h"
#include ".\shaperecords.h"

size_t msGetBitArraySize(int numbits); /* in mapbits.c */
char *msAllocBitArray(int numbits);
int msGetBit(char *array, int index);
void msSetBit(char *array, int index, int value);
void msFlipBit(char *array, int index);

#ifdef __cplusplus
extern "C" {
#endif

#define MS_PATH_LENGTH 1024
#define MS_FALSE 0
#define MS_TRUE 1
#define MS_INDEX_EXTENSION ".qix"

/* Shapefile types */
#define SHP_POINT 1
#define SHP_ARC 3
#define SHP_POLYGON 5
#define SHP_MULTIPOINT 8

#define SHP_POINTZ 11
#define SHP_ARCZ  13
#define SHP_POLYGONZ 15
#define SHP_MULTIPOINTZ 18

#define SHP_POINTM 21
#define SHP_ARCM  23
#define SHP_POLYGONM 25
#define SHP_MULTIPOINTM 28

#define MS_SHAPEFILE_POINT 1
#define MS_SHAPEFILE_ARC 3
#define MS_SHAPEFILE_POLYGON 5
#define MS_SHAPEFILE_MULTIPOINT 8

#define MS_SHP_POINTZ 11
#define MS_SHP_ARCZ  13
#define MS_SHP_POLYGONZ 15
#define MS_SHP_MULTIPOINTZ 18

#define MS_SHP_POINTM 21
#define MS_SHP_ARCM  23
#define MS_SHP_POLYGONM 25
#define MS_SHP_MULTIPOINTM 28


typedef unsigned char uchar;

typedef struct {
	double minX,minY,maxX,maxY;
} rectObj;

/* Shapefile object, no write access via scripts */
typedef struct {

	char source[MS_PATH_LENGTH]; /* full path to this file data */

//#ifndef SWIG
  SHPHandle hSHP; /* SHP/SHX file pointer */
//#endif

  int type; /* shapefile type */
  int numshapes; /* number of shapes */
  rectObj bounds; /* shape extent */

//#ifndef SWIG
  DBFHandle hDBF; /* DBF file pointer */
//#endif

  int lastshape;

  char *status;
  rectObj statusbounds; /* holds extent associated with the status vector */

  int isopen;
} shapefileObj;


/* this can be 2 or 4 for binary or quad tree */
#define MAX_SUBNODES 4


typedef struct shape_tree_node {
  /* area covered by this node */
  rectObj rect;
  
  /* list of shapes stored at this node. */
  int numshapes;
  int *ids;
  
  int numsubnodes;
  struct shape_tree_node *subnode[MAX_SUBNODES];
} treeNodeObj;

typedef struct {
  int numshapes;
  int maxdepth;
  treeNodeObj *root;
} treeObj;


typedef struct
{
    FILE        *fp;
    char        signature[3];
    char	LSB_order;
    char        needswap;
    char	version;
    char	flags[3];

    int         nShapes;
    int         nDepth;
} SHPTreeInfo;
typedef SHPTreeInfo * SHPTreeHandle;

#define MS_LSB_ORDER -1
#define MS_MSB_ORDER -2
#define MS_NATIVE_ORDER 0
#define MS_NEW_LSB_ORDER 1
#define MS_NEW_MSB_ORDER 2


 SHPTreeHandle msSHPDiskTreeOpen(const char * pszTree, int debug);
 void msSHPDiskTreeClose(SHPTreeHandle disktree);
 treeNodeObj *readTreeNode( SHPTreeHandle disktree );

 treeObj *msCreateTree(shapefileObj *shapefile, int maxdepth);
 void msTreeTrim(treeObj *tree);
 void msDestroyTree(treeObj *tree);

 char *msSearchTree(treeObj *tree, rectObj aoi);
 char *msSearchDiskTree(char *filename, rectObj aoi, int debug);

 treeObj *msReadTree(char *filename, int debug);
 int msWriteTree(treeObj *tree, char *filename, int LSB_order);

 //void msFilterTreeSearch(shapefileObj *shp, char *status, rectObj search_rect);
 void msFilterTreeSearch( SHPHandle psSHP, int nShape, char *status, rectObj search_rect, ShapeRecords&);
 //
 int msSHPReadBounds( SHPHandle psSHP, int hEntity, rectObj *padBounds );

 int msRectOverlap(rectObj *a, rectObj *b);
 int msRectContained(rectObj *a, rectObj *b);



#ifdef __cplusplus
}
#endif

#endif /* MAPTREE_H */
