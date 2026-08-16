// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoguePlayerCharacter.generated.h"

class URogueActionSystemComponent;
class ARogueProjectileTeleport;
class ARogueProjectileBlackhole;
class UNiagaraSystem;
class ARogueProjectileMagic;
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
	// Sets default values for this character's properties
	ARoguePlayerCharacter();

protected:
	UPROPERTY(EditDefaultsOnly,Category="PrimaryAttack")
	TSubclassOf<ARogueProjectileMagic> ProjectileClass;
	UPROPERTY(EditDefaultsOnly,Category="SecondaryAttack")
	TSubclassOf<ARogueProjectileBlackhole> BlackHoleProjectileClass;
	UPROPERTY(EditDefaultsOnly,Category="ThirdAttack")
	TSubclassOf<ARogueProjectileTeleport> TeleportClass;
	UPROPERTY(VisibleAnywhere,Category="PrimaryAttack")
	FName MuzzleSocketName;
	UPROPERTY(EditDefaultsOnly,Category="PrimaryAttack")
	TObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(EditDefaultsOnly,Category="PrimaryAttack")
	TObjectPtr<UNiagaraSystem> CastingEffect;
	UPROPERTY(EditDefaultsOnly,Category="PrimaryAttack")
	TObjectPtr<USoundBase> CastingSound;
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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& InValue);
	void Look(const FInputActionInstance& InValue);
	void PrimaryAttack ();
	void AttackTimerElapsed();
	void BlackHoleAttack();
	void BlackHoleAttackTimerElapsed();
	void TeleportAttack();
	void TeleportTimerElapsed();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


};
