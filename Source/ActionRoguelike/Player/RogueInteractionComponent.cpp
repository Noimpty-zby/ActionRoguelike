// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"
#include "CollisionShape.h"
#include "Engine/OverlapResult.h"

URogueInteractionComponent::URogueInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	APlayerController* PC =CastChecked<APlayerController>(GetOwner());
	FVector Center = PC->GetPawn()->GetActorLocation();
	TArray<FOverlapResult> Overlaps;
	ECollisionChannel CollisionChannel = ECC_Visibility;
	FCollisionShape Shape;
	Shape.SetSphere(InteractionRadius);
	GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, CollisionChannel, Shape);
	DrawDebugSphere(GetWorld(), Center, InteractionRadius, 32, FColor::White);
	
	AActor* BestActor = nullptr;
	float HighestDotResult = -1.0f;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		FVector OverlapLocation = Overlap.GetActor()->GetActorLocation();
		DrawDebugBox(GetWorld(), OverlapLocation, FVector(50.f), FColor::Red);
		FVector OverlapDirection = (OverlapLocation - Center).GetSafeNormal();
		float DotResult = FVector::DotProduct(PC->GetControlRotation().Vector(), OverlapDirection);
		FString DebugString = FString::Printf(TEXT("Dot: %f"),DotResult);
		
		DrawDebugString(GetWorld(), OverlapLocation, DebugString, nullptr, FColor::White, 0.f, true);
		if (DotResult > HighestDotResult)
		{
			BestActor = Overlap.GetActor();
			HighestDotResult = DotResult;
		}
	}
	if (BestActor)
	{
		DrawDebugBox(GetWorld(), BestActor->GetActorLocation(), FVector(60.f), FColor::Green);
		
	}
}
