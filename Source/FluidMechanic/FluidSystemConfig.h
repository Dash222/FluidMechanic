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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RestDensity = 0.59f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stiffness = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ParticleRadiusRatio = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Viscosity = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAcceleration = 900.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallFriction = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRestitution = 0.4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mass = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MinCoords;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MaxCoords;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntVector CellSize = FIntVector{10};
};
