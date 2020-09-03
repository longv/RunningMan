// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomGuy_GameMode.h"
#include "GameFramework/Actor.h"

ARandomGuy_GameMode::ARandomGuy_GameMode() 
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARandomGuy_GameMode::BeginPlay() 
{
	Super::BeginPlay();

	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(
		UnusedHandle, this, &ARandomGuy_GameMode::SpawnPlayerRecharge, FMath::RandRange(1, 2), true
	);
}

void ARandomGuy_GameMode::Tick(float DeltaTime) 
{

}

void ARandomGuy_GameMode::SpawnPlayerRecharge()
{
	float RandX = FMath::RandRange(Spawn_X_Min, Spawn_X_Max);
	float RandY = FMath::RandRange(Spawn_Y_Min, Spawn_Y_Max);

	FVector SpawnPosition = FVector(RandX, RandY, Spawn_Z);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	GetWorld()->SpawnActor(PlayerRecharge, &SpawnPosition);
}
