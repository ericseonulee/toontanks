// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"

void ATower::BeginPlay() {
    Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (InFireRange()) {
        RotateTurret(Tank->GetActorLocation());
    }

}

void ATower::CheckFireCondition() {
    if (InFireRange()) {
        Fire();
    }
}

void ATower::HandleDestruction() {
    Super::HandleDestruction();
    
    Destroy();
}

bool ATower::InFireRange() {
    if (Tank) {
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

        return Distance <= FireRange;
    }
    
    return false;
}