// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "SCharacter.h"

#include "BlueprintEditor.h"
#include "DiffResults.h"
#include "NavigationSystemTypes.h"
#include "SAttributeComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SInteractionComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//this is a nice method for creating instances of components from a pointer. ONLY available in the constructor.
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}




// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASCharacter::MoveForward(float Val)
{
	FRotator ControlRot = GetControlRotation();
	//we don't want the forward vector to be affected by pitch or roll of the controller
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(), Val);
}

void ASCharacter::MoveRight(float Val)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	//X = forward (red)
	//Y = right (green)
	//Z = up (blue)

	//Rotate the controller's forward vector to the right, so a right turn is relative to the direction the camera is facing
	FVector RVec = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RVec, Val);
}

void ASCharacter::PrimaryAttack()
{

	PlayAnimMontage(AttackAnim);

	//call the attack function after a timer (see PrimaryAttack_TimeElapsed
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	if (ensureAlways(MagicProjectileClass))
	{
		//this is a socket defined on one of the bones in the mesh
		FVector Hand = GetMesh()->GetSocketLocation("Muzzle_01");
		FRotator Rotation = CorrectProjectileAngle(Hand);
		FTransform SpawnTM = FTransform(Rotation, Hand);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		GetWorld()->SpawnActor<AActor>(MagicProjectileClass, SpawnTM, SpawnParams);
		UGameplayStatics::SpawnEmitterAttached(CastEffect, GetMesh(),"Muzzle_01");
	}
}

void ASCharacter::Teleport()
{
	PlayAnimMontage(AttackAnim);

	//call the attack function after a timer (see PrimaryAttack_TimeElapsed
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::Teleport_TimeElapsed, 0.2f);
}

void ASCharacter::Teleport_TimeElapsed()
{
	if (ensureAlways(TeleportProjectileClass))
	{
		//this is a socket defined on one of the bones in the mesh
		FVector Hand = GetMesh()->GetSocketLocation("Muzzle_01");
		FRotator Rotation = CorrectProjectileAngle(Hand);
		FTransform SpawnTM = FTransform(Rotation, Hand);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		GetWorld()->SpawnActor<AActor>(TeleportProjectileClass, SpawnTM, SpawnParams);
		UGameplayStatics::SpawnEmitterAttached(CastEffect, GetMesh(), "Muzzle_01");
	}
}

void ASCharacter::Blackhole()
{
	PlayAnimMontage(AttackAnim);

	//call the attack function after a timer (see PrimaryAttack_TimeElapsed
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::Blackhole_TimeElapsed, 0.2f);
}

void ASCharacter::Blackhole_TimeElapsed()
{
	if (ensureAlways(BlackholeProjectileClass))
	{
		//this is a socket defined on one of the bones in the mesh
		FVector Hand = GetMesh()->GetSocketLocation("Muzzle_01");
		FRotator Rotation = CorrectProjectileAngle(Hand);
		FTransform SpawnTM = FTransform(Rotation, Hand);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		GetWorld()->SpawnActor<AActor>(BlackholeProjectileClass, SpawnTM, SpawnParams);
		UGameplayStatics::SpawnEmitterAttached(CastEffect, GetMesh(), "Muzzle_01");
	}
}

FRotator ASCharacter::CorrectProjectileAngle(const FVector& StartPoint) const
{
	FVector CamTarget = CameraComp->GetForwardVector() * 25000 + CameraComp->GetComponentLocation();

	//we have to do a line trace in case the object is nearer, and need to shorten the range ... otherwise we'd always be aiming at (basically) infinity
	FHitResult Res;
	FCollisionQueryParams ColParams;
	ColParams.AddIgnoredActor(this);
	bool bHitObject = GetWorld()->LineTraceSingleByChannel(Res, CameraComp->GetComponentLocation(), CamTarget, ECC_Camera, ColParams);

	//rotate the projectile's vector based on where the camera is pointing
	FVector Target = (bHitObject) ? Res.ImpactPoint : CamTarget;
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(StartPoint, Target);
	return Rotation;
}



void ASCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}




// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Teleport", IE_Pressed, this, &ASCharacter::Teleport);
	PlayerInputComponent->BindAction("Blackhole", IE_Pressed, this, &ASCharacter::Blackhole);
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* UOwningComp, float NewHealth, float MaxHealth, float Delta)
{
	if(NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
	GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	FVector FlashColour = (Delta > 0) ? FVector(0,1,0) : FVector(1, 0, 0);
	GetMesh()->SetVectorParameterValueOnMaterials("FlashColour", FlashColour);
}
