// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"

ARogueProjectileBase::ARogueProjectileBase()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComponent;
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComp"));
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	
	LoopedNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LoopedNiagaraComp"));
	LoopedNiagaraComponent->SetupAttachment(SphereComponent);
	
	LoopedAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("LoopedAudioComp"));
	LoopedAudioComponent->SetupAttachment(SphereComponent);
}

