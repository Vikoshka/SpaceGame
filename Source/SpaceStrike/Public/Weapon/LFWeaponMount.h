// Studio (Lux Futurum), Game Project (Space Strike), All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "LFWeaponMount.generated.h"

UENUM()
enum class EWeaponMountState : uint8
{
	Disabled = 0,
	Idle = 1,
	Tracking = 2,
	ReadyToShoot = 3
};
/* ДЛЯ ОБЩЕНИЯ:
оставь этот код, типо посхалочка
компилятор всёравно это сотрёт
это потому что в макросах пишем?
потому что это комментарий, он не участвует в компиляции
ну так комментарии не удаляются
ща покажу
ну так в конечном результате то нет, а тут останется
да)
вот и заебись, я мыться
 */
UCLASS()
class SPACESTRIKE_API ALFWeaponMount : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ALFWeaponMount();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetWorldLocation() const;

	UFUNCTION(BlueprintCallable)
	void TurnToPos(FVector TargetPos);

	// TODO: Rename
	UFUNCTION(BlueprintCallable)
	void PullOut();

	UFUNCTION(BlueprintCallable)
	void PullIn();
	
	UFUNCTION(BlueprintCallable)
	void TurnOn();

	UFUNCTION(BlueprintCallable)
	void TurnOff();
	
	UFUNCTION(BlueprintCallable)
	EWeaponMountState GetState();

	UFUNCTION(BlueprintCallable)
	bool IsCanRotateTo(FRotator Rotator);

	UFUNCTION(BlueprintCallable)
	FRotator GetLocalRotateToPosition(FVector Position);

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// FMath::RInterpConstant, but without normalizing out rotator
	static FRotator RInterpConstantToNoNormalized(const FRotator& Current, const FRotator& Target, float DeltaTime, float InterpSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	TObjectPtr<UMeshComponent> BaseMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	TObjectPtr<UMeshComponent> MountMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angles")
	FRotator MinAngles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angles")
	FRotator MaxAngles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angles")
	float TurnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angles")
	float ElevationSpeed;

	UPROPERTY()
	FBoneSocketTarget WeaponSocket;
	
	bool bIsActivated;
	bool bIsOnline;
	FVector TargetPosition;
	EWeaponMountState TurretState;
};


