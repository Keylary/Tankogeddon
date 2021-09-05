// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"

class UStaticMeshComponent;
class UArrowComponent;
class AProjectile;

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UArrowComponent* ProjectileSpawnPoint;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
        TSubclassOf<AProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    float FireRate = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    float FireRange = 1000.f;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    float FireDamage = 1.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
        float ShotsDelay = 0.5f;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
        int32 AmmoMax = 10;

   

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
        int32 BurstShotNum = 0;
	
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
    ECannonType Type = ECannonType::FireProjectile;

    FTimerHandle ReloadTimerHandle, SingleBurstTimer, BusrtTime;

    bool bReadyToFire = false;

public:
    ACannon();

    void Fire();
    void FireSpecial();
	
    bool IsReadyToFire();
    void BurstSingleShot();
    void StopBurst();

    void SetVisibility(bool bIsVisible);
    void AddAmmo(int32 InNumAmmo);

    int32 AmmoNum = 0;

protected:
    virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void Reload();

  
};

