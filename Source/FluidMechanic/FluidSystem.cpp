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

	MinRadius = Config->Radius * 0.1f;
	float Volume = 4.f/3.f * Config->Radius * Config->Radius * Config->Radius * PI;
	Config->Mass = Volume * Config->RestDensity;

	for (int i = 0; i < ParticleDatas.Num(); i++)
	{
		ParticleDatas[i].Density = Config->RestDensity;
	}
}

// Called every frame
void AFluidSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeltaTime *= Config->TimeScale;
	// DeltaTime = FMath::Min(DeltaTime, 1.0f/ (200.0f * DeltaTime));

	for (FParticleData& ParticleData : ParticleDatas)
	{
		ParticleData.Acceleration = FVector::ZeroVector;
	}
	
	UpdateHashMap();
	FindContacts();
	
	ComputeDensity();
	ComputePressure();
	ComputeSurfaceTension();

	AddPressureForces();
	// AddViscosityForces();

	Integrate(DeltaTime);
	ApplyBorderConstraints();

	UpdateRenderData();
}

void AFluidSystem::SpawnParticle(FVector Location, FVector Velocity)
{
	ParticleDatas.Add({Location, Velocity});
}

void AFluidSystem::UpdateHashMap()
{
	ParticlesHashMap.Empty();

	for (int i = 0; i < ParticleDatas.Num(); i++)
	{
		int Key = GetHashKey(ParticleDatas[i].Location);
		if (!ParticlesHashMap.Contains(Key))
		{
			ParticlesHashMap.Emplace(Key);
		}

		ParticlesHashMap[Key].ParticlesKey.Add(i);
	}
}

int AFluidSystem::GetHashKey(const FVector& Location) const
{
	// Primes for hashing coordinates
	static constexpr int P1 = 73856093;
	static constexpr int P2 = 19349663;
	static constexpr int P3 = 83492791;


	const FIntVector CellCoord = FIntVector{Location / Config->CellSize.X};

	return (CellCoord.X * P1) ^ (CellCoord.Y * P2) ^ (CellCoord.Z * P3);
}

void AFluidSystem::FindContacts()
{
	ParticleContacts.Empty();

	for (int ParticleIdx = 0; ParticleIdx < ParticleDatas.Num(); ParticleIdx++)
	{
		CheckNeighbours(ParticleIdx);
	}
}

void AFluidSystem::CheckNeighbours(int ParticleIdx)
{
	const FParticleData& ParticleData = ParticleDatas[ParticleIdx];
	for (int X = -1; X <= 1; X++)
	{
		for (int Y = -1; Y <= 1; Y++)
		{
			for (int Z = -1; Z <= 1; Z++)
			{
				int Key = GetHashKey(ParticleData.Location + FVector{
					static_cast<float>(X), static_cast<float>(Y), static_cast<float>(Z)
				});

				if (!ParticlesHashMap.Contains(Key))
					continue;
				
				for (const int idx : ParticlesHashMap[Key].ParticlesKey)
				{
					if (ParticleIdx != idx)
						ComputeContact(ParticleIdx, idx);
				}
			}
		}
	}
}

void AFluidSystem::ComputeContact(int idxA, int idxB)
{
	FParticleData& ParticleDataA = ParticleDatas[idxA];
	FParticleData& ParticleDataB = ParticleDatas[idxB];

	float sqrLen = (ParticleDataB.Location - ParticleDataA.Location).SizeSquared();
	
	if (sqrLen <= Config->Radius * Config->Radius)
	{
		ParticleContacts.Push({idxA, idxB, FMath::Clamp(FMath::Sqrt(sqrLen), MinRadius, Config->Radius)});
	}
}

