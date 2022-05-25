#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParticleGenerator.generated.h"

UCLASS()
class PARTICLEZOOM_API AParticleGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AParticleGenerator();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
