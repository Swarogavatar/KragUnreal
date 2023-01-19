// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor.h"
#include "GameFramework/Actor.h"
#include "HAL/CriticalSection.h"
#include "ResourceManager.generated.h"

class ABaseResourceGenerator;
class FResourceManagerThread;



UCLASS()
class KRAGUEPROJ_API AResourceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResourceManager();
	void UpdateResources();

	void PauseCalcThread();
	void UnPauseCalcThread();
	
	bool ShouldUnpauseThread() const { return !GetWorld()->IsPaused();}
	UFUNCTION(BlueprintCallable)
	void AddGenerator(ABaseResourceGenerator* Generator);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class FResourceManagerThread* CalcThread = nullptr;

#if WITH_EDITOR
  FEditorDelegates::FOnPIEEvent::FDelegate OnPausePIEEvent;
	FEditorDelegates::FOnPIEEvent::FDelegate OnResumePIEEvent;
#endif


	void HandlePausePIEEvent(bool bIsSimulating)
	{
		PauseCalcThread();
	}

	void HandleUnPausePIEEvent(bool bIsSimulating)
	{
		UnPauseCalcThread();
	}
	FCriticalSection GeneratorsLock;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<ABaseResourceGenerator*> ResourceGenerators;
	
	int CalculatedPrime = 3;
	int PrimeNumbersCalculated =0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
