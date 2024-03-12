#include "BoidManager.h"
#include "Boid.h"

ABoidManager::ABoidManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Spawn the boids
void ABoidManager::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(BoidClass))
		Boids = SpawnBoids(BoidClass, SpawnCount, GetActorLocation(), SpawnRadius);
}

// Update all the boids
void ABoidManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateBoids();
}

// Updates all boid properties based on boids' current Position and Forward vector
void ABoidManager::UpdateBoids()
{
	if (Boids.Num() != 0)
	{
		for (int i = 0; i < Boids.Num(); i++)
		{
			Boids[i]->VisibleFlockmates = 0;
			Boids[i]->AvgFlockHeading = FVector::ZeroVector;
			Boids[i]->CenterOfFlockmates = FVector::ZeroVector;
			Boids[i]->AvgAvoidanceHeading = FVector::ZeroVector;

			// Recalc boid properties
			for (int j = 0; j < Boids.Num(); j++)
			{
				if (i == j) continue;
				FVector Offset = Boids[j]->Position - Boids[i]->Position;
				float SqrDst = Offset.X * Offset.X + Offset.Y * Offset.Y + Offset.Z * Offset.Z;

				// Is boid j in range of boid i?
				if (SqrDst < Settings.PerceptionRadius * Settings.PerceptionRadius)
				{
					Boids[i]->VisibleFlockmates += 1;
					Boids[i]->AvgFlockHeading += Boids[j]->Forward;
					Boids[i]->CenterOfFlockmates += Boids[j]->Position;

					// Is boid j too close to boid i?
					if (SqrDst < Settings.AvoidanceRadius * Settings.AvoidanceRadius && SqrDst > 0)
					{
						Boids[i]->AvgAvoidanceHeading -= Offset / SqrDst;
					}
				}
			}
			Boids[i]->UpdateBoid();
		}
	}
}

// Spawn boids randomly within a sphere
TArray<ABoid*> ABoidManager::SpawnBoids(TSubclassOf<ABoid> Class, int Count, FVector Location, float Radius) const
{
	TArray<ABoid*> SpawnedBoids;
	for (int i = 0; i < SpawnCount; i++)
	{
		FActorSpawnParameters SpawnParams;
		FVector Loc = GetRandomPointInSphere(Location, Radius);
		ABoid* Boid = GetWorld()->SpawnActor<ABoid>(Class, Loc, GetActorRotation(), SpawnParams);
		Boid->Initialize(Settings, Target);
		SpawnedBoids.Add(Boid);
	}
	return SpawnedBoids;
}

// Gets a random point within a sphere
FVector ABoidManager::GetRandomPointInSphere(FVector Center, float Radius) const
{
	return Center + Radius * FVector(
		FMath::RandRange(-1.f, 1.f),
		FMath::RandRange(-1.f, 1.f),
		FMath::RandRange(-1.f, 1.f));
}

