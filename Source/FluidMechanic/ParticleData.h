#pragma once
#include "ParticleData.generated.h"

USTRUCT(BlueprintType)
struct FParticleData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	FVector Position;
	
	UPROPERTY(BlueprintReadWrite)
	FVector Velocity;
	
	UPROPERTY(BlueprintReadWrite)
	FVector Acceleration;
	
	UPROPERTY(BlueprintReadWrite)
	float Density;
	
	UPROPERTY(BlueprintReadWrite)
	float Pressure;
	
	UPROPERTY(BlueprintReadWrite)
	FVector SurfaceNormal;
	
	UPROPERTY(BlueprintReadWrite)
	float SurfaceCurvature;
};
