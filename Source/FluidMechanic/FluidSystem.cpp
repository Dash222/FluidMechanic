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

float AFluidSystem::KernelDefault(const float r, const float h)
{
	float h2 = h * h;
	float h4 = h2 * h2;
	float kernel = h2 - r * r;
	return (kernel * kernel * kernel) * (4.0f / (PI * h4 * h4));
}

float AFluidSystem::KernelDefaultGradientFactor(const float r, const float h)
{
	float h2 = h * h;
	float h4 = h2 * h2;
	float kernel = h2 - r * r;
	return -(kernel * kernel) * (6.0f / (PI * h4 * h4));
}

float AFluidSystem::KernelDefaultLaplacian(const float r, const float h)
{
	float h2 = h * h;
	float h4 = h2 * h2;
	float kernel = h2 - r * r;
	return -(kernel * kernel) * (6.0f / (PI * h4 * h4)) * (3.0f * h2 - 7.0f * r * r);
}


float AFluidSystem::KernelSpikyGradientFactorNorm(const float r, const float h)
{
	float h2 = h * h;
	float h5 = h2 * h2 * h;
	float kernel = h - r;
	return kernel * kernel * (-15.0f / (PI * h5));
}

float AFluidSystem::KernelSpikyGradientFactor(const float r, const float h)
{
	float h2 = h * h;
	float h5 = h2 * h2 * h;
	float kernel = h - r;
	return kernel * kernel * (-15.0f / (PI * h5 * r));
}

float AFluidSystem::KernelViscosityLaplacian(const float r, const float h)
{
	float h2 = h * h;
	float kernel = h - r;
	return kernel * (30.0f / (PI * h2 * h2 * h));
}

float AFluidSystem::KernelPoly6hGradientFactor(const float r, const float h)
{
	float h2 = h * h;
	float kernel = h2 - r * r;
	return kernel * kernel * (24.0f / (PI * h2 * h2 * h2 * h * r));
}
