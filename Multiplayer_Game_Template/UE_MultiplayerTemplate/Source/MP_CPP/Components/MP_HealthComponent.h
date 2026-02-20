// Stephen Johnston

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interaction/MP_Player.h"
#include "MP_HealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MP_CPP_API UMP_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMP_HealthComponent();
	
	void SetHealth(const float NewHealth) { Health = NewHealth; };
	float GetHealth() const { return Health; }
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Use GetLifetimeReplicatedProps to replicate variables
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Health)
	float Health = 100;
	
	UFUNCTION()
	void OnRep_Health(float PreviousValue);
};
