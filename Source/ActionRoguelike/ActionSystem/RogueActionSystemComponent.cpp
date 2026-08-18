// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"

URogueActionSystemComponent::URogueActionSystemComponent()
{
	
}

void URogueActionSystemComponent::ApplyHealthChange(float InValueChange)
{
	float Oldhealth = AttributeSet.health;
	AttributeSet.health += InValueChange;
	OnhealthChange.Broadcast(AttributeSet.health, Oldhealth);
}
