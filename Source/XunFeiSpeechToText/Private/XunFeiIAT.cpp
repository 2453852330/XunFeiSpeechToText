// Fill out your copyright notice in the Description page of Project Settings.


#include "XunFeiIAT.h"

#include "XunFeiIATWorkerThread.h"

#include "WinAPI/iat_online_record_func.h"

// Sets default values
AXunFeiIAT::AXunFeiIAT()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AXunFeiIAT::BeginDestroy()
{
	BP_QuitRecord();
	if (xunFeiWorkerThread)
	{
		delete xunFeiWorkerThread;
	}
	Super::BeginDestroy();
}

void AXunFeiIAT::BP_InitXunFeiIAT()
{
	new FXunFeiIATWorkerThread(this,IATParam);
}

void AXunFeiIAT::Native_OnLoginResult(bool bSuccess)
{
	UE_LOG(LogTemp, Warning, TEXT("[XunFeiIAT->Native_OnLoginResult] %s"),bSuccess?TEXT("TRUE"):TEXT("FALSE"));
	BP_OnLoginOnLoginResult(bSuccess);
}

void AXunFeiIAT::Native_OnUploadUserWordsFailed()
{
	UE_LOG(LogTemp, Warning, TEXT("[XunFeiIAT->Native_OnUploadUserWordsFailed]"));
	BP_OnUploadUserWordsFailed();
}

void AXunFeiIAT::BP_StartRecord()
{
	UE_LOG(LogTemp, Warning, TEXT("[XunFeiIAT->BP_StartRecord]"));
	SetEvent(events[EVT_START]);
}

void AXunFeiIAT::BP_StopRecord()
{
	SetEvent(events[EVT_STOP]);
	// 立刻接收会导致 on_result 回调每调用,导致 g_result 值为空
	// sleep(0.2);
	GetWorldTimerManager().SetTimer(timerCheckFinish,[this]()
	{
		if (is_convert_finish_ue)
		{
			cacheResult = UTF8_TO_TCHAR(g_result);
			UE_LOG(LogTemp, Warning, TEXT("[XunFeiIAT->OnResult] %s "),*cacheResult);
			BP_OnResult(cacheResult);
			// stop timer
			GetWorldTimerManager().ClearTimer(timerCheckFinish);
			timerCheckFinish.Invalidate();
		}
	},0.1f,true);
}

void AXunFeiIAT::BP_QuitRecord()
{
	UE_LOG(LogTemp, Warning, TEXT("[XunFeiIAT->BP_QuitRecord]"));
	SetEvent(events[EVT_QUIT]);
	PostQuitMessage(0);
}

FString AXunFeiIAT::BP_ConvertJsonResultToText()
{
	TSharedPtr<FJsonObject> JsonRoot;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(cacheResult);
	FString TmpResult;
	if (FJsonSerializer::Deserialize(JsonReader, JsonRoot))
	{
		for (TSharedPtr<FJsonValue, ESPMode::NotThreadSafe> wsObj : JsonRoot->GetArrayField(TEXT("ws")))
		{
			for (TSharedPtr<FJsonValue, ESPMode::NotThreadSafe> cwObj : wsObj->AsObject()->GetArrayField(TEXT("cw")))
			{
				TmpResult += cwObj->AsObject()->GetStringField(TEXT("w"));
			} 
		} 
	}
	UE_LOG(LogTemp, Warning, TEXT("[XunFeiIAT->BP_ConvertJsonResultToText]%s"),*TmpResult);
	return TmpResult;
}

