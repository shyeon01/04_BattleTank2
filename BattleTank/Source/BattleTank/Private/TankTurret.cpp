// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"


void UTankTurret::Rotate(float RelativeSpeed) {
    RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, +1);
    auto RoatationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
    auto Roatation = RelativeRotation.Yaw + RoatationChange;
    SetRelativeRotation(FRotator(0, Roatation, 0));
    
     UE_LOG(LogTemp, Warning, TEXT("Roatation: %f"), Roatation);
}

