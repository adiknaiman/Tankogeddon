#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"


ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = SceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	CannonMesh->SetupAttachment(SceneComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
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
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Fire - trace");
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

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	bReadyToFire = true;
	Reload();
	
}


