// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"
#include "Components/StaticMeshComponent.h"
#include "Cannon.h"
#include "TankPawn.h"

AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = false;


	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	AmmoBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoBoxMesh"));
	AmmoBoxMesh->SetupAttachment(SceneComp);
	AmmoBoxMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AmmoBoxMesh->SetCollisionProfileName("OverlapAll");
	AmmoBoxMesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);

}

void AAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);
	if (TankPawn) 
	{
		TankPawn->SetupCannon(CannonClass);
		Destroy();
	}
}