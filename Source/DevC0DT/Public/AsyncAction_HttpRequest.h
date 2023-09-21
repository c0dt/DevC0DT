// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/WeakInterfacePtr.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Engine/CancellableAsyncAction.h"
#include "Engine/EngineTypes.h"

#include "AsyncAction_HttpRequest.generated.h"

class AGameStateBase;
class UGameInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHttpRequestAsyncDelegate, bool, Result, const FString&, Payload);

/**
 * Watches for team changes in the specified object
 */
UCLASS()
class UAsyncAction_HttpRequest : public UCancellableAsyncAction
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true", Keywords = "Http"))
	static UAsyncAction_HttpRequest* Request(const UObject* WorldContextObject, const FString Url, const FString Verb);

	//~UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	virtual void SetReadyToDestroy() override;
	//~End of UBlueprintAsyncActionBase interface
protected:
	//UFUNCTION()
	//void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool ConnectionSuccessfully);
public:
	UPROPERTY(BlueprintAssignable, Meta = (DisplayName = "ResponseReceived") )
	FHttpRequestAsyncDelegate K2_OnResponseReceived;

private:
	FString Url;
	FString Verb;
};
