// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XunFeiIATType.h"

#include "GameFramework/Actor.h"
#include "XunFeiIAT.generated.h"

UCLASS()
class XUNFEISPEECHTOTEXT_API AXunFeiIAT : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXunFeiIAT();
	virtual void BeginDestroy() override;

	
	//
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FXunFeiIATParam IATParam;


	// 初始化
	UFUNCTION(BlueprintCallable)
	void BP_InitXunFeiIAT();

	
	// 登录失败
	void Native_OnLoginResult(bool bSuccess);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnLoginOnLoginResult(bool bSuccess);

	// 上传用户词表失败
	void Native_OnUploadUserWordsFailed();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnUploadUserWordsFailed();

	//
	UFUNCTION(BlueprintCallable)
	void BP_StartRecord();
	UFUNCTION(BlueprintCallable)
	void BP_StopRecord();
	UFUNCTION(BlueprintCallable)
	void BP_QuitRecord();

	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnResult(const FString & Result);

	UFUNCTION(BlueprintCallable)
	FString BP_ConvertJsonResultToText();
	
private:
	class FXunFeiIATWorkerThread* xunFeiWorkerThread = nullptr;
	FTimerHandle timerCheckFinish;
	FString cacheResult;
};
