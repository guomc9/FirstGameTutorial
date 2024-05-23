// Fill out your copyright notice in the Description page of Project Settings.


#include "XPCharacter.h"

// Sets default values
AXPCharacter::AXPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	PlayerMesh->SetupAttachment(GetCapsuleComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);

	AttackDir = CreateDefaultSubobject<UArrowComponent>(TEXT("AttackDir"));
	AttackDir->SetupAttachment(GetCapsuleComponent());

	Health = MaxHealth;
	CanAttack = true;
}

// Called when the game starts or when spawned
void AXPCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	if (WBHealthBarClass != nullptr)
	{
		WBHealthBarInstance = CreateWidget<UUserWidget>(GetWorld(), WBHealthBarClass);
		if (WBHealthBarInstance != nullptr)
		{
			WBHealthBarInstance->AddToViewport();
		}
	}

	if (WBHealthBarClass != nullptr)
	{
		WBHUDClassInstance = CreateWidget<UUserWidget>(GetWorld(), WBHUDClass);
		if (WBHUDClassInstance != nullptr)
		{
			WBHUDClassInstance->AddToViewport();
		}
	}


	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//if (AnimInstance)
	//{
	//	FString Message = FString::Printf(TEXT("AddDynamic."));
	//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
	//	AnimInstance->OnMontageEnded.AddDynamic(this, &AXPCharacter::OnMontageCompleted);
	//	AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AXPCharacter::OnMontageNotifyBegin);
	//}
}

// Called every frame
void AXPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AXPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerController* PlayerController = CastChecked<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (!InputMappingContext.IsNull())
			{
				InputSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (Input && MoveAction && LookAction && AttackAction)
	{
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AXPCharacter::Look);
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AXPCharacter::Move);
		Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AXPCharacter::Attack);
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		//FString Message = FString::Printf(TEXT("AddDynamic."));
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);

		//FScriptDelegate OnMontageCompletedDelegate;
		//OnMontageCompletedDelegate.BindUFunction(this, FName("OnMontageCompleted"));
		//AnimInstance->OnMontageEnded.Add(OnMontageCompletedDelegate);

		//FScriptDelegate OnPlayMontageNotifyBeginDelegate;
		//OnPlayMontageNotifyBeginDelegate.BindUFunction(this, FName("OnPlayMontageNotifyBegin"));
		//AnimInstance->OnPlayMontageNotifyBegin.Add(OnPlayMontageNotifyBeginDelegate);

		AnimInstance->OnMontageEnded.AddDynamic(this, &AXPCharacter::OnMontageCompleted);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AXPCharacter::OnMontageNotifyBegin);
	}
}

float AXPCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health = Health - DamageAmount;
	if (Health <= 0)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
		GetMesh()->SetSimulatePhysics(true);
	}
	else
	{
		FVector Forward = GetActorForwardVector();
		FVector Up = GetActorUpVector();
		this->LaunchCharacter(-100.0f * Forward + 200.0f * Up, false, false);
	}
	return 0.0f;
}

void AXPCharacter::Look(const FInputActionInstance & Instance)
{
	FVector2D Value = Instance.GetValue().Get<FVector2D>();
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(-1.0f * Value.Y);
}

void AXPCharacter::Move(const FInputActionInstance& Instance)
{
	FVector2D Value = Instance.GetValue().Get<FVector2D>();
	FRotator Rot = GetControlRotation();
	FRotator LocalRot = FRotator(0, Rot.Yaw, Rot.Roll);
	FMatrix RotationMatrix = FRotationMatrix(LocalRot);
	FVector RightVector = RotationMatrix.GetUnitAxis(EAxis::Y);
	FVector ForwardVector = RotationMatrix.GetUnitAxis(EAxis::X);
	
	AddMovementInput(ForwardVector, Value.Y);
	AddMovementInput(RightVector, Value.X);
}

void AXPCharacter::Attack(const FInputActionInstance& Instance)
{
	//FString Message = FString::Printf(TEXT("Attack."));
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
	if (CanAttack)
	{
		//Message = FString::Printf(TEXT("CanAttack."));
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			CanAttack = false;
			AnimInstance->Montage_Play(MontageToPlay, 1.0f);
			//Message = FString::Printf(TEXT("Play Montage."));
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);
		}
	}

}

void AXPCharacter::OnMontageCompleted(UAnimMontage* Montage, bool bInterrupted)
{
	CanAttack = true;
}

void AXPCharacter::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	//FString Message = FString::Printf(TEXT("OnMontageNotifyBegin."));
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Message);

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
