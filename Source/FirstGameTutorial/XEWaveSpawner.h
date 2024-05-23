// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XECharacter.h"
#include "Kismet/GameplayStatics.h"

#include "XEAIController.h"

#include "XEWaveSpawner.generated.h"

UCLASS()
class FIRSTGAMETUTORIAL_API AXEWaveSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXEWaveSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void SpawnWave();

	UFUNCTION(BlueprintCallable)
	void CheckAlive();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = "Wave Spawner")
	TArray<FVector> SpawnPositions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave Spawner")
	TArray<FString> WaveInfos;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave Spawner")
	TArray<int32> EnemyNums;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave Spawner")
	int32 CurrentWave;

	UPROPERTY(EditDefaultsOnly, Category = "Wave Spawner")
	TSubclassOf<AXECharacter> EnemyClass;

};
