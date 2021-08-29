// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

#include <DrawDebugHelpers.h>
#include "TankPawn.h"

ATankPlayerController::ATankPlayerController()
{
    bShowMouseCursor = true;
}

void ATankPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
    InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);
    InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);
}

void ATankPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
	
    FVector MouseDirection;
    DeprojectMousePositionToWorld(MousePos, MouseDirection);
	
    FVector PawnPos = TankPawn->GetActorLocation();
    MousePos.Z = PawnPos.Z;
    FVector Dir = MousePos - PawnPos;
    Dir.Normalize();
    MousePos = PawnPos + Dir * 1000.f;
	
    DrawDebugLine(GetWorld(), PawnPos, MousePos, FColor::Green, false, 0.1f, 0.f, 5.f);
}



void ATankPlayerController::BeginPlay()
{
    Super::BeginPlay();

    TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::MoveForward(float AxisValue)
{

    TankPawn->MoveForward(AxisValue);
}

void ATankPlayerController::RotateRight(float AxisValue)
{

    TankPawn->RotateRight(AxisValue);
}

void ATankPlayerController::Fire()
{
    TankPawn->Fire();
}

