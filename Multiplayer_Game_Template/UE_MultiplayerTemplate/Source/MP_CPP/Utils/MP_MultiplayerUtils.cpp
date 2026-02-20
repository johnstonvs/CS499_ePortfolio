// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_MultiplayerUtils.h"

void UMP_MultiplayerUtils::PrintAuthority(AActor* Actor)
{
	if (Actor->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor %s has Authority."), *GetNameSafe(Actor))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor %s does not have Authority."), *GetNameSafe(Actor))
	}
}

void UMP_MultiplayerUtils::PrintLocalNetRole(AActor* Actor)
{
	switch (Actor->GetLocalRole())
	{
	case ROLE_None:
		UE_LOG(LogTemp, Warning, TEXT("%s LocalRole: ROLE_None"), *GetNameSafe(Actor));
		break;

	case ROLE_SimulatedProxy:
		UE_LOG(LogTemp, Warning, TEXT("%s LocalRole: ROLE_SimulatedProxy"), *GetNameSafe(Actor));
		break;

	case ROLE_AutonomousProxy:
		UE_LOG(LogTemp, Warning, TEXT("%s LocalRole: ROLE_AutonomousProxy"), *GetNameSafe(Actor));
		break;

	case ROLE_Authority:
		UE_LOG(LogTemp, Warning, TEXT("%s LocalRole: ROLE_Authority"), *GetNameSafe(Actor));
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("%s LocalRole: Unknown (%d)"), *GetNameSafe(Actor), static_cast<int32>(Actor->GetLocalRole()));
		break;
	}
}

void UMP_MultiplayerUtils::PrintRemoteNetRole(AActor* Actor)
{
	switch (Actor->GetRemoteRole())
	{
	case ROLE_None:
		UE_LOG(LogTemp, Warning, TEXT("%s RemoteRole: ROLE_None"), *GetNameSafe(Actor));
		break;

	case ROLE_SimulatedProxy:
		UE_LOG(LogTemp, Warning, TEXT("%s RemoteRole: ROLE_SimulatedProxy"), *GetNameSafe(Actor));
		break;

	case ROLE_AutonomousProxy:
		UE_LOG(LogTemp, Warning, TEXT("%s RemoteRole: ROLE_AutonomousProxy"), *GetNameSafe(Actor));
		break;

	case ROLE_Authority:
		UE_LOG(LogTemp, Warning, TEXT("%s RemoteRole: ROLE_Authority"), *GetNameSafe(Actor));
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("%s RemoteRole: Unknown (%d)"), *GetNameSafe(Actor), static_cast<int32>(Actor->GetRemoteRole()));
		break;
	}
}
