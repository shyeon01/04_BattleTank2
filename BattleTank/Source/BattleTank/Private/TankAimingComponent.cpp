// Copyright EmbraceIT Ltd

#include "TankAimingComponent.h"
#include "BattleTank.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
    if(RoundsLeft <= 0) {
        FiringState = EFiringState::OutOfAmmo;
    } else if(FPlatformTime::Seconds() - LastFireTime < ReloadTimeInSeconds) {
        FiringState = EFiringState::Reloading;
    } else if(IsBarrelMoving()) {
        FiringState = EFiringState::Aiming;
    } else {
        FiringState = EFiringState::Locked;
    }
    // TODO Handle aiming and locked states
}

void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet) {
    Barrel = BarrelToSet;
    Turret = TurretToSet;
}

int32 UTankAimingComponent::GetRoundsLeft() const {
    return RoundsLeft;
}

EFiringState UTankAimingComponent::GetFiringState() const {
    return FiringState;
}

bool UTankAimingComponent::IsBarrelMoving() {
    if(!ensure(Barrel)) { return false; }
    auto BarrelForward = Barrel->GetForwardVector();
    return !BarrelForward.Equals(AimDirection, 0.1);
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
//	Super::BeginPlay();

	LastFireTime = FPlatformTime::Seconds();
}

// Called every frame
void UTankAimingComponent::AimAt(FVector HitLocation) {
    
    if (!ensure(Barrel)) { return; }
    
    FVector OutLaunchVelocity;
    FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
    bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
    (
        this,
        OutLaunchVelocity,
        StartLocation,
        HitLocation,
        LaunchSpeed,
        false,
        0,
        0,
        ESuggestProjVelocityTraceOption::DoNotTrace // Paramater must be present to prevent bug
//        FCollisionResponseParams::DefaultResponseParam,
//        TArray<AActor*>(),
//        true
    );

    if (bHaveAimSolution)
    {
        AimDirection = OutLaunchVelocity.GetSafeNormal();
        MoveBarrelTowards(AimDirection);
    }
    // If no solution found do nothing
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {
    if(!ensure(Barrel) || !ensure(Turret)) { return; }

    // Work-out difference between current barrel reaction, and AimDirection
    auto BarrelRotator = Barrel->GetForwardVector().Rotation();
    auto AimAsRotator = AimDirection.Rotation();
    auto DeltaRotator = AimAsRotator - BarrelRotator;
    
    // Always yaw the shortest way
    Barrel->Elevate(DeltaRotator.Pitch);
    
    if(FMath::Abs(DeltaRotator.Yaw) < 180){
        Turret->Rotate(DeltaRotator.Yaw);
    } else { // Avoid going the long-way round
        Turret->Rotate(-DeltaRotator.Yaw);
    }
}

void UTankAimingComponent::Fire() {

    if(FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming) {
        // Spawn a projectile at the socket location on the barrel
        if(!ensure(Barrel)) { return; }
        if(!ensure(ProjectileBlueprint)) { return; }
        auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));
        
        Projectile->LaunchProjectile(LaunchSpeed);
        LastFireTime = FPlatformTime::Seconds();
        RoundsLeft--;
    }
}


