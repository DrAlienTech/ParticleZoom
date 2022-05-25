#include "ParticleGenerator.h"

#include "Particle.h"

AParticleGenerator::AParticleGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AParticleGenerator::BeginPlay()
{
	Super::BeginPlay();
}

void AParticleGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FMath::RandRange(0.0f, 100.0f) < 0.519f * FMath::Pow(GetGameTimeSinceCreation(), 0.25f))
	{	
		FActorSpawnParameters SpawnInfo;
		
		AParticle* Particle = GetWorld()->SpawnActor<AParticle>(AParticle::StaticClass(), FVector(0.0f), FRotator(0.0f), SpawnInfo);
	}
}
