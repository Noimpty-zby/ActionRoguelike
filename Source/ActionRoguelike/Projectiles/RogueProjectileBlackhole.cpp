// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileBlackhole.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ARogueProjectileBlackhole::ARogueProjectileBlackhole()
{
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadiaForceComp"));
	RadialForceComponent->SetupAttachment(SphereComponent);
	RadialForceComponent->SetAutoActivate(true);
	RadialForceComponent->Radius = 750.f;
	RadialForceComponent->ForceStrength = -2000000.f;
	RadialForceComponent->RemoveObjectTypeToAffect(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}

void ARogueProjectileBlackhole::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5.f);
}

void ARogueProjectileBlackhole::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ARogueProjectileBlackhole::OnSphereOverlop);
}

void ARogueProjectileBlackhole::OnSphereOverlop(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherComp && OtherComp->IsSimulatingPhysics()&& OtherActor != GetInstigator() && OtherActor!=this)
	{
		OtherActor->Destroy();
	}
}
