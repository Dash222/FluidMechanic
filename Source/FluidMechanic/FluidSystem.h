// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FluidSystemConfig.h"
#include "ParticleData.h"
#include "GameFramework/Actor.h"
#include "FluidSystem.generated.h"

USTRUCT()
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

	UPROPERTY(BlueprintReadWrite)
	TMap<int, TArray<int>> ParticlesHashMap;

	UPROPERTY(BlueprintReadWrite)
	TArray<FParticleData> ParticleDatas;

	UPROPERTY(BlueprintReadWrite)
	TArray<FParticleContact> ParticleContacts;

	//Helper Functions

	UFUNCTION()
	void UpdateHashMap();

	UFUNCTION()
	int GetKey(FVector Location);

	UFUNCTION()
	void ComputeContact(int keyA, int KeyB);
	
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
	
	// Rendering

	UFUNCTION(BlueprintNativeEvent)
	void UpdateRenderData();
};