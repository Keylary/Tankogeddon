// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Tankogeddon.h"


// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = RootComp;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
    Mesh->SetupAttachment(RootComponent);

    ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
    ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ACannon::Fire()
{
    if (!bReadyToFire || AmmoNum <=0)
    {
        return;
    }
    bReadyToFire = false;

    --AmmoNum;
	
    if (Type == ECannonType::FireProjectile)
    {
        GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - projectile");
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Fire - trace");
    }

    GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
    UE_LOG(LogTankogeddon, Log, TEXT("Fire ammo left: %d"), AmmoNum);
}

void ACannon::FireSpecial()
{
    if (!bReadyToFire || AmmoNum <= 0)
    {
        return;
    }
    bReadyToFire = false;

    --AmmoNum;
	
    if (Type == ECannonType::FireProjectile)
    {
        GEngine->AddOnScreenDebugMessage(10, 1, FColor::Red, "Fire special- projectile");

    }
    else
    {
        GEngine->AddOnScreenDebugMessage(10, 1, FColor::Red, "Fire special - trace");

    }

    GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
    UE_LOG(LogTankogeddon, Log, TEXT("FireSpecial ammo left: %d"), AmmoNum);
}

bool ACannon::IsReadyToFire()
{
    return bReadyToFire;
	
}

void ACannon::Reload()
{
    bReadyToFire = true;
    
}

void ACannon::BeginPlay()
{
    Super::BeginPlay();
    AmmoNum = AmmoMax;
    Reload();
}

void ACannon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
	
    GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}
