// Copyright EmbraceIT Ltd

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

// Enum for aiming sate
UENUM()
enum class EFiringState: uint8 {
    Reloading,
    Aiming,
    Locked
};

// Forward Declaration
class UTankBarrel;
class UTankTurret;
class AProjectile;

// Holds barrel's properties and Elevate method
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Setup")
    void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

    void AimAt(FVector HitLocation);
    
    UFUNCTION(BlueprintCallable, Category = "Firing")
    void Fire();
    
protected:
    UPROPERTY(BlueprintReadOnly, Category = "State")
    EFiringState FiringState = EFiringState::Locked;
    
private:
    // Sets default values for this component's properties
    UTankAimingComponent();
    
    void MoveBarrelTowards(FVector AimDirection);
    
    UTankBarrel* Barrel = nullptr;
    UTankTurret* Turret = nullptr;
    
    UPROPERTY(EditAnywhere, Category = "Firing")
    float LaunchSpeed = 4000;

    UPROPERTY(EditAnywhere, Category = "Setup")
    TSubclassOf<AProjectile> ProjectileBlueprint; // Alternative
    
    UPROPERTY(EditDefaultsOnly, Category = "Firing")
    float ReloadTimeInSeconds = 2;
    
    double LastFireTime = 0;
	
};
