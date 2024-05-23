// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "XEAIController.h"
#include "NavigationSystem.h"
#include "Components/ArrowComponent.h"
#include "InputAction.h"
#include "XPCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "XECharacter.generated.h"


UCLASS()
class FIRSTGAMETUTORIAL_API AXECharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AXECharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* AttackDir;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//// Function to initiate chasing the player
	//void ChasePlayer();

	UPROPERTY(EditAnywhere)
	UAnimMontage* MontageToPlay = NULL;

	// Health of the AI
	UPROPERTY(EditAnywhere)
	float MaxHealth = 200.0f;
	float Health;
	bool CanAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyMeshes")
	TArray<USkeletalMesh*> Meshes;

	UFUNCTION()
	void OnMontageCompleted(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	void PlayMontage();
};
