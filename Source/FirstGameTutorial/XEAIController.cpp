// Fill out your copyright notice in the Description page of Project Settings.


#include "XEAIController.h"

#include "XECharacter.h"

AXEAIController::AXEAIController()
{
    PrimaryActorTick.bCanEverTick = true;

}

void AXEAIController::BeginPlay()
{
    Super::BeginPlay();
    //MoveToPlayer();
}

void AXEAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void AXEAIController::MoveToPlayer()
{
    AXPCharacter* Player = Cast<AXPCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (Player)
    {
        auto loc = Player->GetActorLocation();
        FString Message2 = FString::Printf(TEXT("MoveToPlayer Loc=(%f, %f, %f)."), loc.X, loc.Y, loc.Z);
        GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, Message2);
        this->MoveToActor(Player, 30.0f);
    }
}


void AXEAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    AXECharacter* MyCharacter = Cast<AXECharacter>(GetPawn());
    if (MyCharacter)
    {
        MyCharacter->PlayMontage();
    }
}

void AXEAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
