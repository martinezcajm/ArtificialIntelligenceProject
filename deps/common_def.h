// common_def.h
// Toni Barella
// Jose Maria Martinez
// Artificial Intelligence - 3HND 
// ESAT 2016 - 2019
//
#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#define VERBOSE_
#include "Math/float2.h"

#ifndef ESAT_SDK
typedef struct {
	float x;
	float y;
	float z;
} Vector3;
#endif

typedef enum
{
  kErrorCode_Ok = 0,
  kErrorCode_InvalidPointer = -1,
  kErrorCode_StorageFull = -2,
  kErrorCode_IncorrectPointsNumber = -3,
  kErrorCode_EmptyPath = -4,
  kErrorCode_BadLoopsSetting = -5,
  kErrorCode_BadDirectionSetting = -5,
  kErrorCode_PathNotCreated = -6,
  kErrorCode_InvalidOrigin = -7,
  kErrorCode_InvalidDestination= -8,
  kErrorCode_PathNotFound = -9,
  kErrorCode_Timeout = -10,
	kErrorCode_Memory = -11,
	kErrorCode_File = -20
} ErrorCode;

#endif // __COMMON_DEF_H__