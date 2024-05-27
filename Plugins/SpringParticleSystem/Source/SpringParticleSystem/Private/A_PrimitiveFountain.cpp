// Fill out your copyright notice in the Description page of Project Settings.


#include "A_PrimitiveFountain.h"

// Sets default values
AA_PrimitiveFountain::AA_PrimitiveFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Spawned = 0;

}

// Called when the game starts or when spawned
void AA_PrimitiveFountain::BeginPlay()
{
	Super::BeginPlay();
	R = InitialVelocity.Length();
	Theta = FMath::Acos(InitialVelocity.Z / R);
	Phi = FMath::Atan2(InitialVelocity.Y, InitialVelocity.X);
}

// Called every frame
void AA_PrimitiveFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Spawned < ParticleNum)
	{
		EmitParticles(ParticleNum - Spawned);
		Spawned += SpawnRate;
		return;
	}
}

void AA_PrimitiveFountain::EmitParticles(int32 SpawnNum)
{
	auto World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		FVector Location = GetActorLocation();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		for (auto i = 0; i < SpawnNum; i++)
		{
			AParticleActor* ParticleActor = World->SpawnActor<AParticleActor>(AParticleActor::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);
			if (ParticleActor)
			{
				//FString Message = FString::Printf(TEXT("Spawn Particle."));
				//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, Message);
				float NewTheta = DeltaTheta.X + (DeltaTheta.Y - DeltaTheta.X) * FMath::SRand() + Theta;
				float NewPhi = DeltaPhi.X + (DeltaPhi.Y - DeltaPhi.X) * FMath::SRand() + Phi;
				FVector Velocity = {R * FMath::Sin(NewTheta) * FMath::Cos(NewPhi), R * FMath::Sin(NewTheta) * FMath::Sin(NewPhi), R * FMath::Cos(NewTheta)};
				int32 RandomIndex = FMath::RandRange(0, Meshes.Num() - 1);
				ParticleActor->Initialize(Meshes[RandomIndex], Velocity, ParticleLifetime);
				Particles.Add(ParticleActor);
			}
		}
	}
}

