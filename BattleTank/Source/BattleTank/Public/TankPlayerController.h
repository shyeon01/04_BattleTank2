// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h" // Must be the last include

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
    
protected:
    UFUNCTION(BlueprintCallable, Category = "Setup")
    ATank* GetControlledTank() const;
	
private:
    virtual void BeginPlay() override;
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    
    // Start the tank moving the barrel so that a shot would hit where
    // the crosshair intersects the world
    void AimTowardsCrosshair();
    
    // Return an OUT parameter, true if hit landscape
    bool GetSightRayHitLocation(FVector& HitLocation) const;
    
    UPROPERTY(EditAnyWhere)
    float CrosshairXLocation = 0.5;
    
    UPROPERTY(EditAnyWhere)
    float CrosshairYLocation = 0.333;
    
    UPROPERTY(EditAnyWhere)
    float LineTraceRange = 1000000;
    
    bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;
    bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;
};
