// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplodingBarrel.generated.h"
class URadialForceComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class USoundBase;
class UAudioComponent;
class UStaticMeshComponent;
UCLASS()
class ACTIONROGUELIKE_API AExplodingBarrel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExplodingBarrel();

protected:
	UPROPERTY(EditDefaultsOnly,Category="Explosion")
	float FuseDelay = 3.0f;
	UPROPERTY(EditDefaultsOnly,Category="Effects")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;
	UPROPERTY(EditDefaultsOnly,Category="Effects")
	TObjectPtr<USoundBase> ExplosionSound;
	UPROPERTY(VisibleAnywhere,Category="Components")
	TObjectPtr<UNiagaraComponent> BurningEffect;
	UPROPERTY(VisibleAnywhere,Category="Components")
	TObjectPtr<UAudioComponent> BurningSound;
	UPROPERTY(VisibleAnywhere,Category="Components")
	TObjectPtr<UStaticMeshComponent> BarrelMeshComponent;
	UPROPERTY(VisibleAnywhere,Category="Components")
	TObjectPtr<URadialForceComponent> RadialForceComponent;
	
	FTimerHandle FuseTimerHandle;
	bool bExploded = false;
	
	UFUNCTION(BlueprintCallable)
	void Explode();
public:
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
};
