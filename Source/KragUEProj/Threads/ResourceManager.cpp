// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceManager.h"
#include "BaseResourceGenerator.h"
#include "Editor.h"
#include "Editor/EditorEngine.h"
#include "FResourceManagerThread.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AResourceManager::AResourceManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AResourceManager::UpdateResources()
{
	if (ResourceGenerators.IsEmpty())
	{
		return;
	}
	
	while (PrimeNumbersCalculated < 20000 - 2)
	{
		// if (CalcThread && GetWorld()->IsPaused() && !CalcThread->IsPaused())
		// {
		// 	CalcThread->SetPaused(true);
		// 	return;
		// }
		if (CalcThread && CalcThread->IsPaused())
		{
			return;
		}
		bool NumIsPrime = false;
		while (! NumIsPrime)
		{
	#if WITH_EDITOR
  		// 		if (CalcThread && GetWorld()->IsPaused() && !CalcThread->IsPaused())
				// {
				// 	CalcThread->SetPaused(true);
				// }
				// else if(CalcThread && !GetWorld()->IsPaused() && CalcThread->IsPaused())
				// {
				// 	CalcThread->SetPaused(false);
				// }
	#endif

			if (CalcThread && CalcThread->IsPaused())
			{
				return;
			}
			NumIsPrime = true;

			//Try Next Number
			CalculatedPrime++;

			//Modulus from 2 to current number - 1 
			for (int32 b = 2; b < CalculatedPrime; ++b)
			{
				// if (CalcThread && GetWorld()->IsPaused() && !CalcThread->IsPaused())
				// {
				// 	CalcThread->SetPaused(true);
				// }
				// else if(CalcThread && !GetWorld()->IsPaused() && CalcThread->IsPaused())
				// {
				// 	CalcThread->SetPaused(false);
				// }
				if (CalcThread && CalcThread->IsPaused())
				{
					return;
				}

				// FPlatformProcess::Sleep(0.01);

				if (CalculatedPrime % b == 0)
				{
					NumIsPrime = false;
					break;
				}
			}
		}
		++PrimeNumbersCalculated;
		// UE_LOG(LogTemp, Warning, TEXT("%d is prime" ), CalculatedPrime);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("%d is prime"), CalculatedPrime));

	}
		// ParallelFor(ResourceGenerators.Num(),
		// 	[&](int32 Index)
		// 	{ ResourceGenerators[Index]->AddResources(100); });

		FScopeLock Lock(&GeneratorsLock);
		for (auto Generator : ResourceGenerators)
		{
			Generator->AddResources(100);
		}
	PrimeNumbersCalculated = 0;
	CalculatedPrime = 3;
}

void AResourceManager::PauseCalcThread()
{
	if (CalcThread && !CalcThread->IsPaused())
	{
		CalcThread->SetPaused(true);
	}
}

void AResourceManager::UnPauseCalcThread()
{
	if (CalcThread && CalcThread->IsPaused())
	{
		CalcThread->SetPaused(false);
	}
}

void AResourceManager::AddGenerator(ABaseResourceGenerator* Generator)
{
	if (Generator)
	{
		FScopeLock Lock(&GeneratorsLock);
		ResourceGenerators.AddUnique(Generator);
	}
}

// Called when the game starts or when spawned
void AResourceManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseResourceGenerator::StaticClass(), OutActors);

	for (AActor* OutActor : OutActors)
	{
		ResourceGenerators.AddUnique(Cast<ABaseResourceGenerator>(OutActor));
	}

	UE_LOG(LogTemp, Error, TEXT("ResourceGenerators array has %d elements"), ResourceGenerators.Num());
	
	const FTimespan ThreadWaitTime = FTimespan::FromSeconds(0.1);
	const FString CalcThreadName = "ResourceManagerThread";
	CalcThread = new FResourceManagerThread(ThreadWaitTime, *CalcThreadName, this);

	if (CalcThread)
	{
		CalcThread->Init();
	}

#if WITH_EDITOR
 	OnPausePIEEvent = FEditorDelegates::FOnPIEEvent::FDelegate::CreateUObject(this, &AResourceManager::HandlePausePIEEvent);
	FEditorDelegates::PausePIE.Add(OnPausePIEEvent);
		OnResumePIEEvent = FEditorDelegates::FOnPIEEvent::FDelegate::CreateUObject(this, &AResourceManager::HandleUnPausePIEEvent);
	FEditorDelegates::ResumePIE.Add(OnResumePIEEvent);
#endif
}

// Called every frame
void AResourceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AResourceManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(CalcThread) 
	{
		CalcThread->Stop();
		
		// Wait here until JoyThread is verified as having stopped
		while(!CalcThread->HasStopped())
		{
			FPlatformProcess::Sleep(0.1);
		}
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		
		//Do this last
		delete CalcThread;
	}

	FScopeLock Lock(&GeneratorsLock);
	ResourceGenerators.Empty();

	
	Super::EndPlay(EndPlayReason);
}

