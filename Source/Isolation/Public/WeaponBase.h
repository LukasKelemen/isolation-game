/** Fill out your copyright notice in the Description page of Project Settings. */

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class AWeaponBase;
class USkeletalMeshComponent;
class USkeletalMesh;
class UStaticMesh;
class UAnimMontage;
class UNiagaraSystem;
class UBlendSpace;
class USoundCue;
class UPhysicalMaterial;
class UDataTable;
class AWeaponPickup;

/** Enumerator holding the 4 types of ammunition that weapons can use (used as part of the FSingleWeaponParams struct)
 * and to keep track of the total ammo the player has (ammoMap) */
UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Pistol       UMETA(DisplayName = "Pistol Ammo"),
	Rifle        UMETA(DisplayName = "Rifle Ammo"),
	Shotgun      UMETA(DisplayName = "Shotgun Ammo"),
	Special		 UMETA(DisplayName = "Special Ammo"),
};

/** Enumerator holding all the possible typed of attachment */
UENUM()
enum class EAttachmentType : uint8
{
	Barrel		UMETA(DisplayName = "Barrel Attachment"),
	Magazine	UMETA(DisplayName = "Magazine Attachment"),
	Sights		UMETA(DisplayName = "Sights Attachment"),
	Stock		UMETA(DispayName = "Stock Attachment"),
	Grip		UMETA(DispayName = "Grip Attachment"),
};

/** Struct keeping track of important weapon variables modified at runtime. This structs contains data that is either
 *	modified at runtime, such as the amount of ammunition in the weapon, but also data required to spawn attachments
 *	and pickups
 */
USTRUCT(BlueprintType)
struct FRuntimeWeaponData
{
	GENERATED_BODY()

	/** A reference to the weapon class of the given weapon */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Data")
	TSubclassOf<AWeaponBase> WeaponClassReference;

	/** The maximum size of the player's magazine */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Data")
	int ClipCapacity; 

	/** The amount of ammunition currently in the magazine */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Data")
	int ClipSize;

	/** Enumerator holding the 4 possible ammo types defined above */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Data")
	EAmmoType AmmoType;

	/** The current health of the weapon (degradation values are in the weapon class) */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Data")
	float WeaponHealth;

	/** The attachments used in the current weapon */
	UPROPERTY(BlueprintReadOnly, Category = "Weapon Data")
	TArray<FName> WeaponAttachments;
};

/** Struct holding all data required by an attachment */
USTRUCT(BlueprintType)
struct FAttachmentData : public FTableRowBase
{
	GENERATED_BODY()

	/** The skeletal mesh displayed on the weapon itself */
	UPROPERTY(EditDefaultsOnly, Category = "General")
	USkeletalMesh* AttachmentMesh;

	/** The static mesh displayed on the weapon pickup */
	UPROPERTY(EditDefaultsOnly, Category = "General")
	UStaticMesh* PickupMesh;

