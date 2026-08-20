// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectileBase.h"
#include "RogueProjectileBlackhole.generated.h"

class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API ARogueProjectileBlackhole : public ARogueProjectileBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly,Category="Components")
	TObjectPtr<URadialForceComponent> RadialForceComponent;
	
	
	virtual void BeginPlay() override;
	
public:
	ARogueProjectileBlackhole();
	virtual void PostInitializeComponents() override;
	UFUNCTION()
	void OnSphereOverlop( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
