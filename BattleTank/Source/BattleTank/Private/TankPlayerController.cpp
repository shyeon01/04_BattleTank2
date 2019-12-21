// Copyright EmbraceIT Ltd

#include "TankAimingComponent.h"
#include "TankPlayerController.h"


void ATankPlayerController::BeginPlay() {
    Super::BeginPlay();
    auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if(!ensure(AimingComponent)) { return; }
    FoundAimingComponent(AimingComponent);
}

// Called every frame
void ATankPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AimTowardsCrosshair();
//    UE_LOG(LogTemp, Warning, TEXT("Player conroller ticking"));
}

void ATankPlayerController::AimTowardsCrosshair() {
    auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
    if(!ensure(AimingComponent)) { return; }
    
    FVector HitLocation;  // Out parameter
    if(GetSightRayHitLocation(HitLocation)) {  // Has "side-effect", is going to line trace
        AimingComponent->AimAt(HitLocation);
    }
}

// Get world location of line trace through crosshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const {
    // Find the crosshair position in pixel coordinates
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);
    auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

    // "De-project" the screen position of the crosshair to a world direction
    FVector LookDirection;
    if(GetLookDirection(ScreenLocation, LookDirection)) {
        // Line-trace along that LookDirection, and see what we hit (up to max range)
        GetLookVectorHitLocation(LookDirection, HitLocation);
    }
    
    return true;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const {
    FHitResult FHitResult;
    auto StartLocation = PlayerCameraManager->GetCameraLocation();
    auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
    
    // Line Trace Succeeds
    if(GetWorld()->LineTraceSingleByChannel(FHitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility)) {
        // Set hit location
        HitLocation = FHitResult.Location;
        return true;
    }
    
    HitLocation = FVector(0);
    return false; // Line trace didn't succed
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const {
    FVector CameraWorldLocation; // To be discarded
    DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);
    
    return true;
}
