// Fill out your copyright notice in the Description page of Project Settings.


#include "XunFeiIATWorkerThread.h"

#include "XunFeiIAT.h"

#include "WinAPI/iat_online_record_func.h"

FXunFeiIATWorkerThread::FXunFeiIATWorkerThread(AXunFeiIAT* inXunFeiIAT, FXunFeiIATParam inIATParam)
{
	IATParam = inIATParam;
	XunFeiIAT = inXunFeiIAT;
	FRunnableThread::Create(this,TEXT("XunFeiIATThread"));
}

bool FXunFeiIATWorkerThread::Init()
{
	int			ret						=	MSP_SUCCESS;
	int			upload_on				=	IATParam.upload_on; //是否上传用户词表
	
	// const char* login_params			=	"appid = 5c4ae579, work_dir = ."; // 登录参数，appid与msc库绑定,请勿随意改动
	FString login_params = TEXT("appid = ") + IATParam.appid + TEXT(", word_dir = .");
	/*
		* sub:				请求业务类型
		* domain:			领域
		* language:			语言
		* accent:			方言
		* sample_rate:		音频采样率
		* result_type:		识别结果格式
		* result_encoding:	结果编码格式
		*
		*/
	session_begin_params	=	"sub = iat, domain = iat, language = zh_cn, accent = mandarin, sample_rate = 16000, result_type = json, result_encoding = UTF-8";
	
	/* 用户登录 */
	ret = MSPLogin(NULL, NULL, TCHAR_TO_UTF8(*login_params)); //第一个参数是用户名，第二个参数是密码，均传NULL即可，第三个参数是登录参数	
	if (MSP_SUCCESS != ret)	{
		UE_LOG(LogTemp, Warning, TEXT("[XunFeiIATWorkerThread->Init] MSPLogin failed , Error code %d"),ret);
		AsyncTask(ENamedThreads::GameThread,[this]()
		{
			XunFeiIAT->Native_OnLoginResult(false);
		});
		return false;
	}

	AsyncTask(ENamedThreads::GameThread,[this]()
	{
		XunFeiIAT->Native_OnLoginResult(true);
	});
	
	if (upload_on)
	{
		UE_LOG(LogTemp, Warning, TEXT("[XunFeiIATWorkerThread->Init] 上传用户词表... "));
		ret = upload_userwords();
		if (MSP_SUCCESS != ret)
		{
			AsyncTask(ENamedThreads::GameThread,[this]()
			{
				XunFeiIAT->Native_OnUploadUserWordsFailed();
			});
			return false;
		}
		UE_LOG(LogTemp, Warning, TEXT("[XunFeiIATWorkerThread->Init] 上传用户词表成功"));
	}

	
	
	return true;
}

uint32 FXunFeiIATWorkerThread::Run()
{
	// 默认只从mic输入即可
	demo_mic(session_begin_params);

	//iflytek02音频内容为“中美数控”；如果上传了用户词表，识别结果为：“中美速控”。;
	// demo_file("wav/iflytek02.wav", session_begin_params);
	return 1;
}

void FXunFeiIATWorkerThread::Stop()
{
}

void FXunFeiIATWorkerThread::Exit()
{
	UE_LOG(LogTemp, Warning, TEXT("[XunFeiIATWorkerThread->Exit] MSPLogout "));
	MSPLogout(); //退出登录
}

FXunFeiIATWorkerThread::~FXunFeiIATWorkerThread()
{
	UE_LOG(LogTemp, Warning, TEXT("[XunFeiIATWorkerThread->~FXunFeiIATWorkerThread]"));
	if (RunnableThread)
	{
		RunnableThread->Kill();
		delete RunnableThread;
		RunnableThread = nullptr;
	}
}
