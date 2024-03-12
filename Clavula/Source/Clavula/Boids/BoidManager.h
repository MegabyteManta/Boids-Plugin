// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FBoidSettings.h"
#include "BoidManager.generated.h"

class ABoid;

/*
 * Defines a boid manager.
 * A boid manager spawns all the boids, updates their boid properties, and calls the update function on the boids.
 * It also holds all the settings responsible for boid behavior.
 */
UCLASS(Blueprintable, BlueprintType)
class CLAVULA_API ABoidManager : public AActor
{
	GENERATED_BODY()

protected:
	// The behavior for the boids
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidManager")
	FBoidSettings Settings;

	// The boid class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidManager|Spawning")
	TSubclassOf<ABoid> BoidClass;

	// Number of boids to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=0, UIMax=1000), Category = "BoidManager|Spawning")
	int SpawnCount = 300;

	// Size of sphere with this actor as center to randomly spawn boids in
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=0, UIMax=1000), Category = "BoidManager|Spawning")
	float SpawnRadius = 500;

	// The optional target for the boids
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoidManager")
	AActor* Target = nullptr;

private:
	// The spawned boids
	TArray<ABoid*> Boids;

public:
	ABoidManager();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
private:
	/**
	 * Updates all boids' data
	 */
	void UpdateBoids();

	/**
	 * Spawns boids randomly within a sphere at center Location with radius Radius
	 * @param BoidClass : The class of boid to be spawned. None results in no boids spawning
	 * @param Count : The number of boids to be spawned.
	 * @param Location : The location to spawn the boids
	 * @param Radius : The radius around the location to spawn the boids
	 * @return Array of spawned boids
	 */
	TArray<ABoid*> SpawnBoids(TSubclassOf<ABoid> BoidClass, int Count, FVector Location, float Radius) const;
	
	/**
	 * Gets a random point within a sphere at center Center with radius Radius.
	 * @param Center : Sphere's center
	 * @param Radius : Sphere's radius
	 * @return Random point within sphere
	 */
	FVector GetRandomPointInSphere(FVector Center, float Radius) const;
	
	/*
	// Rep Invariants:
	//	None
	bool CheckRep();
	*/
};
