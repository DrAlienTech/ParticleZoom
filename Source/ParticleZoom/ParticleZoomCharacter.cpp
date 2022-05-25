#include "ParticleZoomCharacter.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AParticleZoomCharacter

AParticleZoomCharacter::AParticleZoomCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent->SetWorldScale3D(FVector(0.48f));
	GetCapsuleComponent()->InitCapsuleSize(0.48 * 53.0f, 0.48 * 53.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;	
	GetCharacterMovement()->RotationRate = FRotator(200.0f, 540.0f, 200.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->MaxWalkSpeed = 120000.0f;
	// GetCharacterMovement()->bForceMaxAccel = true;

	GetCharacterMovement()->GravityScale = 0;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	SetActorEnableCollision(true);

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AParticleZoomCharacter::OnCollision);
}

void AParticleZoomCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AParticleZoomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveForward(150.0f);
	
	// AddControllerYawInput(-0.062f * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

//////////////////////////////////////////////////////////////////////////
// Input

void AParticleZoomCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AParticleZoomCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("MoveRight", this, &AParticleZoomCharacter::TurnAtRate);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	// PlayerInputComponent->BindAxis("TurnRate", this, &AParticleZoomCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	// PlayerInputComponent->BindAxis("LookUpRate", this, &AParticleZoomCharacter::LookUpAtRate);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AParticleZoomCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AParticleZoomCharacter::TouchStopped);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AParticleZoomCharacter::OnResetVR);
}


void AParticleZoomCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AParticleZoomCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AParticleZoomCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AParticleZoomCharacter::TurnAtRate(float Rate)
{	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, 15000.0f * Rate);
}

void AParticleZoomCharacter::LookUpAtRate(float Rate)
{
	FVector NewLocation = GetActorLocation();
	NewLocation.Z += 8.31f * Rate;
	SetActorLocation(NewLocation);
}

void AParticleZoomCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(Rotation.Pitch, Rotation.Yaw, Rotation.Roll);
		
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AParticleZoomCharacter::MoveRight(float Value)
{
	// if ( (Controller != nullptr) && (Value != 0.0f) )
	// {
	// 	const FRotator Rotation = Controller->GetControlRotation();
	// 	const FRotator YawRotation(0, Rotation.Yaw, 0);
	//
	// 	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// 	AddMovementInput(Direction, Value);
	// }
}

void AParticleZoomCharacter::OnCollision(UPrimitiveComponent* SelfActor, AActor* OtherActor, UPrimitiveComponent* a, FVector b, const FHitResult& c)
{
	UE_LOG(LogTemp, Warning, TEXT("Collision occurred with %s"), *OtherActor->GetHumanReadableName());

	// GetCharacterMovement()->MaxAcceleration = 0.0f;
	// GetCharacterMovement()->Velocity = FVector(0, 0, 0);
}