// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"


UTankTrack::UTankTrack() {
    PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay() {
    Super::BeginPlay();
    OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
//    UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
    // Drive the tracks
    // Apply sideways force
    DriveTrack();
    ApplySidewaysForce();
    CurrentThrottle = 0;
//    UE_LOG(LogTemp, Warning, TEXT("OnHit"));
}

void UTankTrack::ApplySidewaysForce() {
    auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
    
    auto DeltaTime = GetWorld()->GetDeltaSeconds();
    auto CorrectionAccerleration = -SlippageSpeed / DeltaTime * GetRightVector();
    
    // Calculate and apply sideways for (F = m a)
    auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
    auto CorrectionForce = (TankRoot->GetMass() * CorrectionAccerleration) / 2; // Two Tracks
    TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::SetThrottle(float Throttle) {
    CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, 1);
//    UE_LOG(LogTemp, Warning, TEXT("SetThrottle"));
}

void UTankTrack::DriveTrack() {
    auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
    auto ForceLocation = GetComponentLocation();
    auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
    TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
//    UE_LOG(LogTemp, Warning, TEXT("DriveTrack"));
}


