// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank() {
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void ATank::BeginPlay() {
    Super::BeginPlay();

    PlayerControllerRef = Cast<APlayerController>(GetController());
}

void ATank::Tick(float DeltaTime)  {
    Super::Tick(DeltaTime);

    if (PlayerControllerRef) {
        FHitResult HitResult;

        PlayerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,
                                                    false,
                                                    HitResult);

        DrawDebugSphere(GetWorld(),
                        HitResult.ImpactPoint,
                        25.f,
                        12,
                        FColor::Red,
                        false,
                        -1.f);

        RotateTurret(HitResult.ImpactPoint);
    }
}

void ATank::Move(float Value) {
    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(DeltaLocation, true);
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
}

void ATank::Turn(float Value) {
    FRotator DeltaRotation = FRotator::ZeroRotator;
    // Yaw = Value * DeltaTime * TurnRate
    DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation, true);
}
