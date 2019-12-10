// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "BattleTank.h"
#include "TankBarrel.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet) {
    Barrel = BarrelToSet;
}

//// Called when the game starts
//void UTankAimingComponent::BeginPlay()
//{
//	Super::BeginPlay();
//
//	// ...
//
//}
//
//
//// Called every frame
//void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed) {
    
    if (!Barrel) { return; }

    FVector OutLaunchVelocity;
    FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
    bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
    (
        this,
        OutLaunchVelocity,
        StartLocation,
        HitLocation,
        LaunchSpeed,
        ESuggestProjVelocityTraceOption::DoNotTrace
     );
    if(bHaveAimSolution)
    {
        auto AimDirection = OutLaunchVelocity.GetSafeNormal();
//        auto TankName = GetOwner()->GetName();
//        UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s"), *TankName, *AimDirection.ToString());
        MoveBarrelTowards(AimDirection);
    }
    // If no solution found do nothing
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {
    // Work-out difference between current barrel reaction, and AimDirection
    auto BarrelRotator = Barrel->GetForwardVector().Rotation();
    auto AimAsRotator = AimDirection.Rotation();
    auto DeltaRotator = AimAsRotator - BarrelRotator;
    UE_LOG(LogTemp, Warning, TEXT("AimAsRotator: %s"), *AimAsRotator.ToString());
    
    Barrel->Elevate(5); // TODO remove magic number
    
    // Move the barrel the right amount this frame
    
    // Given a max elevation speed, and the frame time
    
}
