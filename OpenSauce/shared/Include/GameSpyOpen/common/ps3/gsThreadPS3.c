///////////////////////////////////////////////////////////////////////////////
// File:	gsThreadPS3.c
// SDK:		GameSpy Common
//
// Copyright (c) IGN Entertainment, Inc.  All rights reserved.  
// This software is made available only pursuant to certain license terms offered
// by IGN or its subsidiary GameSpy Industries, Inc.  Unlicensed use or use in a 
// manner not expressly authorized by IGN or GameSpy is prohibited.

#include <sys/synchronization.h>
#include <sys/ppu_thread.h>
#include <cell/atomic.h>

#include "../gsAssert.h"
#include "../gsDebug.h"
#include "../gsPlatformThread.h"
#include "../gsPlatformUtil.h"

void gsiInitializeCriticalSection(GSICriticalSection *theCrit) 
{ 
	int ret;
	sys_mutex_attribute_t defaultAttr;

	sys_mutex_attribute_initialize(defaultAttr);
	ret = sys_mutex_create(theCrit, &defaultAttr);
	if (ret != CELL_OK)
	{
		gsDebugFormat(GSIDebugCat_Common, GSIDebugType_Misc, GSIDebugLevel_WarmError,
			"Failed to initialize critical section: %d\r\n", ret);
	}

}

void gsiEnterCriticalSection(GSICriticalSection *theCrit) 
{
	int ret;
	ret = sys_mutex_lock(*theCrit, 0);
	if (ret != CELL_OK)
	{
		gsDebugFormat(GSIDebugCat_Common, GSIDebugType_Misc, GSIDebugLevel_WarmError,
			"Failed to enter critical section: %d\r\n", ret);
	}

}

void gsiLeaveCriticalSection(GSICriticalSection *theCrit) 
{ 
	int ret;
	ret = sys_mutex_unlock(*theCrit);
	if (ret != CELL_OK)
	{
		gsDebugFormat(GSIDebugCat_Common, GSIDebugType_Misc, GSIDebugLevel_WarmError,
			"Failed to enter critical section: %d\r\n", ret);
	}
}

void gsiDeleteCriticalSection(GSICriticalSection *theCrit) 
{ 
	int ret;
	ret = sys_mutex_destroy(*theCrit);
	if (ret != CELL_OK)
	{
		gsDebugFormat(GSIDebugCat_Common, GSIDebugType_Misc, GSIDebugLevel_WarmError,
			"Failed to delete critical section: %d\r\n", ret);
	}
}

gsi_u32 gsiHasThreadShutdown(GSIThreadID theThreadID) 
{ 
	// NO OP for now since the PS3 documentation doesn't have enough info to detect this.
	return 0;
}

GSISemaphoreID gsiCreateSemaphore(gsi_i32 theInitialCount, gsi_i32 theMaxCount, char* theName)
{
	// NO OP for now since we don't use it
	return 0;
}

// Waits for -- and signals -- the semaphore
gsi_u32 gsiWaitForSemaphore(GSISemaphoreID theSemaphore, gsi_u32 theTimeoutMs)
{
	// NO OP for now since we don't use it
	return 0;
}

// Allow other objects to access the semaphore
void gsiReleaseSemaphore(GSISemaphoreID theSemaphore, gsi_i32 theReleaseCount)
{
	// NO OP for now since we don't use it
}

void gsiCloseSemaphore(GSISemaphoreID theSemaphore)
{
	// NO OP for now since we don't use it
}


int gsiStartThread(GSThreadFunc func, gsi_u32 theStackSize, void *arg, GSIThreadID * id)
{
	int ret;
	ret = sys_ppu_thread_create(id, func, (uint32_t)arg, 1535, theStackSize, 0, NULL);
	if (ret != CELL_OK)
	{
		gsDebugFormat(GSIDebugCat_Common, GSIDebugType_Misc, GSIDebugLevel_WarmError,
			"Failed to create thread: %d\r\n", ret);
		return -1;
	}

	return 0;	
}

void gsiCancelThread(GSIThreadID id)
{
	// NO OP: not needed
}

void gsiExitThread(GSIThreadID id)
{
	// This should only be called from the thread that was created with the thread id = id
	sys_ppu_thread_t thisThread;

	sys_ppu_thread_get_id(&thisThread);
	if (thisThread == id)
		sys_ppu_thread_exit(0);
}

void gsiCleanupThread(GSIThreadID id)
{
	// NO OP: not needed
}

gsi_u32 gsiInterlockedIncrement(gsi_u32* num)
{
	cellAtomicIncr32(num);
	return *num;
}

gsi_u32 gsiInterlockedDecrement(gsi_u32* num)
{
	cellAtomicDecr32(num);
	return *num;
}