	/** The type of attachment */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "General")
	EAttachmentType AttachmentType;

	/** Attachments that are incompatible with the given attachment */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "General")
	TArray<FName> IncompatibleAttachments;

	/** The impact that this attachment has on the base damage of the weapon */
	UPROPERTY(EditDefaultsOnly, Category = "General")
	float BaseDamageImpact;

	/** The pitch variation impact of this attachment */
	UPROPERTY(EditDefaultsOnly, Category = "General")
	float WeaponPitchVariationImpact;

	/** The yaw variation impact of this attachment */
	UPROPERTY(EditDefaultsOnly, Category = "General")
	float WeaponYawVariationImpact;

	/** How much this attachment multiplies the vertical recoil of the weapon */
	UPROPERTY(EditDefaultsOnly, Category = "General")
	float VerticalRecoilMultiplier;

	/** How much this attachment multiplies the horizontal recoil of this weapon */
	UPROPERTY(EditDefaultsOnly, Category = "General")
	float HorizontalRecoilMultiplier;

	/** The name of the socket on the muzzle attachment's skeletal mesh with which to override the muzzle */ 
	UPROPERTY(EditDefaultsOnly, Category = "Barrel", meta=(EditCondition="AttachmentType == EAttachmentType::Barrel"))
	FName MuzzleLocationOverride;

	/** The name of the socket at which to spawn particles for muzzle flash */
	UPROPERTY(EditDefaultsOnly, Category = "Barrel", meta=(EditCondition="AttachmentType == EAttachmentType::Barrel"))
	FName ParticleSpawnLocationOverride;

	/** Whether the current barrel attachment is silenced or not */
	UPROPERTY(EditDefaultsOnly, Category = "Barrel", meta=(EditCondition="AttachmentType == EAttachmentType::Barrel"))
	bool bSilenced;

	/** The firing sound to use instead of the default for this particular magazine attachment */ 
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	USoundBase* FiringSoundOverride;

	/** The silenced firing sound to use instead of the default for this particular magazine attachment */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	USoundBase* SilencedFiringSoundOverride;

	/** An override for the default walk BlendSpace */
	UPROPERTY(EditDefaultsOnly, Category = "Grip", meta=(EditCondition="AttachmentType == EAttachmentType::Grip"))
	UBlendSpace* BS_Walk;

	/** An override for the default ADS walk BlendSpace */
	UPROPERTY(EditDefaultsOnly, Category = "Grip", meta=(EditCondition="AttachmentType == EAttachmentType::Grip"))
	UBlendSpace* BS_Ads_Walk;

	/** An override for the default idle animation sequence */
	UPROPERTY(EditDefaultsOnly, Category = "Grip", meta=(EditCondition="AttachmentType == EAttachmentType::Grip"))
	UAnimSequence* Anim_Idle;

	/** An override for the default ADS idle animation sequence */
	UPROPERTY(EditDefaultsOnly, Category = "Grip", meta=(EditCondition="AttachmentType == EAttachmentType::Grip"))
	UAnimSequence* Anim_Ads_Idle;

	/** An override for the default sprint animation sequence */
	UPROPERTY(EditDefaultsOnly, Category = "Grip", meta=(EditCondition="AttachmentType == EAttachmentType::Grip"))
	UAnimSequence* Anim_Sprint;

	/** The shooting animation for the weapon itself (bolt shooting back/forward) */
	UPROPERTY(EditDefaultsOnly, Category = "Grip", meta=(EditCondition="AttachmentType == EAttachmentType::Grip"))
	UAnimSequence* Gun_Shot;

	/** Unequip animation for the current weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Grip", meta=(EditCondition="AttachmentType == EAttachmentType::Grip"))
	UAnimMontage* WeaponEquip;

	/** The ammunition type to be used (Spawned on the pickup) */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	EAmmoType AmmoToUse;

	/** The clip capacity of the weapon (Spawned on the pickup) */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	int ClipCapacity;

	/** The clip size of the weapon (Spawned on the pickup) */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	int ClipSize;

	/** The default health of the weapon (Spawned on the pickup) */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	float WeaponHealth = 100.0f;

	/** The rate of fire of this magazine attachment */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	float FireRate;

	/** Whether this magazine supports automatic fire */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	bool AutomaticFire;
	
	/** The vertical recoil curve to be used with this magazine */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	UCurveFloat* VerticalRecoilCurve;

	/** The horizontal recoil curve to be used with this magazine */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	UCurveFloat* HorizontalRecoilCurve;

	/** The camera shake to be applied to the recoil from this magazine */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	TSubclassOf<UCameraShakeBase> RecoilCameraShake;

	/** Whether this magazine fires shotgun shells (should we fire lots of pellets or just one bullet?) */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	bool bIsShotgun = false;

	/** The range of the shotgun shells in this magazine */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	float ShotgunRange;

	/** The amount of pellets fired */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	int ShotgunPellets;

	/** The increase in shot variation when the player is not aiming down the sights */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	float AccuracyDebuff = 1.25f;

	/** An override for the weapon's empty reload animation */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	UAnimationAsset* EmptyWeaponReload;

	/** An override for the weapon's reload animation */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	UAnimationAsset* WeaponReload;

	/** An override for the player's empty reload animation */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	UAnimMontage* EmptyPlayerReload;

	/** An override for the player's reload animation */
	UPROPERTY(EditDefaultsOnly, Category = "Magazine", meta=(EditCondition="AttachmentType == EAttachmentType::Magazine"))
	UAnimMontage* PlayerReload;

	/** The offset applied to the camera to align with the sights */
	UPROPERTY(EditDefaultsOnly, Category = "Sights", meta=(EditCondition="AttachmentType == EAttachmentType::Sights"))
	float VerticalCameraOffset;

	/** Whether the player's FOV should change when aiming with this weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Sights", meta=(EditCondition="AttachmentType == EAttachmentType::Sights"))
	bool bAimingFOV = false;

	/** The decrease in FOV of the camera to when aim down sights */
	UPROPERTY(EditDefaultsOnly, Category = "Sights", meta=(EditCondition="AttachmentType == EAttachmentType::Sights"))
	float AimingFOVChange;

	/** Whether this weapon has a scope and we need to render a SceneCaptureComponent2D */
	UPROPERTY(EditDefaultsOnly, Category = "Sights", meta=(EditCondition="AttachmentType == EAttachmentType::Sights"))
	bool bIsScope = false;

	/** The Magnification of the scope */
	UPROPERTY(EditDefaultsOnly, Category = "Sights", meta=(EditCondition="AttachmentType == EAttachmentType::Sights"))
	float ScopeMagnification = 1.0f;

	/** The linear FOV at a magnification of 1x */
	UPROPERTY(EditDefaultsOnly, Category = "Sights", meta=(EditCondition="AttachmentType == EAttachmentType::Sights"))
	float UnmagnifiedLFoV = 200.0f;
};

