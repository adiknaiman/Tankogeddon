// Fill out your copyright notice in the Description page of Project Settings.



#include "Turret.h"
#include <Components\BoxComponent.h>
#include <Components\StaticMeshComponent.h>
#include <Components\ArrowComponent.h>
#include "Engine\StaticMesh.h"
#include "Cannon.h"
#include <Kismet\KismetMathLibrary.h>
#include "HealthComponent.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = false;

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollider"));
	RootComponent = HitCollider;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(HitCollider);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if (BodyMeshTemp)
	{
		BodyMesh->SetStaticMesh(BodyMeshTemp);
	}

	UStaticMesh* TurretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if (TurretMeshTemp)
	{
		TurretMesh->SetStaticMesh(TurretMeshTemp);
	}
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->OnHealthChanged.AddUObject(this, &ATurret::DamageTaken);
	HealthComponent->OnDie.AddUObject(this, &ATurret::Die);
}

void ATurret::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

void ATurret::Die()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
	Destroy();
}

void ATurret::DamageTaken(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), HealthComponent->GetHealth());
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	SetupCannon();

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FTimerHandle TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);
}

void ATurret::Targeting()
{
	if (!PlayerPawn)
	{
		return;
	}

	if (IsPLayerInRange())
	{
		RotateToPlayer();
		if (CanFire())
		{
			Fire();
		}
	}
}

void ATurret::RotateToPlayer()
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(CurrentRotation, TargetRotation, TargetingSpeed));
}

void ATurret::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

bool ATurret::IsPLayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}

bool ATurret::CanFire()
{
	FVector TargetingDir = TurretMesh->GetForwardVector();
	FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	DirToPlayer.Normalize();

	float AimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(TargetingDir, DirToPlayer)));
	return AimAngle <= Accuracy;
}

void ATurret::SetupCannon()
{
	if (!CannonClass)
	{
		return;
	}

	FActorSpawnParameters params;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

}