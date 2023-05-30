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

	UPROPERTY(EditAnywhere)
	float Radius;

	UPROPERTY(EditAnywhere)
	float RestDensity;

	UPROPERTY(EditAnywhere)
	float Stiffness;

	UPROPERTY(EditAnywhere)
	float ParticleRadiusRatio;

	UPROPERTY(EditAnywhere)
	float Viscosity;

	UPROPERTY(EditAnywhere)
	float MaxSpeed;

	UPROPERTY(EditAnywhere)
	float MaxAcceleration;

	UPROPERTY(EditAnywhere)
	float TimeScale;

	UPROPERTY(EditAnywhere)
	float WallFriction;

	UPROPERTY(EditAnywhere)
	float WallRestitution;

	UPROPERTY(EditAnywhere)
	float Mass;

	UPROPERTY(EditAnywhere)
	FVector MinCoords;

	UPROPERTY(EditAnywhere)
	FVector MaxCoords;
};
