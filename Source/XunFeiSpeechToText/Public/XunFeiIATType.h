// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XunFeiIATType.generated.h"


USTRUCT(BlueprintType)
struct FXunFeiIATParam
{
	GENERATED_BODY()

	// 用户的 appid
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString appid = TEXT("5c4ae579");
	// 是否上传用户词表
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool upload_on = false;
};