void AFluidSystem::ComputeDensity()
{
	const float Radius = Config->Radius;
	//float Mass = Config->Mass;

	const float BaseWeight = KernelDefault(0.0f, Radius);
	for (FParticleData& ParticleData : ParticleDatas)
	{
		ParticleData.Density = BaseWeight;
	}

	for (FParticleContact& ParticleContact : ParticleContacts)
	{
		//const FVector& aPos = ParticleDatas[ParticleContact.KeyA].Location;
		//const FVector& bPos = ParticleDatas[ParticleContact.KeyB].Location;

		const float Weight = KernelDefault(ParticleContact.Distance, Radius);
		ParticleDatas[ParticleContact.KeyA].Density += Weight;
		ParticleDatas[ParticleContact.KeyB].Density += Weight;
	}

	for (FParticleData& ParticleData : ParticleDatas)
	{
		ParticleData.Density *= Config->Mass;
	}
}

void AFluidSystem::ComputePressure()
{
	for (int i = 0; i < ParticleDatas.Num(); i++)
	{
		ParticleDatas[i].Pressure = Config->Stiffness * (ParticleDatas[i].Density - Config->RestDensity);
	}
}

void AFluidSystem::ComputeSurfaceTension()
{
	const float Radius = Config->Radius * 1.5f;// 3.0f;
	const float Mass = Config->Mass;

	// En faisant ca on perd sa valeur precedente, on fait ca de cette maniere parce que on utilise des pairs.
	for (FParticleData& ParticleData : ParticleDatas)
	{
		ParticleData.SurfaceNormal = FVector::ZeroVector;
		ParticleData.SurfaceCurvature = -(Mass / ParticleData.Density) * KernelDefaultLaplacian(0.0f, Radius);
	}

	for (FParticleContact& ParticleContact : ParticleContacts)
	{
		const FVector& APos = ParticleDatas[ParticleContact.KeyA].Location;
		const FVector& BPos = ParticleDatas[ParticleContact.KeyB].Location;

		FVector R = APos - BPos;
		FVector Gradient = R * KernelDefaultGradientFactor(ParticleContact.Distance, Radius);

		ParticleDatas[ParticleContact.KeyA].SurfaceNormal += Gradient * (Mass / ParticleDatas[ParticleContact.KeyA].Density);
		ParticleDatas[ParticleContact.KeyB].SurfaceNormal += Gradient * -(Mass / ParticleDatas[ParticleContact.KeyB].Density);

		const float Laplacian = KernelDefaultLaplacian(ParticleContact.Distance, Radius);
		ParticleDatas[ParticleContact.KeyA].SurfaceCurvature += -(Mass / ParticleDatas[ParticleContact.KeyA].Density) * Laplacian;
		ParticleDatas[ParticleContact.KeyB].SurfaceCurvature += -(Mass / ParticleDatas[ParticleContact.KeyB].Density) * Laplacian;
	}

	//float l = 0.5f; // 1f;
	//for (FParticleData& ParticleData : ParticleDatas)
	//{
	//	const float nSqrNorm = ParticleData.SurfaceNormal.SizeSquared();
	//	if (nSqrNorm >= l * l)
	//	{
	//		FVector NormalizedSurfaceNormal = ParticleData.SurfaceNormal;
	//		NormalizedSurfaceNormal.Normalize();
	//		FVector tensionForce = NormalizedSurfaceNormal * ParticleData.SurfaceCurvature * 5.0f;
	//	}
	//}
}

void AFluidSystem::AddPressureForces()
{
	const float Radius = Config->Radius;
	const float Mass = Config->Mass;

	for (FParticleContact& ParticleContact : ParticleContacts)
	{
		const FParticleData& ParticleDataA = ParticleDatas[ParticleContact.KeyA];
		const FParticleData& ParticleDataB = ParticleDatas[ParticleContact.KeyB];
		
		const FVector& APos = ParticleDatas[ParticleContact.KeyA].Location;
		const FVector& BPos = ParticleDatas[ParticleContact.KeyB].Location;

		FVector R = APos - BPos;
		const float Length = ParticleContact.Distance;


		float PressureAcc = -Mass * (ParticleDataA.Pressure + ParticleDataB.Pressure) / 2.0f;
		// PressureAcc +=  0.02f * Mass * (Config->Stiffness * (ParticleDataA.Density + ParticleDataB.Density)) * dW;

		// PressureMap[ParticleContact.KeyA] += (Pressure / (2.0f * ParticleDataB.Density)) * KernelSpikyGradientFactorVector(ParticleDataB.Location - ParticleDataA.Location, Radius);
		
		ParticleDatas[ParticleContact.KeyA].Acceleration += PressureAcc / (ParticleDataB.Density) * KernelSpikyGradientFactorVector(R, Radius) * 100.f;
		ParticleDatas[ParticleContact.KeyB].Acceleration += PressureAcc / (ParticleDataA.Density) * KernelSpikyGradientFactorVector(-R, Radius) * 100.f;
	}

	
}

