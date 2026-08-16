// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogueActionSystemComponent.generated.h"

USTRUCT(BlueprintType)
struct  FRogueAttributeSet
{
	GENERATED_BODY()
	
	FRogueAttributeSet()
		:health(100.f){}
	
	UPROPERTY(BlueprintReadOnly)
	float health;
};
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API URogueActionSystemComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	
	void ApplyHealthChange(float InValueChange);
	
protected:
	UPROPERTY(BlueprintReadOnly,Category="Attributes")
	FRogueAttributeSet AttributeSet;
	
public:
	
	URogueActionSystemComponent();
};
