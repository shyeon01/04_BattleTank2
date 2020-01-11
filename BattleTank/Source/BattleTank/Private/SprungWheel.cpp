// Copyright EmbraceIT Ltd

#include "SprungWheel.h"
#include "BattleTank.h"


// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PostPhysics;
    
    MassWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("MassWheelConstraint"));
    SetRootComponent(MassWheelConstraint);
    
    Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
    Axle->SetupAttachment(MassWheelConstraint);
    
    Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
    Wheel->SetupAttachment(Axle);
    UE_LOG(LogTemp, Warning, TEXT("Wheel constructor!!"));
    
    AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("AxleWheelConstraint"));
    AxleWheelConstraint->SetupAttachment(Axle);
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();
    
    if(!ensure(Wheel)) {
        UE_LOG(LogTemp, Warning, TEXT("BeginPlay Wheel is NULL"))
        return;
    }
    Wheel->SetNotifyRigidBodyCollision(true);
    Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);
    
    SetupConstraint();
}

void ASprungWheel::SetupConstraint() {
    if(!GetAttachParentActor()) return;
    UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());
    if(!BodyRoot) return;
    MassWheelConstraint->SetConstrainedComponents(BodyRoot, NAME_None, Axle, NAME_None);
    AxleWheelConstraint->SetConstrainedComponents(Axle, NAME_None, Wheel, NAME_None);
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(GetWorld()->TickGroup == TG_PostPhysics) {
        TotalForceMagnitudeThisFrame = 0;
    }
}

void ASprungWheel::AddDrivingForce(float ForceMagnitude) {
    
    TotalForceMagnitudeThisFrame += ForceMagnitude;
    
    if(!ensure(Wheel)) {
        UE_LOG(LogTemp, Warning, TEXT("Wheel2 is NULL"))
        return;
    } else if(!ensure(Axle)) {
        UE_LOG(LogTemp, Warning, TEXT("Axle is NULL"))
        return;
    }
}

void ASprungWheel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
    ApplyForce();
}

void ASprungWheel::ApplyForce() {
    if(!ensure(Wheel)) {
        UE_LOG(LogTemp, Warning, TEXT("ApplyForce() Wheel is NULL"))
        return;
    }
    Wheel->AddForce(Axle->GetForwardVector() * TotalForceMagnitudeThisFrame);
}