void AFluidSystem::AddViscosityForces()
{
	const float Radius = Config->Radius;
	const float Mass = Config->Mass;
	const float Viscosity = Config->Viscosity;

	for (FParticleContact& ParticleContact : ParticleContacts)
	{
		FParticleData& ParticleDataA = ParticleDatas[ParticleContact.KeyA];
		FParticleData& ParticleDataB = ParticleDatas[ParticleContact.KeyB];
		
		const FVector& APos = ParticleDatas[ParticleContact.KeyA].Location;
		const FVector& BPos = ParticleDatas[ParticleContact.KeyB].Location;

		FVector DeltaVel = ParticleDataA.Velocity - ParticleDataB.Velocity;
		const FVector ViscosityAcc = DeltaVel * -Mass * (Viscosity / (2.0f * ParticleDataA.Density * ParticleDataB.Density)) * KernelViscosityLaplacian(ParticleContact.Distance, Radius);
		// ViscosityMap[ParticleContact.KeyA] += (-DeltaVel)/ParticleDataB.Density * KernelViscosityLaplacian(ParticleContact.Distance, Radius);
		
		ParticleDataA.Acceleration += ViscosityAcc;
		ParticleDataB.Acceleration -= ViscosityAcc;
	}
}

void AFluidSystem::ApplyBorderConstraints()
{
	const float Restitution = Config->WallRestitution;
	const float Friction = Config->WallFriction;

	for (FParticleData& ParticleData : ParticleDatas)
	{
		FVector& Location = ParticleData.Location;
		if (Location.X <= Config->MinCoords.X && ParticleData.Velocity.X < 0.0f)
		{
			Location.X = Config->MinCoords.X;
			ParticleData.Velocity.X *= -Restitution;
			ParticleData.Velocity.Y *= Friction;
			ParticleData.Velocity.Z *= Friction;
		}
		else if (Location.X >= Config->MaxCoords.X && ParticleData.Velocity.X > 0.0f)
		{
			Location.X = Config->MaxCoords.X;
			ParticleData.Velocity.X *= -Restitution;
			ParticleData.Velocity.Y *= Friction;
			ParticleData.Velocity.Z *= Friction;
		}

		if (Location.Y <= Config->MinCoords.Y  && ParticleData.Velocity.Y < 0.0f)
		{
			Location.Y = Config->MinCoords.Y;
			ParticleData.Velocity.Y *= -Restitution;
			ParticleData.Velocity.Z *= Friction;
			ParticleData.Velocity.X *= Friction;
		}
		else if (Location.Y >= Config->MaxCoords.Y  && ParticleData.Velocity.Y > 0.0f)
		{
			Location.Y = Config->MaxCoords.Y;
			ParticleData.Velocity.Y *= -Restitution;
			ParticleData.Velocity.Z *= Friction;
			ParticleData.Velocity.X *= Friction;
		}

		if (Location.Z <= Config->MinCoords.Z  && ParticleData.Velocity.Z < 0.0f)
		{
			Location.Z = Config->MinCoords.Z;
			ParticleData.Velocity.Z *= -Restitution;
			ParticleData.Velocity.X *= Friction;
			ParticleData.Velocity.Y *= Friction;
		}
		else if (Location.Z >= Config->MaxCoords.Z  && ParticleData.Velocity.Z > 0.0f)
		{
			Location.Z = Config->MaxCoords.Z;
			ParticleData.Velocity.Z *= -Restitution;
			ParticleData.Velocity.X *= Friction;
			ParticleData.Velocity.Y *= Friction;
		}
	}
}

