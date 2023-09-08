// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.


#include "Player/LFBaseShipPawn.h"

#include "LFGameStateBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/LFHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseShipPawn, All, All);

ALFBaseShipPawn::ALFBaseShipPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	HealthComponent = CreateDefaultSubobject<ULFHealthComponent>(TEXT("Health Component"));
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthTextComponent"));
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);
	
	RadarComponent = CreateDefaultSubobject<ULFRadarComponent>(TEXT("Radar Component"));
	RadarComponent->CreationMethod = EComponentCreationMethod::Instance;	
}

void ALFBaseShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALFBaseShipPawn::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(HealthTextComponent);

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ALFBaseShipPawn::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ALFBaseShipPawn::OnHealthChanged);
	
	OwnerShipController = GetController();

	OwnerShip = EFactionType::SpaceResistanceForces;
	
	if (const TObjectPtr<ALFGameStateBase> TheGameState = Cast<ALFGameStateBase>(UGameplayStatics::GetGameState(GetWorld())))
	{
		TheGameState->RegisterShip(this);
	}
}

void ALFBaseShipPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(const TObjectPtr<ALFGameStateBase> TheGameState = Cast<ALFGameStateBase>(UGameplayStatics::GetGameState(GetWorld())))
	{
		TheGameState->UnregisterShip(this);
	}
}

const EFactionType& ALFBaseShipPawn::GetFactionType() const
{
	return OwnerShip;
}

const ULFRadarComponent* ALFBaseShipPawn::GetRadarComponent() const
{
	return RadarComponent;
}

void ALFBaseShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALFBaseShipPawn::OnDeath()
{
	UE_LOG(LogBaseShipPawn, Display, TEXT("Player %s Is Dead"), *GetName());

	IsDead = true;

	/*PlayAnimMontage(DeathAnimMontage);
	
	GetCharacterMovement()->DisableMovement();
	
	SetLifeSpan(LifeSpanOnDeath);
	
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	//WeaponComponent->StopFire();*/
}

void ALFBaseShipPawn::OnHealthChanged(float Health)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}
