// Stephen Johnston

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MP_Actor.generated.h"

UCLASS()
class MP_CPP_API AMP_Actor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMP_Actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(Client, Reliable)
	void Client_PrintActorName();
	
	UFUNCTION(Server, Reliable)
	void Server_PrintActorName();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnRep_Owner() override;
};
