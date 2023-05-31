// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FluidSystemConfig.h"
#include "ParticleData.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h" 

#include "FluidSystem.generated.h"

USTRUCT(BlueprintType)
struct FParticleContact
{
	GENERATED_BODY()

	UPROPERTY()
	int KeyA;

	UPROPERTY()
	int KeyB;

	UPROPERTY()
	float Distance;
};

USTRUCT(BlueprintType)
struct FGridCell
{
	GENERATED_BODY()

public:
	TArray<int> ParticlesKey;
};

UCLASS()
class FLUIDMECHANIC_API AFluidSystem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFluidSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFluidSystemConfig* Config;

	UFUNCTION(BlueprintCallable)
	void SpawnParticle(FVector Location, FVector Velocity);

private:
	UPROPERTY()
	float MinRadius;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess))
	TMap<int, FGridCell> ParticlesHashMap;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess))
	TArray<FParticleData> ParticleDatas;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess))
	TArray<FParticleContact> ParticleContacts;
	
	//Helper Functions

	UFUNCTION()
	void UpdateHashMap();

	UFUNCTION()
	int GetHashKey(const FVector& Location) const;

	UFUNCTION()
	void FindContacts();

	UFUNCTION()
	void CheckNeighbours(int ParticleIdx);
	
	UFUNCTION()
	void ComputeContact(int idxA, int idxB);
	
	//SPH

	UFUNCTION()
	void ComputeDensity();

	UFUNCTION()
	void ComputePressure();

	UFUNCTION()
	void ComputeSurfaceTension();

	UFUNCTION()
	void AddPressureForces();

	UFUNCTION()
	void AddViscosityForces();

	UFUNCTION()
	void ApplyBorderConstraints();

	UFUNCTION()
	void Integrate(float DeltaTime);

public:
	// Rendering
	UFUNCTION(BlueprintNativeEvent)
	void UpdateRenderData();

	UFUNCTION()
	static float KernelDefault(float R, float H);

	UFUNCTION()
	static float KernelDefaultGradientFactor(const float R, const float H);
	
	UFUNCTION()
	static float KernelDefaultLaplacian(const float R, const float H);
	
	UFUNCTION()
	static float KernelSpikyGradientFactorNorm(const float R, const float H);
	
	UFUNCTION()
	static float KernelSpikyGradientFactor(const float R, const float H);

	UFUNCTION()
	FVector KernelSpikyGradientFactorVector(FVector R, float H);

	UFUNCTION()
	static float KernelViscosityLaplacian(const float R, const float H);
	
	UFUNCTION()
	static float KernelPoly6hGradientFactor(const float R, const float H);
};
