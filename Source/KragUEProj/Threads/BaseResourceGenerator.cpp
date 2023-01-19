// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseResourceGenerator.h"

#include "Components/TextRenderComponent.h"


// Sets default values
ABaseResourceGenerator::ABaseResourceGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ResourceDisplay = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ResourceDisplay"));
	ResourceDisplay->SetupAttachment(RootComponent);
}

void ABaseResourceGenerator::AddResources(const int DeltaResources)
{
	{
		FScopeLock Lock(&CurrentResourcesLock);
		CurrentResources +=DeltaResources;
	}
	UpdateDisplay();
}

int ABaseResourceGenerator::TakeOutResources()
{
	FScopeLock Lock(&CurrentResourcesLock);
	const int OutResources = CurrentResources;
	CurrentResources = 0;
	Lock.Unlock();
	UpdateDisplay();
	return OutResources;
}

void ABaseResourceGenerator::UpdateDisplay()
{
	FScopeLock Lock(&CurrentResourcesLock);
	ResourceDisplay->SetText(FText::AsNumber(CurrentResources));
}

// Called when the game starts or when spawned
void ABaseResourceGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseResourceGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

