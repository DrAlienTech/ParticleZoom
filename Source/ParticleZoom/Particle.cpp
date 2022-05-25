#include "Particle.h"

#include "Components/SphereComponent.h"

AParticle::AParticle()
{
	PrimaryActorTick.bCanEverTick = true;
	
	XYDisplacement = FMath::RandRange(1.0f / 1.02f, 1.02f);
	ZDisplacement = FMath::Sqrt(1904400.0f - FMath::Square(1412.0f / 1.02f * XYDisplacement)) * 1100.0f / 1380.0f;
	Angle = FMath::RandRange(0.0f, 2 * PI) * 180.0f / PI;
	
	SpawnLocationX = 1.2f * (4160.0f * cos(Angle)) * XYDisplacement;
	SpawnLocationY = 1.2f * (4160.0f * sin(Angle) - 2638.0f) * XYDisplacement;
	SpawnLocationZ = 2200.0f /*+ FMath::RandRange(-ZDisplacement, ZDisplacement)*/;
	Radius = FMath::Sqrt(FMath::Square(SpawnLocationX) + FMath::Square(SpawnLocationY));

	FVector SpawnLocation = FVector(SpawnLocationX, SpawnLocationY, SpawnLocationZ);
	SetActorLocation(SpawnLocation, false, nullptr, ETeleportType::TeleportPhysics);

	USphereComponent* ParticleSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ParticleSphereComponent"));
	RootComponent = ParticleSphereComponent;

	UStaticMeshComponent* SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	
	if (SphereMeshAsset.Succeeded()) {
		SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
		
		RootComponent->SetWorldScale3D(FVector(3.12f));
		
		SphereMesh->CastShadow = 0;
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error occurred generating sphere mesh asset for %s"), *GetName());
	}
	
	SetActorEnableCollision(true);

	UE_LOG(LogTemp, Warning, TEXT("%s created at coordinates (%d %d %d) or (%d, %d)"), *GetName(), FMath::RoundToInt(SpawnLocationX), FMath::RoundToInt(SpawnLocationY), FMath::RoundToInt(SpawnLocationZ), FMath::RoundToInt(Radius), FMath::RoundToInt(Angle));
}

void AParticle::BeginPlay()
{
	Super::BeginPlay();
}

void AParticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetGameTimeSinceCreation() > 12)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s destroyed at time %d"), *GetHumanReadableName(), GetGameTimeSinceCreation());
		
		Destroy();
		
		EndPlay(EEndPlayReason::Destroyed);
	} else
	{
		Loop();
	}
}

void AParticle::Loop()
{
	if (XYDisplacement != 0.0f && ZDisplacement != 0.0f && Angle != 0.0f)
	{
		float AngularDisplacement = 0.312f * GetGameTimeSinceCreation();
		float NewLocationX = 1.2f * (4160.0f * cos(Angle + AngularDisplacement)) * XYDisplacement;
		float NewLocationY = 1.2f * (4160.0f * sin(Angle + AngularDisplacement) - 2638.0f) * XYDisplacement;
		float NewLocationZ = SpawnLocationZ/*+ FMath::RandRange(-12.0f, 12.0f)*/;
	
		SetActorLocation(FVector(NewLocationX, NewLocationY, NewLocationZ), true, nullptr, ETeleportType::None);
	}
}