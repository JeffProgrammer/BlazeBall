//
//  mngsupport.h
//  DifViewer
//
//  Created by Glenn Smith on 11/11/14.
//  Copyright (c) 2014 CouleeApps. All rights reserved.
//

#ifndef mngsupport_h
#define mngsupport_h

#include <stdbool.h>
#include "types.h"

bool initMNG();
void closeMNG();

bool mngReadImage(String file, U8 **bitmap, Point2I *dims);

#endif
