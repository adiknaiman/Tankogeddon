#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"
#include "Projectile.h"
#include "DamageTaker.h"


ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = SceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	CannonMesh->SetupAttachment(SceneComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	bReadyToFire = true;
	Reload();

}

void ACannon::Fire()
{
	if (!IsReadyToFire() || Missile <= 0)
	{
		return;
	}
	bReadyToFire = false;
	Missile--;
	if (CannonType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Fire - projectile");
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation(), spawnParams);
		if (Projectile)
		{
			Projectile->Start();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Fire - trace");

		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams();
		traceParams.AddIgnoredActor(this);
		traceParams.bReturnPhysicalMaterial = false;
		FVector Start = ProjectileSpawnPoint->GetComponentLocation();
		FVector End = Start + ProjectileSpawnPoint->GetForwardVector() * FireRange;

		if (GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), Start, hitResult.Location, FColor::Red, false, 1.0f, 0, 5);
		
			UE_LOG(LogTemp, Warning, TEXT("Owner %s"), GetOwner());
			if (hitResult.GetActor())
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *hitResult.GetActor()->GetName());
				hitResult.GetActor();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.0f, 0, 5);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this,	&ACannon::Reload, FireRate, false);
}

void ACannon::FireSpecial()
{
	if (!IsReadyToFire())
	{
		return;
	}
	bReadyToFire = false;
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Fire - Special");
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);

}

bool ACannon::IsReadyToFire()
{
	return bReadyToFire;
}

void ACannon::AddMissile(int32 newMissile)
{
	Missile += newMissile;
}

void ACannon::Reload()
{
	bReadyToFire = true;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Missiles: %d"), Missile));
}




