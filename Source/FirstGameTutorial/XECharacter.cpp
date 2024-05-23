// Fill out your copyright notice in the Description page of Project Settings.


#include "XECharacter.h"

// Sets default values
AXECharacter::AXECharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    CanAttack = true;
    Health = MaxHealth;
    AIControllerClass = AXEAIController::StaticClass();
}

// Called when the game starts or when spawned
void AXECharacter::BeginPlay()
{
	Super::BeginPlay();
    FString Message = FString::Printf(TEXT("BeginPlay AXECharacter."));
    GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, Message);
    Health = MaxHealth;
    int32 RandomIndex = FMath::RandRange(0, Meshes.Num() - 1);
    GetMesh()->SetSkeletalMeshAsset(Meshes[RandomIndex]);
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        AnimInstance->OnMontageEnded.AddDynamic(this, &AXECharacter::OnMontageCompleted);
        AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AXECharacter::OnMontageNotifyBegin);
    }
  
    //ChasePlayer();
}

// Called every frame
void AXECharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AXECharacter::OnMontageCompleted(UAnimMontage* Montage, bool bInterrupted)
{
    CanAttack = true;
}

void AXECharacter::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
    FVector Location = AttackDir->GetComponentLocation();
    FVector Forward = AttackDir->GetForwardVector();
    FVector Start = Location;
    FVector End = Location + 100.0f * Forward;
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    float Radius = 30.0f;
    FHitResult HitResult;

    bool bDrawDebug = true;
    FLinearColor TraceColor = bDrawDebug ? FLinearColor::Red : FLinearColor::Transparent;
    FLinearColor TraceHitColor = bDrawDebug ? FLinearColor::Green : FLinearColor::Transparent;
    float DrawTime = 5.0f;
    bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(this, Start, End, Radius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true, TraceColor, TraceHitColor, DrawTime);
    float Damage = 10.0f;
    if (bHit)
    {
        UGameplayStatics::ApplyDamage(
            HitResult.GetActor(),
            Damage,
            NULL,
            this,
            UDamageType::StaticClass()
        );
    }
}

void AXECharacter::PlayMontage()
{
    GetMesh()->GetAnimInstance()->Montage_Play(MontageToPlay, 1.f);
}

// Called to bind functionality to input
void AXECharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AXECharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Health = Health - DamageAmount;
    FString Message = FString::Printf(TEXT("Enemy Health=%f."), Health);
   	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Message);
    if (Health <= 0)
    {
        APlayerController* PlayerController = Cast<APlayerController>(GetController());
        DisableInput(PlayerController);
        GetMesh()->SetSimulatePhysics(true);
        Destroy();
    }
    else
    {
        FVector Forward = GetActorForwardVector();
        FVector Up = GetActorUpVector();
        this->LaunchCharacter(-100.0f * Forward + 200.0f * Up, false, false);
    }
    return 0.0f;
}

