// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "DamageTaker.h"
#include "GameStruct.h"

AProjectile::AProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;

    USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = SceneComp;

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(SceneComp);
    ProjectileMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
    ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
}

void AProjectile::Start()
{
    GetWorld()->GetTimerManager().SetTimer(MovementTimer, this, &AProjectile::Move, MoveRate, true, 0.0f);
}

void AProjectile::Move()
{
    FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
    SetActorLocation(nextPosition);
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* owner = GetOwner();
	AActor* OwnerByOwner = owner != nullptr ? owner->GetOwner() : nullptr;
	UE_LOG(LogTemp, Warning, TEXT("Owner %s"), GetOwner());
	if (OtherActor != owner || OtherActor != OwnerByOwner)
	{
		IDamageTaker* DamageActor = Cast<IDamageTaker>(OtherActor);
		if (DamageActor)
		{
			FDamageData damageData;
			damageData.DamageValue = Damage;
			damageData.Instigator = owner;
			damageData.DamageMaker = this;

			DamageActor->TakeDamage(damageData);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlapped actor: %s"), *OtherActor->GetName());
			OtherActor->Destroy();
		}
		Destroy();
	}
}

