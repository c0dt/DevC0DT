// Copyright c0dt Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AssetsHelperEditorFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DEVC0DTED_API UAssetsHelperEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Assets")
	static bool MoveAsset(const FString& SourcePath, const FString& DestinationPath);
   
};
