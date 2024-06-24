// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "XunFeiIATType.h"
class AXunFeiIAT;
/**
 * 
 */
class FXunFeiIATWorkerThread : public FRunnable
{
	FXunFeiIATParam IATParam;
	AXunFeiIAT* XunFeiIAT = nullptr;
	FRunnableThread* RunnableThread = nullptr;
	char* session_begin_params = nullptr;
public:
	FXunFeiIATWorkerThread(AXunFeiIAT* inXunFeiIAT , FXunFeiIATParam inIATParam );
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	virtual ~FXunFeiIATWorkerThread() override;
};