void AFluidSystem::Integrate(float DeltaTime)
{
	for (FParticleData& ParticleData : ParticleDatas)
	{
		if(ParticleData.Acceleration.Size() > Config->MaxAcceleration)
		{
			ParticleData.Acceleration.Normalize();
			ParticleData.Acceleration *= Config->MaxAcceleration;
		}
		FVector Gravity {0.0f, 0.0f, -981.f};
		ParticleData.Velocity += (ParticleData.Acceleration + Gravity) * DeltaTime;
		
		if(ParticleData.Velocity.Size() > Config->MaxSpeed)
		{
			ParticleData.Velocity.Normalize();
			ParticleData.Velocity *= Config->MaxSpeed;
		}
		
		ParticleData.Location += ParticleData.Velocity * DeltaTime;
	}
}

void AFluidSystem::UpdateRenderData_Implementation()
{
}

float AFluidSystem::KernelDefault(float R, float H)
{

	const float H2 = H * H;
	const float R2 = R * R;
	float Res = 315.0f / (64.0f * PI * FMath::Pow(H, 9));
	if(0.0f <= abs(R) && abs(R) <= H)
		return  Res * FMath::Pow(H2 - R2, 3);
	
	if(abs(R) > H)
		return 0.0f;

	return 0.0f;
	
	//const float H4 = H2 * H2;
	//const float Kernel = H2 - R * R;
	//return (Kernel * Kernel * Kernel) * (4.0f / (PI * H4 * H4));
}

float AFluidSystem::KernelDefaultGradientFactor(float R, float H)
{

	const float H2 = H * H;
	const float R2 = R * R;

	return -945.0f / (32.0f * PI * FMath::Pow(H, 9)) * R *  FMath::Pow(H2 - R2, 2);
	
	//const float H4 = H2 * H2;
	//const float Kernel = H2 - R * R;
	//return -(Kernel * Kernel) * (6.0f / (PI * H4 * H4));
}

float AFluidSystem::KernelDefaultLaplacian(float R, float H)
{

	const float H2 = H * H;
	const float R2 = R * R;

	return -945.0f / (32.0f * PI * FMath::Pow(H, 9)) * (H2 - R2) * (3 * H2 - 7 * R2);
	
	//const float H4 = H2 * H2;
	//const float Kernel = H2 - R * R;
	//return -(Kernel * Kernel) * (6.0f / (PI * H4 * H4)) * (3.0f * H2 - 7.0f * R * R);
}


float AFluidSystem::KernelSpikyGradientFactorNorm(float R, float H)
{

	float Res = 15.0f / (PI * FMath::Pow(H, 6));
	
	if(0.0f <= abs(R) && abs(R) <= H)
		return Res * FMath::Pow(H - abs(R), 3);
	else if(abs(R) > H)
		return 0.0f;
	
	return 0.0f;
	//const float H2 = H * H;
	//const float H5 = H2 * H2 * H;
	//const float Kernel = H - R;
	//return Kernel * Kernel * (-15.0f / (PI * H5));
}

float AFluidSystem::KernelSpikyGradientFactor(float R, float H)
{
	return -45.0f / (PI * FMath::Pow(H, 6)) * (R / abs(R)) * FMath::Pow(H - abs(R), 2);
	//const float H2 = H * H;
	//const float H5 = H2 * H2 * H;
	//const float Kernel = H - R;
	//return Kernel * Kernel * (-15.0f / (PI * H5 * R));
}

FVector AFluidSystem::KernelSpikyGradientFactorVector(FVector R, float H)
{
	return -45.0f / (PI * FMath::Pow(H, 6)) * R.GetSafeNormal() * FMath::Pow(H - R.Size(), 2);
}

float AFluidSystem::KernelViscosityLaplacian(float R, float H)
{
	return 45.0f / (PI * FMath::Pow(H, 6)) * (H - R);
	//const float H2 = H * H;
	//const float Kernel = H - R;
	//return Kernel * (30.0f / (PI * H2 * H2 * H));
}

float AFluidSystem::KernelPoly6hGradientFactor(float R, float H)
{
	const float H2 = H * H;
	const float Kernel = H2 - R * R;
	return Kernel * Kernel * (24.0f / (PI * H2 * H2 * H2 * H * R));
}
