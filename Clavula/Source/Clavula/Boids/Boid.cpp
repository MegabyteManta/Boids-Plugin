#include "Boid.h"

ABoid::ABoid()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABoid::BeginPlay()
{
	Super::BeginPlay();
	Position = GetActorLocation();
	Forward = GetActorForwardVector();
}

void ABoid::Initialize(FBoidSettings BoidSettings, AActor* BoidTarget)
{
	Settings = BoidSettings;
	Target = BoidTarget;
	float StartSpeed = (Settings.MinSpeed + Settings.MaxSpeed) / 2;
	Velocity = GetActorForwardVector() * StartSpeed;
}

// Calcs boid properties and checks for collision
void ABoid::UpdateBoid(float DeltaTime)
{
	FVector Acceleration = FVector::ZeroVector;

	// Calc target acceleration
	if (IsValid(Target))
	{
		FVector OffsetToTarget = (Target->GetActorLocation() - Position);
		Acceleration = SteerTowards(OffsetToTarget) * Settings.TargetWeight;
	}

	// Calc rest of boid property accelerations
	if (VisibleFlockmates != 0)
	{
		CenterOfFlockmates /= VisibleFlockmates;

		FVector OffsetToFlockmatesCenter = CenterOfFlockmates - Position;

		FVector AlignmentForce = SteerTowards(AvgFlockHeading) * Settings.AlignWeight;
		FVector CohesionForce = SteerTowards(OffsetToFlockmatesCenter) * Settings.CohesionWeight;
		FVector SeparationForce = SteerTowards(AvgAvoidanceHeading) * Settings.SeparateWeight;

		Acceleration += AlignmentForce;
		Acceleration += CohesionForce;
		Acceleration += SeparationForce;
	}

	// Check if boid is heading towards an obstacle
	FHitResult HitResult = CheckHeadingForCollision();
	if (HitResult.bBlockingHit)
	{
		OnHeadedForCollision.Broadcast();
		// -GetActorForwardVector() optimizes collisions without losing too much fidelity
		FVector CollisionAvoidDir = FindUnobstructedDirection();//-GetActorForwardVector();
		// Scales avoidance force higher if the distance to the obstacle is small
		float DistanceScale = (HitResult.Distance == 0 || Settings.CollisionAvoidDst/HitResult.Distance < 1) ? 1 : Settings.CollisionAvoidDst/HitResult.Distance;
		FVector CollisionAvoidForce = SteerTowards(CollisionAvoidDir) * Settings.AvoidCollisionWeight * DistanceScale;
		Acceleration += CollisionAvoidForce;
	}

	// Calc new position and rotation based on acceleration
	
	Velocity += Acceleration * DeltaTime;
	float Speed = Velocity.Size();
	FVector Dir = Velocity / Speed;
	Speed = FMath::Clamp(Speed, Settings.MinSpeed, Settings.MaxSpeed);
	// Effectively clamps Velocity since Speed got clamped
	Velocity = Dir * Speed;

	// Position and Forward are set so the BoidManager can use them to update the boids
	Position = GetActorLocation()+Velocity*DeltaTime;
	Forward = Dir;
	SetActorLocation(Position);
	// Rotation feels too twitchy without an interp
	FRotator Rotation = FMath::RInterpTo(GetActorForwardVector().Rotation(),Dir.Rotation(), DeltaTime, 10.f);
	SetActorRotation(Rotation);
}

// Sphere trace for oncoming collision
FHitResult ABoid::CheckHeadingForCollision() const
{
	// Divide avoidance by 2 so the boids can come closer to the obstacles
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Settings.AvoidanceRadius/2);
	FHitResult OutResult;
	ECollisionChannel TraceChannel = ECC_Pawn;
	FVector SweepStart = GetActorLocation();
	FVector SweepEnd = GetActorLocation() + GetActorForwardVector()*Settings.CollisionAvoidDst;
	FCollisionQueryParams CollisionQueryParams;
	GetWorld()->SweepSingleByChannel(OutResult, SweepStart, SweepEnd, FQuat::Identity, TraceChannel, Sphere);
	return OutResult;
}

// Get a direction which doesn't collide with an obstacle
FVector ABoid::FindUnobstructedDirection() const
{
	for (int i = 0; i < Settings.NumObstacleChecks; i++)
	{
		// Choose a random direction to shoot a ray from
		FVector Direction = FVector(
		FMath::RandRange(-1.f, 1.f),
		FMath::RandRange(-1.f, 1.f),
		FMath::RandRange(-1.f, 1.f)).GetSafeNormal();

		FHitResult OutResult;
		ECollisionChannel TraceChannel = ECC_Pawn;
		FVector SweepStart = GetActorLocation();
		FVector SweepEnd = GetActorLocation() + Direction * Settings.CollisionAvoidDst;

		// Return the first direction that doesn't hit anything
		if (!GetWorld()->LineTraceSingleByChannel(OutResult, SweepStart, SweepEnd, TraceChannel))
		{
			return Direction;
		}
	}
	// Fail and return forward vector
	return GetActorForwardVector();
}

// Returns the scaled direction from the velocity to the inputted vector, i.e. steering towards the vector
FVector ABoid::SteerTowards(FVector Vector) const
{
	FVector V = Vector.GetSafeNormal() * Settings.MaxSpeed - Velocity;
	return V.GetClampedToMaxSize(Settings.MaxSteerForce);
}

bool ABoid::CheckRep() const
{
	ensureMsgf(VisibleFlockmates>=0, TEXT("VisibleFlockmates < 0"));
	return VisibleFlockmates>=0;
}
