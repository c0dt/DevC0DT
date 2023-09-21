// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/WeakInterfacePtr.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Engine/CancellableAsyncAction.h"
#include "Engine/EngineTypes.h"

#include "AsyncAction_EditorQueryAssetData.generated.h"

class AGameStateBase;
class UGameInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAssetListReadyAsyncDelegate, const TArray<FAssetData>&, AssetDataList);

/**
 * Watches for team changes in the specified object
 */
UCLASS()
class UAsyncAction_EditorQueryAssetData : public UCancellableAsyncAction
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true", Keywords = "System"))
	static UAsyncAction_EditorQueryAssetData* ListAssetPackagePathAndName(const UObject* WorldContextObject,
		const FString FolderPath, 
		const bool bRecursivePaths,
		const TArray<FTopLevelAssetPath> ClassPaths);

	//~UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	virtual void SetReadyToDestroy() override;
	//~End of UBlueprintAsyncActionBase interface


	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "OnAssetListReady") )
	FOnAssetListReadyAsyncDelegate K2_OnAssetListReady;

private:
	FString FolderPath;
	TArray<FTopLevelAssetPath> FilterClassPaths;
	bool bFilterRecursivePaths;
};
