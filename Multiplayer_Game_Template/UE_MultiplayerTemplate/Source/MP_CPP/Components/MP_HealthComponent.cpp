// Stephen Johnston


#include "MP_HealthComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UMP_HealthComponent::UMP_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// In an actor component's constructor, use SetIsReplicatedByDefault instead of SetIsReplicated		
	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UMP_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UMP_HealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, Health);
}

void UMP_HealthComponent::OnRep_Health(float PreviousValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Health replicated from %f to %f"), PreviousValue, Health));
}

