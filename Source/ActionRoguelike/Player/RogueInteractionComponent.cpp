// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"
#include "CollisionShape.h"
#include "RogueEngineTypes.h"
#include "ActionRoguelike/Core/RogueInteractionInterface.h"
#include "Engine/OverlapResult.h"

URogueInteractionComponent::URogueInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URogueInteractionComponent::Interact()
{
	IRogueInteractionInterface* InteractInterface = Cast <IRogueInteractionInterface>(SelectedActor);
	if (InteractInterface)
	{
		InteractInterface->Interact();
	}	
}


void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	APlayerController* PC =CastChecked<APlayerController>(GetOwner());
	FVector Center = PC->GetPawn()->GetActorLocation();
	TArray<FOverlapResult> Overlaps;
	ECollisionChannel CollisionChannel = COLLISION_INTERACTION;
	FCollisionShape Shape;
	Shape.SetSphere(InteractionRadius);
	GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, CollisionChannel, Shape);
	
	
	AActor* BestActor = nullptr;
	float HighestDotResult = -1.0f;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		FVector OverlapLocation = Overlap.GetActor()->GetActorLocation();
		FVector OverlapDirection = (OverlapLocation - Center).GetSafeNormal();
		
		float DotResult = FVector::DotProduct(PC->GetControlRotation().Vector(), OverlapDirection);
		FString DebugString = FString::Printf(TEXT("Dot: %f"),DotResult);
		
		if (DotResult > HighestDotResult)
		{
			BestActor = Overlap.GetActor();
			HighestDotResult = DotResult;
		}
		DrawDebugSphere(GetWorld(), Center, InteractionRadius, 32, FColor::White);
		DrawDebugBox(GetWorld(), OverlapLocation, FVector(50.f), FColor::Red);
		DrawDebugString(GetWorld(), OverlapLocation, DebugString, nullptr, FColor::White, 0.f, true);
	}
	
	SelectedActor = BestActor;
	
	if (SelectedActor)
	{

		DrawDebugBox(GetWorld(), BestActor->GetActorLocation(), FVector(60.f), FColor::Green);
		
	}
}
