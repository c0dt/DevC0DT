// Copyright Epic Games, Inc. All Rights Reserved.

#include "DevC0DT.h"

#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "FDevC0DTModule"

void RegisterC0DTStatusWidgetWithToolMenu();

void FDevC0DTModule::StartupModule()
{
	RegisterC0DTStatusWidgetWithToolMenu();
}

void FDevC0DTModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDevC0DTModule, DevC0DT)