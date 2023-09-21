// Copyright c0dt Games, Inc. All Rights Reserved.


#include "AssetsHelperFunctionLibrary.h"

#if WITH_EDITOR
#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(AssetsHelperFunctionLibrary)


void UAssetsHelperFunctionLibrary::GetAssetDataPackageName(const FAssetData& AssetData, FString& PackageName)
{
    PackageName = AssetData.PackageName.ToString();
}

void UAssetsHelperFunctionLibrary::GetAssetDataPackagePath(const FAssetData& AssetData, FString& PackagePath)
{
    PackagePath = AssetData.PackagePath.ToString();
}