// Copyright c0dt Games, Inc. All Rights Reserved.


#include "AssetsHelperEditorFunctionLibrary.h"

#if WITH_EDITOR
#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetViewUtils.h"
#include "EditorAssetLibrary.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(AssetsHelperEditorFunctionLibrary)


bool UAssetsHelperEditorFunctionLibrary::MoveAsset(const FString& SourcePath, const FString& DestinationPath)
{
    bool bSuccess = false;

    //// Ensure the asset exists
    //if (FPaths::FileExists(SourcePath))
    //{
    //    // Load it to ensure it is in memory
    //    UEditorAssetLibrary::LoadAsset(
    //    UObject* Asset = LoadObject<UObject>(nullptr, *SourcePath);
    //    if (Asset)
    //    {
    //        TArray<UObject*> AssetsToMove;
    //        AssetsToMove.Add(Asset);

    //        // Move the asset
    //        AssetViewUtils::MoveAssets(AssetsToMove, DestinationPath);
    //    }
    //}

    return bSuccess;
}
