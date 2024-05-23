// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITypes.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "XEAIController.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTGAMETUTORIAL_API AXEAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AXEAIController();
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	void MoveToPlayer();
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	virtual void Tick(float DeltaTime) override;
};
