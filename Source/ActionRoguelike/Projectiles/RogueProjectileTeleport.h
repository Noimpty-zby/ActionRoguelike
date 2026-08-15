// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectileBase.h"
#include "RogueProjectileTeleport.generated.h"

class UNiagaraSystem;
class USoundBase;
UCLASS()
class ACTIONROGUELIKE_API ARogueProjectileTeleport : public ARogueProjectileBase
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly,Category="Effects")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;
	UPROPERTY(EditDefaultsOnly,Category="Sound")
	TObjectPtr<USoundBase> ExplosionSound;
	virtual void BeginPlay() override;	
	void Explode();
	void TeleportInstigator();
	FTimerHandle ExplodeTimer;
	FTimerHandle TeleportTimer;
public:
	ARogueProjectileTeleport();
	virtual void PostInitializeComponents() override;
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
