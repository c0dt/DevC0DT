// Copyright Epic Games, Inc. All Rights Reserved.

#include "AsyncAction_HttpRequest.h"
#include "Engine/GameInstance.h"
#include "Json.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AsyncAction_HttpRequest)

UAsyncAction_HttpRequest::UAsyncAction_HttpRequest(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UAsyncAction_HttpRequest* UAsyncAction_HttpRequest::Request(const UObject* WorldContextObject, const FString Url, const FString Verb)
{
	UAsyncAction_HttpRequest* Action = nullptr;
	Action = NewObject<UAsyncAction_HttpRequest>();
	Action->Url = Url;
	Action->Verb = Verb;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}

void UAsyncAction_HttpRequest::SetReadyToDestroy()
{
	Super::SetReadyToDestroy();
}

void UAsyncAction_HttpRequest::Activate()
{

	const FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindLambda(
		// Here, we "capture" the 'this' pointer (the "&"), so our lambda can call this
		// class's methods in the callback.
		[&](
			FHttpRequestPtr pRequest,
			FHttpResponsePtr pResponse,
			bool connectedSuccessfully) mutable {

				if (connectedSuccessfully) {
					UE_LOG(LogTemp, Display, TEXT("Response %s"), *pResponse->GetContentAsString());
					K2_OnResponseReceived.Broadcast(true, pResponse->GetContentAsString());
					TArray<FString> Lines;
					pResponse->GetContentAsString().ParseIntoArrayLines(Lines);
					for (int i = 1; i < Lines.Num(); i++)
					{
						UE_LOG(LogTemp, Display, TEXT("[%d]: %s"),i, *Lines[i]);
					}
				}
				else {
					switch (pRequest->GetStatus()) {
					case EHttpRequestStatus::Failed:
						UE_LOG(LogTemp, Error, TEXT("Connection failed."));
					default:
						UE_LOG(LogTemp, Error, TEXT("Request failed."));
					}
					K2_OnResponseReceived.Broadcast(false, FString());
				}
		});
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->ProcessRequest();
}

//void UAsyncAction_HttpRequest::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool ConnectionSuccessfully)
//{
//
//}
