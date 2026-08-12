// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueItemChest.h"



ARogueItemChest::ARogueItemChest()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComp"));
	BaseMeshComponent->SetCollisionProfileName("Interaction");
	RootComponent = BaseMeshComponent;
	
	LidMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMeshComp"));
	LidMeshComponent->SetCollisionProfileName("NoCollision");
	LidMeshComponent->SetupAttachment(BaseMeshComponent);
}

void ARogueItemChest::Interact_Implementation()
{
	//开始播放动画
	if (bChestOpened)
	{
		return ;
	}
	bChestOpened = true;
	SetActorTickEnabled(true);
}

void ARogueItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentAnimationPitch = FMath::FInterpConstantTo(CurrentAnimationPitch, AnimationTargetPitch, DeltaTime, AnimationSpeed);
	LidMeshComponent->SetRelativeRotation(FRotator(CurrentAnimationPitch,0.f,0.f));
	if (FMath::IsNearlyEqual(CurrentAnimationPitch,AnimationTargetPitch))
	{
		SetActorTickEnabled(false);
		ChestAnimationComplete();
	}
	
}

