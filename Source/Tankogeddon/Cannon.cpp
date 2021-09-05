// Fill out your copyright notice in the Description page of Project Settings.

#include "Cannon.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Engine/Engine.h>
#include <TimerManager.h>
#include <Engine/World.h>

#include "Tankogeddon.h"
#include "Projectile.h"
#include <DrawDebugHelpers.h>
#include "ActorPoolSubsystem.h"


// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = RootComp;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
        GEngine->AddOnScreenDebugMessage(9, 1, FColor::Green, "Fire - projectile");
        AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
        if (projectile)
        {
            projectile->Start();
        }

    }
    else
    {
        GEngine->AddOnScreenDebugMessage(9, 1, FColor::Green, "Fire - trace");
        FHitResult hitResult;
        FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
        traceParams.bTraceComplex = true;
        traceParams.bReturnPhysicalMaterial = false;

        FVector start = ProjectileSpawnPoint->GetComponentLocation();
        FVector end = ProjectileSpawnPoint->GetForwardVector() * FireRange + start;
        if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
        {
            DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Red, false, 0.5f, 0, 5);
            if (hitResult.Actor.Get())
            {
                hitResult.Actor.Get()->Destroy();
            }
        }
        else
        {
            DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.5f, 0, 5);
        }
    }

    GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
    UE_LOG(LogTankogeddon, Log, TEXT("Fire ammo left: %d"), AmmoNum);
}

void ACannon::FireSpecial()
{
    if (!bReadyToFire || AmmoNum <= 0 || BurstShotNum)
    {
        UE_LOG(LogTankogeddon, Log, TEXT("GUN NOT READY %d %d"), AmmoNum, BurstShotNum);
        return;
    }
    bReadyToFire = false;

    --AmmoNum;
    BurstShotNum = 3;
    UE_LOG(LogTankogeddon, Log, TEXT("Burst started!"));



    GetWorld()->GetTimerManager().SetTimer(SingleBurstTimer, this, &ACannon::BurstSingleShot, ShotsDelay,true);
    GetWorld()->GetTimerManager().SetTimer(BusrtTime, this, &ACannon::StopBurst, ShotsDelay * BurstShotNum + ShotsDelay / 2, false);

    GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
    UE_LOG(LogTankogeddon, Log, TEXT("FireSpecial ammo left: %d"), AmmoNum);
}



bool ACannon::IsReadyToFire()
{
    return bReadyToFire;
	
}

void ACannon::BurstSingleShot()
{
    UE_LOG(LogTankogeddon, Log, TEXT("Burst shot called!"));
    if (Type == ECannonType::FireProjectile)
    {
        GEngine->AddOnScreenDebugMessage(8, 1, FColor::Red, "Fire special- projectile");
        AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
        if (projectile)
        {
            projectile->Start();
        }

    }
    else
    {
        GEngine->AddOnScreenDebugMessage(8,1, FColor::Red, "Fire special - trace");

    }
}

void ACannon::StopBurst()
{
    BurstShotNum = 0;
    UE_LOG(LogTankogeddon, Log, TEXT("Burst stoped!"));
    GetWorld()->GetTimerManager().ClearTimer(SingleBurstTimer);
}

void ACannon::Reload()
{
    bReadyToFire = true;
    UE_LOG(LogTankogeddon, Log, TEXT("GUN READY"));
    
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

void ACannon::SetVisibility(bool bIsVisible)
{
    Mesh->SetHiddenInGame(!bIsVisible);
}

void ACannon::AddAmmo(int32 InNumAmmo)
{
    AmmoNum = FMath::Clamp(AmmoNum + InNumAmmo, 0, AmmoMax);
    UE_LOG(LogTankogeddon, Log, TEXT("AddAmmo(%d)! AmmoNum: %d"), InNumAmmo, AmmoMax);
}
