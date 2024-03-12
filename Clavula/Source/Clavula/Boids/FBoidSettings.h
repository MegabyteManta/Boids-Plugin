#pragma once
#include "FBoidSettings.generated.h"

/*
 * Defines all the boid settings
 */
USTRUCT(BlueprintType)
struct CLAVULA_API FBoidSettings {
	GENERATED_BODY()

	// Min speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=0, UIMax=400), Category = "BoidSettings|Speed")
	float MinSpeed = 400.f;

	// Max speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=0, UIMax=500), Category = "BoidSettings|Speed")
	float MaxSpeed = 500.f;

	// How sharp a turn can be
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=0, UIMax=500), Category = "BoidSettings|Speed")
	float MaxSteerForce = 300.f;

	// At what distance do boids start "seeing" each other (can control clump size)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=0, UIMax=500), Category = "BoidSettings|Perception")
	float PerceptionRadius = 200.f;

	// At what distance to boids want to start avoiding each other
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=0, UIMax=250), Category = "BoidSettings|Perception")
	float AvoidanceRadius = 50.f;
	
	// Controls how strongly boids want to go in the same direction as other boids
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=0, UIMax=100), Category = "BoidSettings|Weights")
	float AlignWeight = 1;
	
	// Controls how strongly boids want to stick together
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=0, UIMax=100), Category = "BoidSettings|Weights")
	float CohesionWeight = 1;
	
	// Controls how strongly boids want to separate from each other
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=0, UIMax=100), Category = "BoidSettings|Weights")
	float SeparateWeight = 1;

	// Controls how strongly boids want to go to the target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=0, UIMax=100), Category = "BoidSettings|Weights")
	float TargetWeight = 1;

	// Controls how strongly boids avoid obstacles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=0, UIMax=100), Category = "BoidSettings|Weights")
	float AvoidCollisionWeight = 1.f;

	// Distance when boids start moving away from obstacles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=0, UIMax=1000), Category = "BoidSettings|Obstacles")
	float CollisionAvoidDst = 500.f;
	
	// The number of checks for a clear path from
	// an obstacle before failing. A higher number means
	// boids are less likely to go through obstacles.
	// This is secondary to AvoidCollisionWeight and
	// CollisionAvoidDst, however.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=0, UIMax=1000), Category = "BoidSettings|Obstacles")
	int NumObstacleChecks = 300;
};