/** Struct holding all required information about the weapon class. This data is set once at tbe beginning of this
 * actor's lifetime, and then remains unchanged for it's duration. It encapsulates all the data regarding the statistics
 * of this weapon, as well as data regarding it's appearance, such as animations and particle effects.
 */
USTRUCT(BlueprintType)
struct FStaticWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	/**Pickup reference */
	UPROPERTY(EditDefaultsOnly, Category = "Required")
	TSubclassOf<AWeaponPickup> PickupReference;
	
	/** Determines the socket or bone with which the weapon will be attached to the character's hand (typically the root bone or the grip bone) */
	UPROPERTY(EditDefaultsOnly, Category = "Required")
	FName WeaponAttachmentSocketName;
		
	/** The distance the shot will travel */
	UPROPERTY(EditDefaultsOnly, Category = "Required")
	float LengthMultiplier;
		
	/** unmodified damage values of this weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Required")
	float BaseDamage;
	
	/** multiplier to be applied when the player hits an enemy's head bone */
	UPROPERTY(EditDefaultsOnly, Category = "Required")
	float HeadshotMultiplier;

	/** The amount of health taken away from the weapon every time the trigger is pulled */
	UPROPERTY(EditDefaultsOnly, Category = "Required")
	float WeaponDegradationRate;

	/** The pitch variation applied to the bullet as it leaves the barrel */
	UPROPERTY(EditDefaultsOnly, Category = "Required")
	float WeaponPitchVariation;
	
	/** The yaw variation applied to the bullet as it leaves the barrel */
	UPROPERTY(EditDefaultsOnly, Category = "Required")
	float WeaponYawVariation;

	/** Attachments */

	/** Whether this weapon has a unique set of attachments and is broken up into multiple meshes or is unique */
	UPROPERTY(EditDefaultsOnly, Category = "Attachments")
	bool bHasAttachments = true;

	/** The table which holds the attachment data */
	UPROPERTY(EditDefaultsOnly, Category = "Attachments", meta=(EditCondition="bHasAttachments"))
	UDataTable* AttachmentsDataTable;

	/** Animations */

	/** The walking BlendSpace */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	UBlendSpace* BS_Walk;

	/** The ADS Walking BlendSpace */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	UBlendSpace* BS_Ads_Walk;

	/** The Idle animation sequence */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	UAnimSequence* Anim_Idle;

	/** The ADS Idle animation sequence */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	UAnimSequence* Anim_Ads_Idle;

	/** The weapon's empty reload animation */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	UAnimationAsset* EmptyWeaponReload;

	/** The weapon's reload animation */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	UAnimationAsset* WeaponReload;

	/** The player's empty reload animation */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	UAnimMontage* EmptyPlayerReload;

	/** The player's reload animation */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	UAnimMontage* PlayerReload;

	/** The sprinting animation sequence */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	UAnimSequence* Anim_Sprint;

	/** The shooting animation for the weapon itself (bolt shooting back/forward) */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	UAnimSequence* Gun_Shot;
	
	/** An override for the player's reload animation */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	UAnimMontage* WeaponEquip;

	/** An override for the player's reload animation */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	UAnimMontage* WeaponUnequip;

	/** Firing Mechanisms */

	/** Determines if the weapon can have a round in the chamber or not */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	bool bCanBeChambered;

	/** Whether the weapon is silenced or not */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	bool bSilenced;

	/** We wait for the animation to finish before the player is allowed to fire again (for weapons where the character has to perform an action before being able to fire again) Requires fireMontage to be set */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	bool bWaitForAnim;

	/** Whether this weapon is a shotgun or not */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	bool bIsShotgun = false;

	/** Whether the player's FOV should change when aiming with this weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	bool bAimingFOV = false;

	/** The decrease in FOV of the camera to when aim down sights */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	float AimingFOVChange;

	/** Whether this weapon has a scope and we need to render a SceneCaptureComponent2D */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	bool bIsScope = false;

	/** The Magnification of the scope */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	float ScopeMagnification = 1.0f;

	/** The linear FOV at a magnification of 1x */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	float UnmagnifiedLFoV = 200.0f;
	
	/** The name of the socket which denotes the end of the muzzle (used for gunfire) */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	FName MuzzleLocation;

	/** The name of the socket at which to spawn particles for muzzle flash */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	FName ParticleSpawnLocation;

	/** The ammunition type to be used (Spawned on the pickup) */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	EAmmoType AmmoToUse;

	/** The clip capacity of the weapon (Spawned on the pickup) */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	int ClipCapacity;

	/** The clip size of the weapon (Spawned on the pickup) */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	int ClipSize;

	/** The rate of fire of the weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	float RateOfFire;

	/** Whether this weapon supports automatic fire */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	bool bAutomaticFire;
		
	/** The vertical recoil curve to be used with this weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	UCurveFloat* VerticalRecoilCurve;

	/** The horizontal recoil curve to be used with this weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	UCurveFloat* HorizontalRecoilCurve;

	/** The camera shake to be applied to the recoil from this weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	TSubclassOf<UCameraShakeBase> RecoilCameraShake;
	
	/** The range of the shotgun shells of this weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	float ShotgunRange;

	/** The amount of pellets fired */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	int ShotgunPellets;

	/** The increase in shot variation when the player is not aiming down the sights */
	UPROPERTY(EditDefaultsOnly, Category = "Unique Weapon (No Attachments)", meta=(EditCondition="!bHasAttachments"))
	float AccuracyDebuff = 1.25f;

	/** Damage surfaces */

	/** surface (physical material) for areas which should spawn blood particles when hit and receive normal damage (equivalent to the baseDamage variable) */
	UPROPERTY(EditDefaultsOnly, Category = "Damage Surfaces", meta=(EditCondition="!bHasAttachments"))
	UPhysicalMaterial* NormalDamageSurface;
	
	/** surface (physical material) for areas which should spawn blood particles when hit and receive boosted damage (equivalent to the baseDamage variable multiplied by the headshotMultiplier) */
	UPROPERTY(EditDefaultsOnly, Category = "Damage Surfaces", meta=(EditCondition="!bHasAttachments"))
	UPhysicalMaterial* HeadshotDamageSurface;
	
	/** surface (physical material) for areas which should spawn ground particles when hit) */
	UPROPERTY(EditDefaultsOnly, Category = "Damage Surfaces", meta=(EditCondition="!bHasAttachments"))
	UPhysicalMaterial* GroundSurface;
	
	/** surface (physical material) for areas which should spawn rock particles when hit) */
	UPROPERTY(EditDefaultsOnly, Category = "Damage Surfaces", meta=(EditCondition="!bHasAttachments"))
	UPhysicalMaterial* RockSurface;

	/** VFX */
	
	/** particle effect (Niagara system) to be spawned when an enemy is hit */
	UPROPERTY(EditDefaultsOnly, Category = "VFX", meta=(EditCondition="!bHasAttachments"))
	UNiagaraSystem* EnemyHitEffect;
	
	/** particle effect (Niagara system) to be spawned when the ground is hit */
	UPROPERTY(EditDefaultsOnly, Category = "VFX", meta=(EditCondition="!bHasAttachments"))
	UNiagaraSystem* GroundHitEffect;
	
	/** particle effect (Niagara system) to be spawned when a rock is hit */
	UPROPERTY(EditDefaultsOnly, Category = "VFX", meta=(EditCondition="!bHasAttachments"))
	UNiagaraSystem* RockHitEffect;
	
	/** particle effect (Niagara system) to be spawned when no defined type is hit */
	UPROPERTY(EditDefaultsOnly, Category = "VFX", meta=(EditCondition="!bHasAttachments"))
	UNiagaraSystem* DefaultHitEffect;

	/** particle effect to be spawned at the muzzle when a shot is fired */
	UPROPERTY(EditDefaultsOnly, Category = "VFX", meta=(EditCondition="!bHasAttachments"))
	UParticleSystem* MuzzleFlash;

	/** particle effect to be spawned at the muzzle that shows the path of the bullet */
	UPROPERTY(EditDefaultsOnly, Category = "VFX", meta=(EditCondition="!bHasAttachments"))
	UParticleSystem* BulletTrace;

	/** Sound bases */

	/** Firing sound */
	UPROPERTY(EditDefaultsOnly, Category = "Sound bases", meta=(EditCondition="!bHasAttachments"))
	USoundBase* FireSound;
	
	/** Silenced firing sound */
	UPROPERTY(EditDefaultsOnly, Category = "Sound bases", meta=(EditCondition="!bHasAttachments"))
	USoundBase* SilencedSound;
	
	/** Empty firing sound */
	UPROPERTY(EditDefaultsOnly, Category = "Sound bases", meta=(EditCondition="!bHasAttachments"))
	USoundBase* EmptyFireSound;
	
};

