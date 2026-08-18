// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"

URogueActionSystemComponent::URogueActionSystemComponent()
{
	
}

void URogueActionSystemComponent::ApplyHealthChange(float InValueChange)
{
	float Oldhealth = AttributeSet.health;
	float MaxHealth = GetDefault<URogueActionSystemComponent>()->AttributeSet.health;
	AttributeSet.health = FMath::Clamp(AttributeSet.health + InValueChange, 0.f, MaxHealth);
	if (!FMath::IsNearlyEqual(Oldhealth, AttributeSet.health))
	{
		OnhealthChange.Broadcast(AttributeSet.health, Oldhealth);
	}
	UE_LOG(LogTemp, Log, TEXT("New Health: %f, MaxHealth: %f"),AttributeSet.health, MaxHealth);
}
