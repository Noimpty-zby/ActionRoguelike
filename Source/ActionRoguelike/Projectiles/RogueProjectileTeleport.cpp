/*
 */
#include "RogueProjectileTeleport.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARogueProjectileTeleport::ARogueProjectileTeleport()
{
	SphereComponent->SetCollisionProfileName("Projectile");
}

void ARogueProjectileTeleport::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentHit.AddDynamic(this, &ARogueProjectileTeleport::OnActorHit);
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ARogueProjectileTeleport::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ARogueProjectileTeleport::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(ExplodeTimer, this, &ARogueProjectileTeleport::Explode, 0.2f);
}

void ARogueProjectileTeleport::Explode()
{
	GetWorldTimerManager().ClearTimer(ExplodeTimer);
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->StopMovementImmediately();
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation(), FRotator::ZeroRotator);
	GetWorldTimerManager().SetTimer(TeleportTimer, this, &ARogueProjectileTeleport::TeleportInstigator, 0.2f);
}

void ARogueProjectileTeleport::TeleportInstigator()
{
	AActor* InstigatorActor = GetInstigator();
	if (InstigatorActor)
	{
		InstigatorActor->TeleportTo(GetActorLocation(), InstigatorActor->GetActorRotation(), false ,false);
	}
	const bool bSuccess = InstigatorActor->TeleportTo(GetActorLocation(), InstigatorActor->GetActorRotation());
	UE_LOG(LogTemp, Log, TEXT("TeleportTo: %s"), bSuccess ? TEXT("OK") : TEXT("FAILED"));
	Destroy();
}
