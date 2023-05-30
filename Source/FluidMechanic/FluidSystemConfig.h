// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FluidSystemConfig.generated.h"

/**
 * 
 */
UCLASS()
class FLUIDMECHANIC_API UFluidSystemConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	float Radius = 0.2f;

	UPROPERTY(EditAnywhere)
	float RestDensity = 0.59f;

	UPROPERTY(EditAnywhere)
	float Stiffness = 500.0f;

	UPROPERTY(EditAnywhere)
	float ParticleRadiusRatio = 3.0f;

	UPROPERTY(EditAnywhere)
	float Viscosity = 0.1f;

	UPROPERTY(EditAnywhere)
	float MaxSpeed = 10.f;

	UPROPERTY(EditAnywhere)
	float MaxAcceleration = 900.f;

	UPROPERTY(EditAnywhere)
	float TimeScale = 1.0f;

	UPROPERTY(EditAnywhere)
	float WallFriction = 0.4f;

	UPROPERTY(EditAnywhere)
	float WallRestitution = 0.4;

	UPROPERTY(EditAnywhere)
	float Mass = 1.0f;

	UPROPERTY(EditAnywhere)
	FVector MinCoords;

	UPROPERTY(EditAnywhere)
	FVector MaxCoords;
	
	UPROPERTY(EditAnywhere)
	FIntVector CellSize = FIntVector{10};
};
