// Fill out your copyright notice in the Description page of Project Settings.


#include "XEWaveSpawner.h"

// Sets default values
AXEWaveSpawner::AXEWaveSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AXEWaveSpawner::BeginPlay()
{
	Super::BeginPlay();
	FString Message = FString::Printf(TEXT("AXEWaveSpawner BeginPlay."), CurrentWave, WaveInfos.Num());
	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, Message);
	SpawnWave();
}

void AXEWaveSpawner::SpawnWave()
{
	FString Message = FString::Printf(TEXT("SpawnWave: [%d / %d]."), CurrentWave, WaveInfos.Num());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
	if (CurrentWave < WaveInfos.Num())
	{
		auto EnemyNum = EnemyNums[CurrentWave];
		FString Message2 = FString::Printf(TEXT("EnemyNum:%d."), EnemyNum);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message2);
		UWorld* World = GetWorld();
		for (auto i = 0; i < EnemyNum; i++)
		{
			int32 RandomIndex = FMath::RandRange(0, SpawnPositions.Num() - 1);
			FVector Location = SpawnPositions[RandomIndex];
			
			FString Message3 = FString::Printf(TEXT("Spawn Location: (%f, %f, %f)."), Location.X, Location.Y, Location.Z);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message3);

			FRotator Rotation(0.0f, 0.0f, 0.0f);
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			////SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			SpawnParams.Instigator = GetInstigator();
			
			if (AXECharacter* NewEnemy = Cast<AXECharacter>(World->SpawnActor(EnemyClass, &Location, &Rotation, SpawnParams)))
			{
				NewEnemy->SpawnDefaultController();
			}
			else
			{

				FString Message4 = FString::Printf(TEXT("NewEnemy."));
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, Message4);
			}

			//AXECharacter* NewEnemy = World->SpawnActor<AXECharacter>(EnemyClass, Location, Rotation, SpawnParams);
			//AXEAIController* NewController = Cast<AXEAIController>(World->SpawnActor(AXEAIController::StaticClass()));
			//NewController->OnPossess(NewEnemy);
		}
	}
}

void AXEWaveSpawner::CheckAlive()
{
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AXECharacter::StaticClass(), FoundEnemies);

	//FString Message = FString::Printf(TEXT("FoundEnemyNum: %d."), FoundEnemies.Num());
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
	if (FoundEnemies.Num() == 0)
	{
		CurrentWave++;
		SpawnWave();
	}
}

// Called every frame
void AXEWaveSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckAlive();
}

