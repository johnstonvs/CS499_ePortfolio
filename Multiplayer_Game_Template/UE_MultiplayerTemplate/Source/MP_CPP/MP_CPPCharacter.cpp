// Copyright Epic Games, Inc. All Rights Reserved.

#include "MP_CPPCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Actors/MP_Actor.h"
#include "Components/MP_HealthComponent.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AMP_CPPCharacter::AMP_CPPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
	HealthComponent = CreateDefaultSubobject<UMP_HealthComponent>(TEXT("HealthComponent"));
}

////// MP_Player interface functions //////
USkeletalMeshComponent* AMP_CPPCharacter::GetSkeletalMesh_Implementation() const
{
	return GetMesh();
}

void AMP_CPPCharacter::GrantArmor_Implementation(float ArmorAmount)
{
	Armor = ArmorAmount;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("New Armor: %f"), Armor));
}

void AMP_CPPCharacter::IncrementPickupCount_Implementation()
{
	PickupCount++;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("New PickupCount: %i"), PickupCount));
}

void AMP_CPPCharacter::GrantHealth_Implementation(float HealthAmount)
{
	if (!HealthComponent) return;
	HealthComponent->SetHealth(HealthComponent->GetHealth() + HealthAmount);
}
///////////////////////////////////////////

void AMP_CPPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMP_CPPCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AMP_CPPCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMP_CPPCharacter::Look);

		// General Input for testing
		EnhancedInputComponent->BindAction(GeneralInput, ETriggerEvent::Started, this, &AMP_CPPCharacter::OnGeneralInput);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMP_CPPCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AMP_CPPCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AMP_CPPCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AMP_CPPCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AMP_CPPCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AMP_CPPCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AMP_CPPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 3. Call DOREPLIFETIME passing in the parent object and the variable to be replicated
	//DOREPLIFETIME(ThisClass, PickupCount);
	//DOREPLIFETIME_CONDITION(ThisClass, PickupCount, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(ThisClass, PickupCount, COND_Custom); // For custom replication conditions
																  // A boolean must be associated with
																  // the variable in PreReplication func
	
	// Further implementation can be done: this particular case will replicate Armor only to the Autonomous Proxy
	//DOREPLIFETIME(ThisClass, Armor);
	DOREPLIFETIME_CONDITION(ThisClass, Armor, COND_InitialOrOwner);
	//DOREPLIFETIME_CONDITION(ThisClass, Armor, COND_Custom);
	
	
}

void AMP_CPPCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
	
	// Associate a boolean condition with the replication condition for PickupCount
	// NOTE - the value of bReplicatePickupCount only matters on the server.
	//        To use DOREPLIFETIME_ACTIVE_OVERRIDE you must include the NetCore module
	//        in the Build.cs file.
	DOREPLIFETIME_ACTIVE_OVERRIDE(ThisClass, PickupCount, bReplicatePickupCount);
}

void AMP_CPPCharacter::OnGeneralInput()
{
	Server_PrintMessage(TEXT("This should run on the server."));
}

void AMP_CPPCharacter::OnRep_Armor()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Armor: %f"), Armor));
}

void AMP_CPPCharacter::OnRep_PickupCount(int32 PreviousValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("PickupCount Replicated: %i"), PickupCount));
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Previous PickupCount: %i"), PreviousValue));
}

// RPC function definitions are required to end with "_Implementation"
void AMP_CPPCharacter::Client_PrintMessage_Implementation(const FString& Message)
{
	FString MessageString = HasAuthority() ? "Server: " : "Client: ";
	MessageString += Message;
	
	GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Yellow, MessageString);
}

void AMP_CPPCharacter::Server_PrintMessage_Implementation(const FString& Message)
{
	FString MessageString = HasAuthority() ? "Server: " : "Client: ";
	MessageString += Message;
	
	GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Purple, MessageString);
}

void AMP_CPPCharacter::OnRPCDelayTimer()
{
	/*
	if (HasAuthority())
	{
		Client_PrintMessage(TEXT("This should run on the owning client."));
	}
	*/
	
	if (!HasAuthority()) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	
	GetWorld()->SpawnActor<AMP_Actor>(GetActorLocation(), GetActorRotation(), SpawnParams);
}

void AMP_CPPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Setting a timer to call the client RPC. Begin play is called too early to recognize net roles.
	GetWorldTimerManager().SetTimer(RPCDelayTimer, this, &AMP_CPPCharacter::OnRPCDelayTimer, 4.f, false);
}
