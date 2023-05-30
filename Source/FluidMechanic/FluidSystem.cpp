// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidSystem.h"

// Sets default values
AFluidSystem::AFluidSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFluidSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFluidSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFluidSystem::SpawnParticle(FVector Location, FVector Velocity)
{
}

void AFluidSystem::UpdateHashMap()
{
}

int AFluidSystem::GetKey(FVector Location)
{
	return 0;
}

void AFluidSystem::ComputeContact(int keyA, int KeyB)
{
}

void AFluidSystem::ComputeDensity()
{
}

void AFluidSystem::ComputePressure()
{
}

void AFluidSystem::ComputeSurfaceTension()
{
}

void AFluidSystem::AddPressureForces()
{
}

void AFluidSystem::AddViscosityForces()
{
}

void AFluidSystem::ApplyBorderConstraints()
{
}

void AFluidSystem::Integrate(float DeltaTime)
{
}

void AFluidSystem::UpdateRenderData_Implementation()
{
}

