// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueProjectileBase.generated.h"
class UProjectileMovementComponent;
class USphereComponent;
class UAudioComponent;
class UNiagaraComponent;

UCLASS(Abstract)
class ACTIONROGUELIKE_API ARogueProjectileBase : public AActor
{
	GENERATED_BODY()

protected:
	//球体
	UPROPERTY(EditDefaultsOnly,Category="Components")
	TObjectPtr<USphereComponent> SphereComponent;
	//投射物移动
	UPROPERTY(EditDefaultsOnly,Category="Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	UPROPERTY(EditDefaultsOnly,Category="Components")
	//投射物攻击过程中的粒子效果
	TObjectPtr<UNiagaraComponent> LoopedNiagaraComponent;
	//投射物攻击过程中的声音效果
	UPROPERTY(EditDefaultsOnly,Category="Components")
	TObjectPtr<UAudioComponent> LoopedAudioComponent;
	
public:
	ARogueProjectileBase();
};