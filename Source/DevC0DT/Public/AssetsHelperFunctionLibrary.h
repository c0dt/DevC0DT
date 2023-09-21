// Copyright c0dt Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AssetsHelperFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DEVC0DT_API UAssetsHelperFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (Keywords = "Assets"))
	static void GetAssetDataPackageName(const FAssetData& AssetData, FString& PackageName);

	UFUNCTION(BlueprintPure, meta = (Keywords = "Assets"))
	static void GetAssetDataPackagePath(const FAssetData& AssetData, FString& PackagePath);
   
};
