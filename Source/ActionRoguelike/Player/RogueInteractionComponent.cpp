// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"
#include "CollisionShape.h"
#include "RogueEngineTypes.h"
#include "ActionRoguelike/Core/RogueInteractionInterface.h"
#include "Engine/OverlapResult.h"

TAutoConsoleVariable <bool> CVarInteractionDebugDrawing(TEXT("game.interaction.DebugDraw"), false, 
	TEXT("Eable interaction component debug rendering. (0 = off, 1 = enabled"), ECVF_Cheat);

URogueInteractionComponent::URogueInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URogueInteractionComponent::Interact()
{
	if (SelectedActor==nullptr)
	{
		return;
	}
	if (SelectedActor->Implements<URogueInteractionInterface>())
	{
		IRogueInteractionInterface::Execute_Interact(SelectedActor);
	}
}


void URogueInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	APlayerController* PC =CastChecked<APlayerController>(GetOwner());
	APawn* Mypawn = PC->GetPawn();
	if (!Mypawn)
	{
		return;
	}
	FVector Center = Mypawn->GetActorLocation();
	TArray<FOverlapResult> Overlaps;
	ECollisionChannel CollisionChannel = COLLISION_INTERACTION;
	FCollisionShape Shape;
	Shape.SetSphere(InteractionRadius);
	GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, CollisionChannel, Shape);
	
	
	AActor* BestActor = nullptr;
	float HighestDotResult = 0.7f;
	bool bEnabledDebugDraw = CVarInteractionDebugDrawing.GetValueOnGameThread();
	
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* OverlapActor = Overlap.GetActor();
		if (OverlapActor == nullptr)
		{
			continue;
		}
		FVector OverlapLocation = OverlapActor->GetActorLocation();
		FVector OverlapDirection = (OverlapLocation - Center).GetSafeNormal();
		
		float DotResult = FVector::DotProduct(PC->GetControlRotation().Vector(), OverlapDirection);
		
		if (DotResult > HighestDotResult)
		{
			BestActor = OverlapActor;
			HighestDotResult = DotResult;
		}
		if (bEnabledDebugDraw)
		{
			FString DebugString = FString::Printf(TEXT("Dot: %f"),DotResult);
			DrawDebugBox(GetWorld(), OverlapLocation, FVector(50.f), FColor::Red);
			DrawDebugString(GetWorld(), OverlapLocation, DebugString, nullptr, FColor::White, 0.f, true);
		}
	}
	
	
	SelectedActor = BestActor;
	if (bEnabledDebugDraw)
	{
		if (SelectedActor)
		{

			DrawDebugBox(GetWorld(), BestActor->GetActorLocation(), FVector(60.f), FColor::Green);
		
		}
		DrawDebugSphere(GetWorld(), Center, InteractionRadius, 32, FColor::White);
	}
}
