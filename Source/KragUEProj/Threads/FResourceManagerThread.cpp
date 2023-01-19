#include "FResourceManagerThread.h"

#include "Editor.h"
#include "ResourceManager.h"

FResourceManagerThread::~FResourceManagerThread()
{
	if (Thread)
	{
		Thread->Kill(true);
		delete Thread;
	}
}

bool FResourceManagerThread::Init()
{
	return FRunnable::Init();
}

void FResourceManagerThread::Stop()
{
	bThreadPaused.AtomicSet(true);
	bShouldStop = true;


	FRunnable::Stop();
}

uint32 FResourceManagerThread::Run()
{
	// FEditorDelegates::PausePIE.AddRaw(this, &FResourceManagerThread::OnPausePIE);
	// FEditorDelegates::ResumePIE.AddRaw(this, &FResourceManagerThread::OnResumePIE);
	bThreadStopped.AtomicSet(false);
	while (!bShouldStop)
	{
		if (bThreadPaused)
		{
			if (ShouldUnpause())
			{
				SetPaused(false);
				continue;
			}
			FPlatformProcess::Sleep(ThreadTickRate.GetTotalSeconds());
			continue;
		}
			UpdateResources();
	}
	bThreadStopped.AtomicSet(true);
	return 0;
}

void FResourceManagerThread::SetPaused(const bool InPaused)
{
	bThreadPaused.AtomicSet(InPaused);
}

void FResourceManagerThread::UpdateResources()
{
	if (ManagerActor)
	{
		ManagerActor->UpdateResources();
	}
}

void FResourceManagerThread::Tick()
{
	bThreadStopped.AtomicSet(false);
	while (!bShouldStop)
	{
		if (bThreadPaused)
		{
			if (ShouldUnpause())
			{
				SetPaused(false);
				continue;
			}
			FPlatformProcess::Sleep(ThreadTickRate.GetTotalSeconds());
			continue;
		}
		UpdateResources();
	}
	bThreadStopped.AtomicSet(true);
}

void FResourceManagerThread::OnPausePIE(const bool bIsSimulating)
{
	SetPaused(true);
}

void FResourceManagerThread::OnResumePIE(const bool bIsSimulating)
{
	SetPaused(false);
}

bool FResourceManagerThread::ShouldUnpause() const
{
	if(ManagerActor)
	{
		return ManagerActor->ShouldUnpauseThread();
	}
	return false;
}
