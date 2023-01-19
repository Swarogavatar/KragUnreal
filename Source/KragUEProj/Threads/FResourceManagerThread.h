#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "HAL/ThreadSafeBool.h"
#include "Core/Public/HAL/CriticalSection.h"
#include "Core/Public/Misc/ScopeLock.h"

class AResourceManager;
class FScopeLock;

class FResourceManagerThread : public FRunnable, FSingleThreadRunnable
{
public:
	FResourceManagerThread(const FTimespan InThreadTickRate, const TCHAR* InThreadName, TObjectPtr<AResourceManager> InManagerActor)
		: ThreadTickRate(InThreadTickRate), bShouldStop(false), ManagerActor(InManagerActor)
	{
		bThreadPaused.AtomicSet(false);
		Thread = FRunnableThread::Create(this, InThreadName, 0, TPri_Normal, FPlatformAffinity::GetPoolThreadMask());
	}

	virtual ~FResourceManagerThread() override;
	virtual bool Init() override;
	virtual void Stop() override;
	virtual uint32 Run() override;
	virtual void SetPaused(const bool InPaused);
	virtual void UpdateResources();
	virtual void Tick() override;
	virtual FSingleThreadRunnable* GetSingleThreadInterface() override {return this;}

	void OnPausePIE(const bool bIsSimulating);
	void OnResumePIE(const bool bIsSimulating);
	virtual bool ShouldUnpause() const;
	
	bool HasStopped() { return bThreadStopped; }
	bool IsPaused() { return bThreadPaused; }
protected:
	FThreadSafeBool bThreadStopped;

	FThreadSafeBool bThreadPaused = false;
	FCriticalSection GeneratorsLock;

	FTimespan ThreadTickRate;
	
	bool bShouldStop = false;
	TObjectPtr<AResourceManager> ManagerActor;

	FRunnableThread* Thread;
	
};
