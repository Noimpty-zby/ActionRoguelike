// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplodingBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AExplodingBarrel::AExplodingBarrel()
{
	PrimaryActorTick.bCanEverTick = false;
	BarrelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMeshComp"));
	RootComponent = BarrelMeshComponent;
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComponent->SetupAttachment(BarrelMeshComponent);
	BurningEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BurningEffectComp"));
	BurningEffect->SetupAttachment(BarrelMeshComponent);
	BurningSound = CreateDefaultSubobject<UAudioComponent>(TEXT("BurningSoundComp"));
	BurningSound->SetupAttachment(BarrelMeshComponent);
	BarrelMeshComponent->SetCollisionProfileName("PhysicsActor");
	BarrelMeshComponent->SetSimulatePhysics(true);
	BurningEffect->SetAutoActivate(false);
	BurningSound->SetAutoActivate(false);
	RadialForceComponent->SetAutoActivate(false);
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->Radius = 750.f;
	RadialForceComponent->ImpulseStrength = 2500.f;
	
}

float AExplodingBarrel::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (bExploded)
	{
		return ActualDamage;
	}
	bExploded = true;
	BurningEffect->Activate();
	BurningSound->Play();
	GetWorldTimerManager().SetTimer(FuseTimerHandle, this, &AExplodingBarrel::Explode, FuseDelay, false);	
	return ActualDamage;
}
void AExplodingBarrel::Explode()
{
	BurningEffect->Deactivate();
	BurningSound->Stop();
	if (ExplosionEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());
	}
	if (BurningSound)
	{	
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}
	BarrelMeshComponent->AddImpulse(FVector::UpVector * 1000, NAME_None, true);
	BarrelMeshComponent->AddAngularImpulseInDegrees(FVector::RightVector * 1000, NAME_None, true);
	RadialForceComponent->FireImpulse();
}