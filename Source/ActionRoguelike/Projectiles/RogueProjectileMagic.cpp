// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileMagic.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARogueProjectileMagic::ARogueProjectileMagic()
{
	SphereComponent->SetSphereRadius(16.0f);
	SphereComponent->SetCollisionProfileName("Projectile");
}

void ARogueProjectileMagic::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentHit.AddDynamic(this,&ARogueProjectileMagic::OnActorHit);
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(),true);
	
}

void ARogueProjectileMagic::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
 {
	
	FVector HitFromDirection = GetActorRotation().Vector();
	UGameplayStatics::ApplyPointDamage(OtherActor, 10.f, HitFromDirection, Hit, GetInstigatorController(),this, DmgTypeClass);
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());
	
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
	Destroy();
	
 }