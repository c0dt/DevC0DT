// Copyright Epic Games, Inc. All Rights Reserved.

#include "AsyncAction_EditorQueryAssetData.h"
#include "Engine/GameInstance.h"

#if WITH_EDITOR
#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(AsyncAction_EditorQueryAssetData)

UAsyncAction_EditorQueryAssetData::UAsyncAction_EditorQueryAssetData(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

UAsyncAction_EditorQueryAssetData* UAsyncAction_EditorQueryAssetData::ListAssetPackagePathAndName(const UObject* WorldContextObject,
    const FString FolderPath, 
    const bool bRecursivePaths,
    const TArray<FTopLevelAssetPath> ClassPaths)
{
    UAsyncAction_EditorQueryAssetData* Action = nullptr;
    Action = NewObject<UAsyncAction_EditorQueryAssetData>();
    Action->FolderPath = FolderPath;
    Action->bFilterRecursivePaths = bRecursivePaths;
    Action->FilterClassPaths = ClassPaths;
    Action->RegisterWithGameInstance(WorldContextObject);

    return Action;
}

void UAsyncAction_EditorQueryAssetData::SetReadyToDestroy()
{
    Super::SetReadyToDestroy();
}

void UAsyncAction_EditorQueryAssetData::Activate()
{
    // Form the filter
    FARFilter Filter;
    Filter.PackagePaths.Add(*FolderPath);
    Filter.ClassPaths = FilterClassPaths;
    Filter.bRecursivePaths = bFilterRecursivePaths;
    TArray<FAssetData> AssetList;
    IAssetRegistry& AssetRegistry = IAssetRegistry::GetChecked();
    AssetRegistry.GetAssets(Filter, AssetList);
    // Run the task asynchronously
    //AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [=]()
    //{

    //    // Fire the delegate on the main thread.
    //    AsyncTask(ENamedThreads::GameThread, [=]()
    //    {
    K2_OnAssetListReady.Broadcast(AssetList);
    SetReadyToDestroy();
    //    });
    //});


}
