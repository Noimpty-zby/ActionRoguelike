// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueInteractionComponent.h"
#include "CollisionShape.h"
#include "RogueEngineTypes.h"
#include "ActionRoguelike/Core/RogueInteractionInterface.h"
#include "Engine/OverlapResult.h"

TAutoConsoleVariable <bool> CVarInteractionDebugDrawing(TEXT("game.interaction.DebugDraw"), false, 
	TEXT("Enable interaction component debug rendering. (0 = off, 1 = enabled"), ECVF_Cheat);

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
	APawn* MyPawn = PC->GetPawn();
	if (!MyPawn)
	{
		return;
	}
	FVector Center = MyPawn->GetActorLocation();
	FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
	TArray<FOverlapResult> Overlaps;
	ECollisionChannel CollisionChannel = COLLISION_INTERACTION;
	FCollisionShape Shape;
	Shape.SetSphere(InteractionRadius);
	GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, CollisionChannel, Shape);
	
	
	AActor* BestActor = nullptr;
	float HighestWeight = 0.0f;
	bool bEnabledDebugDraw = CVarInteractionDebugDrawing.GetValueOnGameThread();
	
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* OverlapActor = Overlap.GetActor();
		if (OverlapActor == nullptr)
		{
			continue;
		}
		FVector Origin;
		FVector BoxExtend;
		OverlapActor->GetActorBounds(true, Origin, BoxExtend);
		FVector OverlapDirection = (Origin - CameraLocation).GetSafeNormal();
		float RadiusSquared = InteractionRadius * InteractionRadius;
		float DistanceTo = (Origin - Center).SizeSquared();
		float NormalizedDistanceTo = 1.0f - (DistanceTo / RadiusSquared);
		float DotResult = FVector::DotProduct(PC->GetControlRotation().Vector(), OverlapDirection);
		float NormalizedDotResult = DotResult * 0.5f + 0.5f;
		float Weight = (NormalizedDotResult *DirectionWeightScale) + (NormalizedDistanceTo * DistanceToWeightScale);
		if (Weight > HighestWeight)
		{
			BestActor = OverlapActor;
			HighestWeight = Weight;
		}
		if (bEnabledDebugDraw)
		{
			FString DebugString = FString::Printf(TEXT("Weight: %f, Dot: %f, Dist: %f"),Weight, NormalizedDotResult, NormalizedDistanceTo);
			DrawDebugBox(GetWorld(), Origin, FVector(50.f), FColor::Red);
			DrawDebugString(GetWorld(), Origin, DebugString, nullptr, FColor::White, 0.f, true);
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
