#pragma once

#ifndef _GEOMETRYHELPER_H_
#define _GEOMETRYHELPER_H_

#include "mymath.h"

using namespace mymath;

bool isRayIntersectWithPlane(Vectorf3 v1, Vectorf3 v2, Vectorf3 v3, Vectorf3 origin, Vectorf3 direction, Vectorf3 outPointOfInersection);
bool isPointInsideOfTriangle(Vectorf3 v1, Vectorf3 v2, Vectorf3 v3, Vectorf3 point);


#endif
