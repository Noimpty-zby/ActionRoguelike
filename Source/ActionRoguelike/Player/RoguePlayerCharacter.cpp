// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
#include "Projectiles/RogueProjectileBlackhole.h"

ARoguePlayerCharacter::ARoguePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	MuzzleSocketName = "Muzzle_01";
	
	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("ActionSystemComp"));
}


void ARoguePlayerCharacter::Move(const FInputActionValue& InValue)
{
	FVector2D InputValue = InValue.Get<FVector2D>();
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	//Forward/Back
	AddMovementInput(ControlRot.Vector(),InputValue.X);
	//Sideways
	FVector RightDirection = ControlRot.RotateVector(FVector::RightVector);
	AddMovementInput(RightDirection,InputValue.Y);
	
}

void ARoguePlayerCharacter::Look(const FInputActionInstance& InValue)
{
	FVector2D InputValue = InValue.GetValue().Get<FVector2D>();
	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void ARoguePlayerCharacter::StartAttack(TSubclassOf<ARogueProjectileBase> InProjectileClass)
{
	PlayAnimMontage(AttackMontage);

	UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, GetMesh(), MuzzleSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
	UGameplayStatics::PlaySound2D(this, CastingSound);

	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(
		this, &ARoguePlayerCharacter::AttackTimerElapsed, InProjectileClass);

	GetWorldTimerManager().SetTimer(AttackTimerHandle, Delegate, AttackDelayTime, false);
}

void ARoguePlayerCharacter::AttackTimerElapsed(TSubclassOf<ARogueProjectileBase> InProjectileClass)
{
	if (!ensure(InProjectileClass)) { return; }

	FVector SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
	FRotator SpawnRotation = GetControlRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<ARogueProjectileBase>(InProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
}


void ARoguePlayerCharacter::PrimaryAttack()
{
	StartAttack(ProjectileClass);
}

void ARoguePlayerCharacter::BlackHoleAttack()
{
	StartAttack(BlackHoleProjectileClass);
}

void ARoguePlayerCharacter::TeleportAttack()
{
	StartAttack(TeleportClass);
}


float ARoguePlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                        class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ActionSystemComponent->ApplyHealthChange(-DamageAmount);
	return ActualDamage;
}

void ARoguePlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ActionSystemComponent->OnhealthChange.AddDynamic(this,&ARoguePlayerCharacter::OnHealthChanged);
}

void ARoguePlayerCharacter::OnHealthChanged(float NewHealth, float OldHealth)
{
	if (FMath::IsNearlyZero(NewHealth))
	{
		DisableInput(nullptr);
		GetMovementComponent()->StopMovementImmediately();
		PlayAnimMontage(DeathMontage);
	}
}

void ARoguePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInput->BindAction(Input_Move,ETriggerEvent::Triggered,this,&ARoguePlayerCharacter::Move);
	EnhancedInput->BindAction(Input_Look,ETriggerEvent::Triggered,this,&ARoguePlayerCharacter::Look);
	EnhancedInput->BindAction(Input_PrimaryAttack,ETriggerEvent::Triggered,this,&ARoguePlayerCharacter::PrimaryAttack);
	EnhancedInput->BindAction(Input_Jump,ETriggerEvent::Started,this,&ACharacter::Jump);
	EnhancedInput->BindAction(Input_Jump,ETriggerEvent::Completed,this,&ACharacter::StopJumping);
	EnhancedInput->BindAction(Input_BlackHoleAttack,ETriggerEvent::Triggered,this,&ARoguePlayerCharacter::BlackHoleAttack);
	EnhancedInput->BindAction(Input_Teleport,ETriggerEvent::Triggered,this,&ARoguePlayerCharacter::TeleportAttack);
}


