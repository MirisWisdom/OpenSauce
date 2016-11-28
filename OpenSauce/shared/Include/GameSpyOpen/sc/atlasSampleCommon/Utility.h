// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#include "../../common/gsDebug.h"

#ifdef __cplusplus
extern "C" {
#endif
#define ATLAS_RS_LOG(t, l, m, ... ) gsDebugFormat(GSIDebugCat_App, t, l, "{%s:%d} In %s: " m "\n", __FILE__, __LINE__, __FUNCTION__, ## __VA_ARGS__)

#define ATLAS_QS_LOG(t, l, m, ... ) gsDebugFormat(GSIDebugCat_App, t, l, "{%s:%d} In %s: " m "\n", __FILE__, __LINE__, __FUNCTION__, ## __VA_ARGS__)

int test_main(int argc, char *argv[]);

char *SCResultToString(SCResult result);

void SetDebugLogPreferences();
#ifdef __cplusplus
}
#endif