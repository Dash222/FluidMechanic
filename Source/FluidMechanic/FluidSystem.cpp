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

void AFluidSystem::UpdateRenderData_Implementation()
{
}

