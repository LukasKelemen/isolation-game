// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSCharacter.h"
#include "InteractInterface.h"
#include "AmmoPickup.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class USoundBase;

/** Enum for the ammo amounts that a pickup can have */
UENUM()
enum class EAmmoAmount : uint8
{
	Low    		UMETA(DisplayName="Low Ammo"),
	Medium 		UMETA(DisplayName="Medium Ammo"),
	High 		UMETA(DisplayName="High Ammo"),
};

/** Struct that keeps track of all our data per ammo type */
USTRUCT()
struct FAmmoTypeData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TMap<EAmmoAmount, UStaticMesh*> FullAmmoBoxes;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<EAmmoAmount, UStaticMesh*> EmptyAmmoBoxes;

	UPROPERTY(EditDefaultsOnly)
	TMap<EAmmoAmount, int32> AmmoCounts;
	
};

UCLASS()
class ISOLATION_API AAmmoPickup : public AActor, public ISInteractInterface
{
	GENERATED_BODY()

public:
	
	FText GetPickupName() { return PickupName[AmmoType]; }

	EAmmoType GetAmmoType() const { return AmmoType; }

protected:

	/** Map to keep track of the name showed to the player for each ammo type */
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	TMap<EAmmoType, FText> PickupName;

	/** Map to keep track of all values for the meshes and ammo counts */
	UPROPERTY(EditDefaultsOnly, Category = "Meshes")
	TMap<EAmmoType, FAmmoTypeData> AmmoData;
	
private:
	
	/** Interact function from Interact Interface */
	virtual void Interact() override;
	
	/** Sets default values for this actor's properties */
	AAmmoPickup();

	/** Overriden construction script to display mesh preview in engine */
	virtual void OnConstruction(const FTransform& Transform) override;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Updates the pickup mesh from the full mesh to the empty one */
	void SetEmptyMesh();
	
	/** Display mesh */
	UPROPERTY()
	UStaticMeshComponent* MeshComp;
	
	/** The amount of ammo (low/medium/high that this instance should have */
	UPROPERTY(EditInstanceOnly, Category = "Properties")
	EAmmoAmount AmmoAmount;

	/** The type of ammo that this instance should have */
	UPROPERTY(EditInstanceOnly, Category = "Properties")
	EAmmoType AmmoType;

	/** Whether this is an infinite ammo box or not (used for demo level) */
	UPROPERTY(EditInstanceOnly, Category = "Properties")
	bool bInfinite;
	
	/** Whether the player can interact with this ammo pickup (whether it is full or empty, basically) */
	bool bIsEmpty;

	/** Whether debug print statements should be shown */
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bDrawDebug;

	/** The sound effect to play when ammunition is collected */
	UPROPERTY(EditDefaultsOnly, Category = "Sound bases	")
	USoundBase* PickupSFX;
};