UCLASS()
class ISOLATION_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:

	/** Returns the runtime weapon data of the weapon */
	FRuntimeWeaponData* GetRuntimeWeaponData() { return &GeneralWeaponData; }

	/** Update the weapon's runtime weapon data
	 *	@param NewWeaponData The weapons new runtime weapon data 
	 */
	void SetRuntimeWeaponData(const FRuntimeWeaponData NewWeaponData) { GeneralWeaponData = NewWeaponData; }

	/** Returns a reference to the static weapon data of the weapon */
	FStaticWeaponData* GetStaticWeaponData() { return &WeaponData; }

	/** Updates the weapon's static weapon data
	 *	@param NewWeaponData The weapon's new static weapon data
	 */
	void SetStaticWeaponData(const FStaticWeaponData NewWeaponData) { WeaponData = NewWeaponData; }
	
	/** Starts firing the gun (sets the timer for automatic fire) */
	void StartFire();
	
	/** Stops the timer that allows for automatic fire */
	void StopFire();
	
	/** Plays the reload animation and sets a timer based on the length of the reload montage */
	void Reload();

	/** Spawns the weapons attachments and applies their data/modifications to the weapon's statistics */ 
	void SpawnAttachments();

	/** Whether the weapon can fire or not */
	bool CanFire() const { return bCanFire; }

	/** Update the weapon's ability to fire
	 *	@param bNewFire The new state of the weapon's ability to fire
	 */
	void SetCanFire(const bool bNewFire) { bCanFire = bNewFire; }

	/** Whether the weapon is currently in it's reload state */
	bool IsReloading() const { return bIsReloading; }

	/** Update the weapon's recovery behaviour
	 *	@param bNewShouldRecover Whether the weapon should recover from recoil or not
	 */
	void SetShouldRecover(const bool bNewShouldRecover) { bShouldRecover = bNewShouldRecover; } 

	/** A reference to the recoil recovery timeline */
	FTimeline* GetRecoilRecoveryTimeline() { return &RecoilRecoveryTimeline; }

	/** A reference to the key name of the Weapon Data datatable */
	FString GetDataTableNameRef() const { return DataTableNameRef; }

	UFUNCTION(BlueprintCallable)
	void SetShowDebug(const bool IsVisible) { bShowDebug = IsVisible; }

	UFUNCTION(BlueprintCallable)
	bool GetShowDebug() { return bShowDebug; }
	
	/** Returns the character's empty-handed walking blend space */
	UFUNCTION(BlueprintCallable)
	UBlendSpace* GetWalkBlendSpace() const { return WalkBlendSpace; }

	/** Returns the character's empty-handed walking blend space for use in the aiming state */
	UFUNCTION(BlueprintCallable)
	UBlendSpace* GetWalkAdsBlendSpace() const { return ADSWalkBlendSpace; }

	/** Returns the character's empty-handed idle animation */
	UFUNCTION(BlueprintCallable)
	UAnimSequence* GetIdleAnim() const { return Anim_Idle; }

	/** Returns the character's empty handed idle animation for use in the aiming state */
	UFUNCTION(BlueprintCallable)
	UAnimSequence* GetAdsIdleAnim() const { return Anim_ADS_Idle; }

	/** Returns the character's empty handed sprinting animation */
	UFUNCTION(BlueprintCallable)
	UAnimSequence* GetSprintAnim() const { return Anim_Sprint; }

	/** Returns the vertical camera offset for this weapon instance */
	UFUNCTION(BlueprintCallable)
	float GetVerticalCameraOffset() const { return VerticalCameraOffset; }
	
