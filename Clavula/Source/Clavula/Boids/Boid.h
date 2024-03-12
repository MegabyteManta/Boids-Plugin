// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BoidManager.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHeadedForCollision);

/*
 * Defines a single boid.
 * Contains all the functions to calculate and set movement and direction
 * given the properties that are set by the BoidManager.
 */
UCLASS(Blueprintable, BlueprintType)
class CLAVULA_API ABoid : public AActor
{
	GENERATED_BODY()

public:
	// Delegate called when boid is heading for a collision
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Boids")
	FOnHeadedForCollision OnHeadedForCollision;

	// Position and Forward used in calculations by BoidManager
	FVector Position;
	FVector Forward;

	// Properties calculated by BoidManager, used to steer boid in a direction
	FVector AvgFlockHeading = FVector::ZeroVector;
	FVector AvgAvoidanceHeading = FVector::ZeroVector;
	FVector CenterOfFlockmates = FVector::ZeroVector;
	int VisibleFlockmates = 0;

private:
	
	FBoidSettings Settings;
	FVector Velocity = FVector::ZeroVector;
	UPROPERTY()
	AActor* Target = nullptr;

public:	
	ABoid();

protected:
	virtual void BeginPlay() override;

public:
	/**
	 * Initializes the boids with their initial behavior and target.
	 * @param BoidSettings : The boid behavior
	 * @param BoidTarget : The target the boids may go towards
	 */
	void Initialize(FBoidSettings BoidSettings, AActor* BoidTarget);
	
	/**
	 * Updates their properties, checks for collision, and sets position and rotation.
	 * Called by the manager.
	 * @param DeltaTime : The delta time given by the manager
	 */
	void UpdateBoid(float DeltaTime);
	
private:
	/**
	 * Sphere traces forward from the boid to see if it's heading for a collision.
	 * @return The hit result from the trace
	 */
	FHitResult CheckHeadingForCollision() const;

	/**
	 * Line traces in random directions to find somewhere that isn't heading for a collision.
	 * @return The unobstructed direction, or the boid's forward vector if no direction was found
	 */
	FVector FindUnobstructedDirection() const;

	/**
	 * Finds the vector from the boid's velocity to Vector
	 * @param Vector : The vector to steer towards
	 * @return The clamped vector returned from steering from Velocity to Vector
	 */
	FVector SteerTowards(FVector Vector) const;

	// Rep Invariants:
	//	VisibleFlockmates >= 0
	bool CheckRep() const; 
};
