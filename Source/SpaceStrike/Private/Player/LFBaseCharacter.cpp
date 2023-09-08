// Source Game, All Rights Reserved.


#include "Player/LFBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/LFCharacterMovementComponent.h"
#include "Components/LFHealthComponent.h"
#include "Components/TextRenderComponent.h"
//#include "Components/LFWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

// Sets default values
ALFBaseCharacter::ALFBaseCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<ULFCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Component");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<ULFHealthComponent>("Health Component");

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);

	//WeaponComponent = CreateDefaultSubobject<USGWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ALFBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	//check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ALFBaseCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ALFBaseCharacter::OnHealthChanged);

	LandedDelegate.AddDynamic(this, &ALFBaseCharacter::OnGroundLanded);

}

// Called every frame
void ALFBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ALFBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	//check(WeaponComponent);


	PlayerInputComponent->BindAxis("MoveForward", this, &ALFBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALFBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ALFBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ALFBaseCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALFBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ALFBaseCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ALFBaseCharacter::OnStopRunning);
	/*PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USGWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USGWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USGWeaponComponent::NextWeapon);*/
	
}//
/**
 * @brief Movement and Rotation 
 */

void ALFBaseCharacter::MoveForward(float Amount)
{
	IsMovingForward = Amount > 0.0f;
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ALFBaseCharacter::MoveRight(float Amount)
{
	AddMovementInput(GetActorRightVector(), Amount);
}

void ALFBaseCharacter::OnStartRunning()
{
	WantsToRun = true;
}

void ALFBaseCharacter::OnStopRunning()
{
	WantsToRun = false;
}

bool ALFBaseCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

float ALFBaseCharacter::GetMovementDirection() const
{
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	return FMath::RadiansToDegrees(AngleBetween) * FMath::Sign(CrossProduct.Z);
}
/////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief 
 */

void ALFBaseCharacter::OnDeath()
{
	UE_LOG(LogBaseCharacter,Display,TEXT("Player %s Is Dead"), *GetName());

	PlayAnimMontage(DeathAnimMontage);
	
	GetCharacterMovement()->DisableMovement();
	
	SetLifeSpan(LifeSpanOnDeath);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	//WeaponComponent->StopFire();
}

void ALFBaseCharacter::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ALFBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetVelocity().Z;
	if (FallVelocityZ < LandDamageVelocity.X) return;

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(
		LandDamageVelocity, LandDamage, FallVelocityZ);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
	UE_LOG(LogBaseCharacter,Display,TEXT("Player %s recived landed damage: %f"), *GetName(), FinalDamage);
}
