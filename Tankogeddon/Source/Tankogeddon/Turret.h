 // Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTaker.h"
#include "Turret.generated.h"

class UStaticMeshComponent;
class ACannon;
UCLASS()
class TANKOGEDDON_API ATurret : public AActor, public IDamageTaker
{
	GENERATED_BODY()

public:
	ATurret();
	
	UFUNCTION()
		void TakeDamage(FDamageData DamageData) override;

	UFUNCTION()
		void Die();

	UFUNCTION()
		void DamageTaken(float Value);

protected:

	void SetupCannon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* HitCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		TSubclassOf<ACannon> CannonClass;

	UPROPERTY()
		ACannon* Cannon;

	UPROPERTY()
		APawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		float TargetingRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		float TargetingSpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		float TargetingRate = 0.005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		float Accuracy = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UHealthComponent* HealthComponent;

	const FString BodyMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";


	virtual void BeginPlay() override;

	void Targeting();
	void RotateToPlayer();
	void Fire();
	bool IsPLayerInRange();
	bool CanFire();
};