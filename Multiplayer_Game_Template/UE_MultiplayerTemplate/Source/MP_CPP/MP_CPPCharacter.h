// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/MP_Player.h"
#include "Logging/LogMacros.h"
#include "MP_CPPCharacter.generated.h"

class UMP_HealthComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AMP_CPPCharacter : public ACharacter, public IMP_Player
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MouseLookAction;

public:

	/** Constructor */
	AMP_CPPCharacter();

	//////** IMP_Player Interface *//////
	virtual USkeletalMeshComponent* GetSkeletalMesh_Implementation() const override;
	virtual void GrantArmor_Implementation(float ArmorAmount) override;
	virtual void GrantHealth_Implementation(float HealthAmount) override;
	virtual void IncrementPickupCount_Implementation() override;
	
	////////////////////////////////////

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/////////// Crash Course ///////////

	
	// Making a variable replicated
	// 1. Override GetLifetimeReplicatedProps
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// This function is necessary for custom conditions for a variable to replicate
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	
private:

	// 2. Add the UPROPERTY macro with the Replicated specifier. If using RepNotify, use ReplicatedUsing = OnRep_Armor
	UPROPERTY(ReplicatedUsing = OnRep_Armor)
	float Armor;
	
	UPROPERTY(ReplicatedUsing = OnRep_PickupCount)
	int32 PickupCount;

	// Step 3 is in GetLifetimeReplicatedProps function in .cpp

	/** General Input for debugging, testing, and observation */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* GeneralInput;

	void OnGeneralInput();
	
	// Rep notify functions get called when their respective variable is replicated
	UFUNCTION()
	void OnRep_Armor();
	
	UFUNCTION()
	void OnRep_PickupCount(int32 PreviousValue); // Passing in an argument of the same data type as the 
												 // variable itself will give you access to the value it
												 // was prior to replicating.
	
	bool bReplicatePickupCount = true;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMP_HealthComponent> HealthComponent;
	
	// Remote Procedure Call (RPC) - Client, Server, and NetMulticast
	
	UFUNCTION(Client, Reliable) // Client - standard practice to prepend any client RPC with "Client_"
	void Client_PrintMessage(const FString& Message);
	
	UFUNCTION(Server, Reliable) // Server
	void Server_PrintMessage(const FString& Message);
	
	FTimerHandle RPCDelayTimer;
	
	void OnRPCDelayTimer();
	
protected:
	virtual void BeginPlay() override;
};

