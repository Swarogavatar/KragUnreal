// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseResourceGenerator.generated.h"

UCLASS()
class KRAGUEPROJ_API ABaseResourceGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseResourceGenerator();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ResourceDisplay;
	
	void AddResources(const int DeltaResources);

	UFUNCTION(BlueprintCallable)
	virtual int TakeOutResources();

	void UpdateDisplay();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FCriticalSection CurrentResourcesLock;
	int CurrentResources =0;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
