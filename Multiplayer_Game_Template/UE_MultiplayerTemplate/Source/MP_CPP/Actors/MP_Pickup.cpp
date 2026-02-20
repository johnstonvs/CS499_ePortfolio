// Stephen Johnston


#include "MP_Pickup.h"

#include "Components/SphereComponent.h"
#include "Interaction/MP_Player.h"


// Sets default values
AMP_Pickup::AMP_Pickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetIsReplicated(true); // Must be replicated since it's not the root component
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	
	// Allow the actor to be loaded on the client side
	bNetLoadOnClient = true;
	
	// Allow this actor's variables to replicate
	bReplicates = true;
}

// Called when the game starts or when spawned
void AMP_Pickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMP_Pickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	// Ensure this only happens on the server
	if (!OtherActor->HasAuthority()) return;
	
	// Check if the other actor implements MP_Player interface
	if (OtherActor->Implements<UMP_Player>())
	{
		// Execute the grant pickup function in the interface
		// First argument of these functions must be the object it is to be executed on, then the arguments for the function itself
		IMP_Player::Execute_IncrementPickupCount(OtherActor);
		IMP_Player::Execute_GrantHealth(OtherActor, HealthValue);
		Destroy();
	}
	
}