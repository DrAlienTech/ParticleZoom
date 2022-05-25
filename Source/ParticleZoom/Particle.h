#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particle.generated.h"

UCLASS()
class PARTICLEZOOM_API AParticle : public AActor
{
	GENERATED_BODY()
	
public:
	AParticle();

protected:
	virtual void BeginPlay() override;

public:
	float XYDisplacement = 0.0f;
	float ZDisplacement = 0.0f;
	
	float Radius = 0.0f;
	float Angle = 0.0f;

	float SpawnLocationX;
	float SpawnLocationY;
	float SpawnLocationZ;

	virtual void Tick(float DeltaTime) override;

	void Loop();
};
