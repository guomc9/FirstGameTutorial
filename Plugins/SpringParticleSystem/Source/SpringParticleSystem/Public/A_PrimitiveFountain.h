// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParticleActor.h"
#include "A_PrimitiveFountain.generated.h"

UCLASS()
class SPRINGPARTICLESYSTEM_API AA_PrimitiveFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_PrimitiveFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TArray<AParticleActor*> Particles;

	UPROPERTY(EditAnywhere)
	int32 ParticleNum;

	UPROPERTY(EditAnywhere)
	int32 SpawnRate;

	UPROPERTY(EditAnywhere)
	FVector InitialVelocity;
	float R;
	float Phi;
	float Theta;

	UPROPERTY(EditAnywhere)
	FVector2D DeltaTheta;

	UPROPERTY(EditAnywhere)
	FVector2D DeltaPhi;

	UPROPERTY(EditAnywhere)
	float ParticleLifetime;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> Meshes;

	void EmitParticles(int32 SpawnNum);

	int32 Spawned = 0;
};
