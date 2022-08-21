// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"

#include "FPSCharacter.h"
#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponPickup::AWeaponPickup()
{
	// Creating all of our meshes, and drawing them to the custom stencil for use with the outline shader 
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	MainMesh->SetRenderCustomDepth(true);
	MainMesh->SetCustomDepthStencilValue(2);
	MainMesh->SetupAttachment(RootComponent);

	BarrelAttachment = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelAttachment"));
	BarrelAttachment->SetRenderCustomDepth(true);
	BarrelAttachment->SetCustomDepthStencilValue(2);
	BarrelAttachment->SetupAttachment(MainMesh);

	MagazineAttachment = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagazineAttachment"));
	MagazineAttachment->SetRenderCustomDepth(true);
	MagazineAttachment->SetCustomDepthStencilValue(2);
	MagazineAttachment->SetupAttachment(MainMesh);
	
	SightsAttachment = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SightsAttachment"));
	SightsAttachment->SetRenderCustomDepth(true);
	SightsAttachment->SetCustomDepthStencilValue(2);
	SightsAttachment->SetupAttachment(MainMesh);

	StockAttachment = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StockAttachment"));
	StockAttachment->SetRenderCustomDepth(true);
	StockAttachment->SetCustomDepthStencilValue(2);
	StockAttachment->SetupAttachment(MainMesh);
	
	GripAttachment = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GripAttachment"));
	GripAttachment->SetRenderCustomDepth(true);
	GripAttachment->SetCustomDepthStencilValue(2);
	GripAttachment->SetupAttachment(MainMesh);
}

// Called when the game starts or when spawned
void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();

	if (AttachmentArrayOverride.Num() > 0)
	{
		DataStruct.WeaponAttachments = AttachmentArrayOverride;
	}

	// Spawning attachments on begin play
	SpawnAttachmentMesh();

	// Simulating physics if not bStatic
	if (!bStatic)
	{
		MainMesh->SetSimulatePhysics(true);
	}
}

// Updating the appearance of the pickup in the editor
void AWeaponPickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (AttachmentArrayOverride.Num() > 0)
	{
		DataStruct.WeaponAttachments = AttachmentArrayOverride;
	}

	// Live attachment preview in the editor
	SpawnAttachmentMesh();
}


void AWeaponPickup::SpawnAttachmentMesh()
{
	// Getting a reference to our Weapon Data table in order to see if we have attachments
	const AWeaponBase* WeaponBaseReference =  WeaponReference.GetDefaultObject();
	if (const FStaticWeaponData* WeaponData = WeaponDataTable->FindRow<FStaticWeaponData>(FName(WeaponBaseReference->GetDataTableNameRef()), FString(WeaponBaseReference->GetDataTableNameRef()), true))
	{
		// Spawning attachments if the weapon has them and the attachments table exists
		if (WeaponData->bHasAttachments && AttachmentsDataTable)
		{
			// Iterating through all the attachments in AttachmentArray
			for (FName RowName : DataStruct.WeaponAttachments)
			{
				// Searching the data table for the attachment
				const FAttachmentData* AttachmentData = AttachmentsDataTable->FindRow<FAttachmentData>(RowName, RowName.ToString(), true);

				// Applying the effects of the attachment
				if (AttachmentData)
				{
					if (AttachmentData->AttachmentType == EAttachmentType::Barrel)
					{
						BarrelAttachment->SetStaticMesh(AttachmentData->PickupMesh);
					}
					else if (AttachmentData->AttachmentType == EAttachmentType::Magazine)
					{
						MagazineAttachment->SetStaticMesh(AttachmentData->PickupMesh);
						// Pulling default values from the Magazine attachment type
						if (!bRuntimeSpawned)
						{
							DataStruct.AmmoType = AttachmentData->AmmoToUse;
							DataStruct.ClipCapacity = AttachmentData->ClipCapacity;
							DataStruct.ClipSize = AttachmentData->ClipSize;
							DataStruct.WeaponHealth = 100.0f;
						}
					}
					else if (AttachmentData->AttachmentType == EAttachmentType::Sights)
					{
						SightsAttachment->SetStaticMesh(AttachmentData->PickupMesh);
					}
					else if (AttachmentData->AttachmentType == EAttachmentType::Stock)
					{
						StockAttachment->SetStaticMesh(AttachmentData->PickupMesh);
					}
					else if (AttachmentData->AttachmentType == EAttachmentType::Grip)
					{
						GripAttachment->SetStaticMesh(AttachmentData->PickupMesh);
					}
				}
			}
		}
		else
		{
			// Applying default values if the weapon doesn't use attachments
			if (!bRuntimeSpawned)
			{
				DataStruct.AmmoType = WeaponData->AmmoToUse;
				DataStruct.ClipCapacity = WeaponData->ClipCapacity;
				DataStruct.ClipSize = WeaponData->ClipSize;
				DataStruct.WeaponHealth = 100.0f;
			}
		}
	}
}

void AWeaponPickup::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Testing!"));

	// Getting a reference to the Character Controller
	const AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PlayerCharacter->GetInventoryComponent())
	{
		int InventoryPosition = PlayerCharacter->GetInventoryComponent()->GetCurrentWeaponSlot();
		bool SpawnPickup = true;

		// Checking if the player has a free weapon slot. If not, we swap out the currently equipped weaopon
		for (int Index = 0; Index < PlayerCharacter->GetInventoryComponent()->GetNumberOfWeaponSlots(); Index++)
		{
			if (PlayerCharacter->GetInventoryComponent()->GetEquippedWeapons().Find(Index) == nullptr)
			{
				InventoryPosition = Index;
				SpawnPickup = false;
				break;
			}
		}

		// Spawning the new weapon in the player's inventory component
		PlayerCharacter->GetInventoryComponent()->UpdateWeapon(WeaponReference, InventoryPosition, SpawnPickup, bStatic, GetActorTransform(),  DataStruct);
	
		// Destroying the pickup
		Destroy();
	}
}