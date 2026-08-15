// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePlayerCharacter.h"
#include "Projectiles/RogueProjectileMagic.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
#include "Projectiles/RogueProjectileBlackhole.h"

// Sets default values
ARoguePlayerCharacter::ARoguePlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	MuzzleSocketName = "Muzzle_01";
}

// Called when the game starts or when spawned
void ARoguePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoguePlayerCharacter::Move(const FInputActionValue& InValue)
{
	FVector2D InputValue = InValue.Get<FVector2D>();
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	//Foward/Back
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

void ARoguePlayerCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackMontage);
	FTimerHandle AttackTimerHandle;
	const float AttackDelayTime = 0.2f;
	UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, GetMesh(), MuzzleSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget,true);
	UGameplayStatics::PlaySound2D(this, CastingSound);
	GetWorldTimerManager().SetTimer(AttackTimerHandle,this, &ARoguePlayerCharacter::AttackTimerElapsed,AttackDelayTime);
	
}

void ARoguePlayerCharacter::AttackTimerElapsed()
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
    FRotator SpawnRotation = GetControlRotation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Instigator = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    ARogueProjectileMagic* NewProjectile =  GetWorld()->SpawnActor<ARogueProjectileMagic>(ProjectileClass,SpawnLocation,SpawnRotation,SpawnParams);
	MoveIgnoreActorAdd(NewProjectile);
}

void ARoguePlayerCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackMontage);
	FTimerHandle BlackHoleAttackTimerHandle;
	const float AttackDelayTime = 0.2f;
	UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, GetMesh(), MuzzleSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget,true);
	UGameplayStatics::PlaySound2D(this, CastingSound);
	GetWorldTimerManager().SetTimer(BlackHoleAttackTimerHandle,this, &ARoguePlayerCharacter::BlackHoleAttackTimerElapsed,AttackDelayTime);
}

void ARoguePlayerCharacter::BlackHoleAttackTimerElapsed()
{
	// 这里的逻辑和 AttackTimerElapsed 几乎一样，只是 Spawn 的类变成了 BlackHoleProjectileClass
	FVector SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
	FRotator SpawnRotation = GetControlRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	// 生成黑洞弹
	GetWorld()->SpawnActor<AActor>(BlackHoleProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
}

// Called every frame
void ARoguePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
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
}
