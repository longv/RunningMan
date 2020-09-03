// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomGuy.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARandomGuy::ARandomGuy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bDead = false;
	Power = 100.0f;
}

// Called when the game starts or when spawned
void ARandomGuy::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARandomGuy::OnBeginOverlap);

	if (Player_Power_Widget_Class != nullptr) {
		Player_Power_Widget = CreateWidget(GetWorld(), Player_Power_Widget_Class);
		Player_Power_Widget->AddToViewport();
	}
}

void ARandomGuy::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName(), false));
}

void ARandomGuy::OnBeginOverlap(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (OtherActor->ActorHasTag("Recharge")) {
		UE_LOG(LogTemp, Warning, TEXT("Collided with"));
		
		Power += 10.0f;

		if (Power > 100.0f) {
			Power = 100.0f;
		}

		OtherActor->Destroy();
	}
}

// Called every frame
void ARandomGuy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Power -= DeltaTime * Power_Threshold;

	if (Power <= 0) {
		if (!bDead) {
			bDead = true;

			GetMesh()->SetSimulatePhysics(true);

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(
				UnusedHandle, this, &ARandomGuy::RestartGame, 3.0f, false
			);
		}
	}
}

// Called to bind functionality to input
void ARandomGuy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARandomGuy::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARandomGuy::MoveRight);
}

void ARandomGuy::MoveForward(float Axis)
{
	if (!bDead) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}

void ARandomGuy::MoveRight(float Axis)
{
	if (!bDead) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
}
