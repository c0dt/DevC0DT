// Copyright Epic Games, Inc. All Rights Reserved.

#include "DevC0DTEd.h"

#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "FDevC0DTEdModule"

void FDevC0DTEdModule::StartupModule()
{

}

void FDevC0DTEdModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDevC0DTEdModule, DevC0DTEd)