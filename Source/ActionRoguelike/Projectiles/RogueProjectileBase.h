// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueProjectileBase.generated.h"
class UProjectileMovementComponent;
class USphereComponent;

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
	
public:
	ARogueProjectileBase();
};