private:

#pragma region FUNCTIONS
	
	/** Sets default values for this actor's properties */
	AWeaponBase();
	
	/** Spawns the line trace that deals damage and applies sound/visual effects */
	void Fire();	

	/** Applies recoil to the player controller */
	void Recoil();

	/** Updates ammunition values (we do this after the animation has finished for cleaner UI updates and to prevent the player from being able to switch weapons to skip the reload animation) */
	void UpdateAmmo();

	/** Allows the player to fire again */
	void EnableFire();

	/** Begins applying recoil to the weapon */
	void StartRecoil();

	/** Initiates the recoil function */
	void RecoilRecovery();

	/** Interpolates the player back to their initial view vector */
	UFUNCTION()
	void HandleRecoveryProgress(float Value) const;

	/** Converts an unmagnified linear FOV and a magnification constant into a magnified FOV */
	float FOVFromMagnification() const;

	/** Renders the scope widget to it's virtual texture */
	void RenderScope() const;
	
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;
	
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

#pragma endregion

#pragma region USER_VARIABLES
	
	/** The framerate that the scope widget renders at. Cannot be faster than the game framerate - if so it will render
	 *	at the game's framerate */
	UPROPERTY(EditDefaultsOnly, Category = "Temp")
	float ScopeFrameRate = 60.0f;

	/** Data table reference */
	UPROPERTY(EditDefaultsOnly, Category = "Data | Data Table")
	UDataTable* WeaponDataTable;

	/** The Key reference to the weapon data table */
	UPROPERTY(EditDefaultsOnly, Category = "Data | Data Table")
	FString DataTableNameRef;

	/** Debug boolean, toggle for debug strings and line traces to be shown */
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bShowDebug = false;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USceneCaptureComponent2D* ScopeCaptureComponent;
	
	/** The main skeletal mesh - holds the weapon model */
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;
	
	/** damage type (set in blueprints) */
	UPROPERTY(EditDefaultsOnly, Category = "Data | Damage")
	TSubclassOf<UDamageType> DamageType;

	/** The curve for recovery */ 
	UPROPERTY(EditDefaultsOnly, Category = "Data | Recoil Recovery")
	UCurveFloat* RecoveryCurve;

	/** The ejected casing particle effect to be played after each shot */
	UPROPERTY(EditDefaultsOnly, Category = "Particles")
	UNiagaraSystem* EjectedCasing;

