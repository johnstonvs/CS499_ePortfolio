// Stephen Johnston


#include "MP_Armor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interaction/MP_Player.h"

// Sets default values
AMP_Armor::AMP_Armor()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(RootComponent);
	SphereMesh->SetIsReplicated(true); // Must replicate components since they're not root component

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxMesh->SetupAttachment(RootComponent);
	BoxMesh->SetIsReplicated(true); // Must replicate components since they're not root component
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);

	// Allow the actor to be loaded on the client side
	bNetLoadOnClient = true;
	
	// Allow this actor's variables to replicate
	bReplicates = true;
	
	// Allows this actor's movement to be replicated
	// SeReplicateMovement is a virtual function that should not be called in a constructor
	// SetReplicatingMovement must be used
	SetReplicatingMovement(true);
}

// Called when the game starts or when spawned
void AMP_Armor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMP_Armor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMP_Armor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// Ensure this only happens on the server
	if (!OtherActor->HasAuthority()) return;
	
	// Check if the other actor implements the MP_Player interface
	if (OtherActor->Implements<UMP_Player>())
	{
		// Execute the MP_Player interface GetSkeletalMesh function to use the mesh
		USkeletalMeshComponent* Mesh = IMP_Player::Execute_GetSkeletalMesh(OtherActor);

		// Attach the components to the other actor using a socket name
		// You can only use a socket name if the other actors root component is a skeletal mesh with a socket sharing that name
		SphereMesh->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "s_head");
		BoxMesh->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "s_hand_r");
		
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Execute the grant armor function within MP_Player interface
		// First argument to these functions must be the object it is to be executed on, then the arguments for the function itself
		IMP_Player::Execute_GrantArmor(OtherActor, ArmorValue);
	}
}

