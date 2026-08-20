// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoguePlayerCharacter.generated.h"

class ARogueProjectileBase;
class URogueActionSystemComponent;
class UNiagaraSystem;
struct FInputActionInstance;
struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API ARoguePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ARoguePlayerCharacter();

protected:
	
	UPROPERTY(EditDefaultsOnly,Category="Attack")
	TSubclassOf<ARogueProjectileBase> ProjectileClass;
	UPROPERTY(EditDefaultsOnly,Category="Attack")
	TSubclassOf<ARogueProjectileBase> BlackHoleProjectileClass;
	UPROPERTY(EditDefaultsOnly,Category="Attack")
	TSubclassOf<ARogueProjectileBase> TeleportClass;
	UPROPERTY(EditDefaultsOnly,Category="Attack")
	FName MuzzleSocketName;
	UPROPERTY(EditDefaultsOnly,Category="Attack")
	TObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(EditDefaultsOnly,Category="Attack")
	TObjectPtr<UNiagaraSystem> CastingEffect;
	UPROPERTY(EditDefaultsOnly,Category="Attack")
	TObjectPtr<USoundBase> CastingSound;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackDelayTime = 0.2f;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> Input_Move;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> Input_Look;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> Input_PrimaryAttack;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> Input_Jump;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> Input_BlackHoleAttack;
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> Input_Teleport;
	UPROPERTY(VisibleAnywhere,Category="Components")
	TObjectPtr<UCameraComponent> CameraComponent;
	UPROPERTY(VisibleAnywhere,Category="Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(visibleDefaultsOnly,BlueprintReadOnly,Category="Components")
	TObjectPtr<URogueActionSystemComponent> ActionSystemComponent;
	UPROPERTY(EditDefaultsOnly,Category="death")
	TObjectPtr<UAnimMontage> DeathMontage;
	
	FTimerHandle AttackTimerHandle;
	
	void Move(const FInputActionValue& InValue);
	void Look(const FInputActionInstance& InValue);
	void StartAttack(TSubclassOf<ARogueProjectileBase> InProjectileClass);
	void AttackTimerElapsed(TSubclassOf<ARogueProjectileBase> InProjectileClass);
	void PrimaryAttack ();
	void BlackHoleAttack();
	void TeleportAttack();
	UFUNCTION()
	void OnHealthChanged(float NewHealth,float OldHealth);
	
public:	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


};
