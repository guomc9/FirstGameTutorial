// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleActor.h"


AParticleActor::AParticleActor()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	Alive = false;
	LiveTime = 0.0f;
	ElapsedTime = 0.0f;
	Velocity = FVector::ZeroVector;
}

void AParticleActor::Initialize(UStaticMesh* ParticleMesh, FVector ParticleVelocity, float ParticleLivetime)
{
	MeshComponent->SetStaticMesh(ParticleMesh);
	MeshComponent->SetWorldScale3D(FVector(0.02f, 0.02f, 0.02f));
	Alive = true;
	LiveTime = ParticleLivetime;
	ElapsedTime = 0.0f;
	Velocity = ParticleVelocity;
	//FString Message = FString::Printf(TEXT("Particle LiveTime=%f, ElapsedTime=%f."), LiveTime, ElapsedTime);
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, Message);
}

// Called when the game starts or when spawned
void AParticleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Alive)
	{
		Velocity = Velocity - FVector(0, 0, 10.0f) * DeltaTime;
		FVector NewLocation = GetActorLocation() + DeltaTime * Velocity;
		//FString Message = FString::Printf(TEXT("Particle Location=(%f, %f, %f)."), NewLocation.X, NewLocation.Y, NewLocation.Z);
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, Message);
		SetActorLocation(NewLocation);
		ElapsedTime += DeltaTime;
		if (ElapsedTime > LiveTime)
		{
			Alive = false;
			//MeshComponent->SetVisibility(false);
			Destroy();
		}
	}
}

