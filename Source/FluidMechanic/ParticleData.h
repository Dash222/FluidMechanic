#pragma once
#include "ParticleData.generated.h"

USTRUCT(BlueprintType)
struct FParticleData
{
	FParticleData(){}
	FParticleData(const FVector& Location, const FVector& Velocity)
		: Location(Location),
		  Velocity(Velocity)
	{
	}

	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite)
	FVector Velocity = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite)
	FVector Acceleration = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite)
	float Density = 0;
	
	UPROPERTY(BlueprintReadWrite)
	float Pressure = 0;
	
	UPROPERTY(BlueprintReadWrite)
	FVector SurfaceNormal = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite)
	float SurfaceCurvature = 0;
};