#pragma endregion 

#pragma region INTERNAL_VARIABLES
	
	FRuntimeWeaponData GeneralWeaponData;

	/** The skeletal mesh used to hold the current barrel attachment */
	UPROPERTY()
	USkeletalMeshComponent* BarrelAttachment;

	/** The skeletal mesh used to hold the current magazine attachment */
	UPROPERTY()
	USkeletalMeshComponent* MagazineAttachment;

	/** The skeletal mesh used to hold the current sights attachment */
	UPROPERTY()
	USkeletalMeshComponent* SightsAttachment;

	/** The skeletal mesh used to hold the current stock attachment */
	UPROPERTY()
	USkeletalMeshComponent* StockAttachment;

	/** The skeletal mesh used to hold the current grip attachment */
	UPROPERTY()
	USkeletalMeshComponent* GripAttachment;

	/** Determines if the player can fire */
	bool bCanFire = true;
	
	/** Keeps track of whether the weapon is being reloaded */
	bool bIsReloading = false;

	/** The sum of the modifications the attachments make to damage */
	float DamageModifier;

	/** The sum of the modifications the attachments make to pitch */
	float WeaponPitchModifier;

	/** The sum of the modifications the attachments make to yaw */
	float WeaponYawModifier;

	/** Reference to the data stored in the weapon DataTable */
	FStaticWeaponData WeaponData;

	/** Reference to the data stored in the attachment DataTable */
	FAttachmentData* AttachmentData;
	
	/** The override for the weapon socket, in the case that we have a barrel attachment */
	FName SocketOverride;
	
	/** The override for the particle system socket, in the case that we have a barrel attachment */
	FName ParticleSocketOverride;
	
	/** Keeps track of the starting position of the line trace */
	FVector TraceStart;
	
	/** keeps track of the starting rotation of the line trace (required for calculating the trace end point) */
	FRotator TraceStartRotation;
	
	/** keeps track of the vector direction of the line trace (derived from rotation) */
	FVector TraceDirection;
	
	/** end point of the line trace */
	FVector TraceEnd;
	
	/** collision parameters for spawning the line trace */
	FCollisionQueryParams QueryParams;

	/** hit result variable set when a line trace is spawned */
	UPROPERTY()
	FHitResult Hit;

	/** internal variable used to keep track of the final damage value after modifications */
	float FinalDamage;
	
	/** The timer that handles automatic fire */
	FTimerHandle ShotDelay;
	
	/** The timer that is used when we need to wait for an animation to finish before being able to fire again */
	FTimerHandle AnimationWaitDelay;
	
	/** The timer used to keep track of how long a reloading animation takes and only assigning variables */ 
	FTimerHandle ReloadingDelay;
	
	/** The timer used for rendering the scope image */
	FTimerHandle ScopeRenderTimer;
	
	/** The curve for vertical recoil (set from WeaponData) */
	UPROPERTY()
	UCurveFloat* VerticalRecoilCurve;

	/** The timeline for vertical recoil (generated from the curve) */
	FTimeline VerticalRecoilTimeline;

	/** The curve for horizontal recoil (set from WeaponData) */ 
	UPROPERTY()
	UCurveFloat* HorizontalRecoilCurve;

	/** The timeline for horizontal recoil (generated from the curve) */
	FTimeline HorizontalRecoilTimeline;

	/** The timeline for recover (set from the curve) */
	FTimeline RecoilRecoveryTimeline;
	
	/** A value to temporarily cache the player's control rotation so that we can return to it */
	FRotator ControlRotation;

	/** Keeping track of whether we should do a recoil recovery after finishing firing or not */
	bool bShouldRecover;

	/** Used in recoil to make sure the first shot has properly applied recoil */
	int ShotsFired;
	
	/** The base multiplier for vertical recoil, modified by attachments */
	float VerticalRecoilModifier = 1.0f;

	/** The base multiplier for horizontal recoil, modified by attachments */
	float HorizontalRecoilModifier = 1.0f;
	
	/** Animation */

	/** Value used to keep track of the length of animations for timers */
	float AnimTime;

	/** The offset given to the camera in order to align the gun sights */
	UPROPERTY()
	float VerticalCameraOffset;
	
	/** Local instances of animations for use in AnimBP (Set from WeaponData and/or Attachments) */

	UPROPERTY()
	UAnimMontage* WeaponEquip;
	
	UPROPERTY()
	UBlendSpace* WalkBlendSpace;
	
	UPROPERTY()
	UBlendSpace* ADSWalkBlendSpace;

	UPROPERTY()
	UAnimSequence* Anim_Idle;

	UPROPERTY()
	UAnimSequence* Anim_Sprint;

	UPROPERTY()
	UAnimSequence* Anim_ADS_Idle;

	UPROPERTY()
	UAnimationAsset* EmptyWeaponReload;
	
	UPROPERTY()
	UAnimationAsset* WeaponReload;
	
	UPROPERTY()
	UAnimMontage* EmptyPlayerReload;
	
	UPROPERTY()
	UAnimMontage* PlayerReload;

#pragma endregion
};
