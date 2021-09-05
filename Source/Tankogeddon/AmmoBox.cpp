// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoBox.h"
#include "TankPawn.h"
#include <Components/StaticMeshComponent.h>
#include <Components/SceneComponent.h>
#include <Engine/World.h>
#include "GameFramework/PlayerController.h"
#include "Cannon.h"
#include "Tankogeddon.h"



AAmmoBox::AAmmoBox()
{
    PrimaryActorTick.bCanEverTick = false;
    USceneComponent* SceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = SceeneCpm;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);

    Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);
    Mesh->SetCollisionProfileName(FName("OverlapAll"));
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetGenerateOverlapEvents(true);
}


void AAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ATankPawn* PlayerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (OtherActor == PlayerPawn)
    {
        ACannon* Cannon = PlayerPawn->GetActiveCannon();

        if (Cannon && Cannon->GetClass() == CannonClass)
        {
            Cannon->AddAmmo(AmmoNum);
        }
        else
        {
            PlayerPawn->SetupCannon(CannonClass);
        }

        Destroy();
    }

}