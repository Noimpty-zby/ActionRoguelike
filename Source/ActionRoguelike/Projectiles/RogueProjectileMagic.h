// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueProjectileMagic.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UProjectileMovementComponent;
class USphereComponent;
class UAudioComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARogueProjectileMagic : public AActor
{
	GENERATED_BODY()

public:

	ARogueProjectileMagic();
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(EditDefaultsOnly,Category="Damage")
	TSubclassOf<UDamageType> DmgTypeClass;
	UPROPERTY(EditDefaultsOnly,Category="Effects")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;
	UPROPERTY(EditDefaultsOnly,Category="Sound")
	TObjectPtr<USoundBase> ExplosionSound;
	//球体
	UPROPERTY(EditDefaultsOnly,Category="Components")
	TObjectPtr<USphereComponent> SphereComponent;
	//投射物移动
	UPROPERTY(EditDefaultsOnly,Category="Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	UPROPERTY(EditDefaultsOnly,Category="Components")
	TObjectPtr<UNiagaraComponent> LoopedNiagaraComponent;
	UPROPERTY(EditDefaultsOnly,Category="Components")
	TObjectPtr<UAudioComponent> LoopedAudioComponent;
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
};
