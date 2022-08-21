// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"
#include "AmmoPickup.h"
#include "EnhancedInputComponent.h"
#include "FPSCharacter.h"
#include "InteractionActor.h"
#include "WeaponPickup.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Interact with a hit actor through an interact interface
void UInteractionComponent::WorldInteract()
{    
    FCollisionQueryParams TraceParams;
    TraceParams.bTraceComplex = true;

    const AActor* Owner = GetOwner();
    
    if (Owner)
    {
        TraceParams.AddIgnoredActor(Owner);
    }

    FVector CameraLocation = FVector::ZeroVector;
    FRotator CameraRotation = FRotator::ZeroRotator;
    
    if (const AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(GetOwner()))
    {
        CameraLocation = FPSCharacter->GetCameraComponent()->GetComponentLocation();
        CameraRotation = FPSCharacter->GetCameraComponent()->GetComponentRotation();
    }
    
    const FVector TraceDirection = CameraRotation.Vector();
    const FVector TraceEndLocation = CameraLocation + TraceDirection * InteractDistance;

    // Checking if we hit something with our line trace
    if (GetWorld()->LineTraceSingleByChannel(InteractionHit, CameraLocation, TraceEndLocation, ECC_WorldStatic, TraceParams))
    {
        // Checking if the actor we hit implements the interaction interface
        if(InteractionHit.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
        {
            // Calling the Interact function within our hit actor via the interface
            Cast<IInteractInterface>(InteractionHit.GetActor())->Interact();
        }
    }
}

// Performing logic around the visibility of the interaction indicator - called every frame
void UInteractionComponent::InteractionIndicator()
{
    bCanInteract = false;
    bInteractionIsWeapon = false;
    
    FCollisionQueryParams TraceParams;
    TraceParams.bTraceComplex = true;

    const AActor* Owner = GetOwner();
    
    if (Owner)
    {
        // Making sure we do not collide with our own line trace
        TraceParams.AddIgnoredActor(Owner);
    }
    
    FVector CameraLocation = FVector::ZeroVector;
    FRotator CameraRotation = FRotator::ZeroRotator;
    
    if (const AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(GetOwner()))
    {
        CameraLocation = FPSCharacter->GetCameraComponent()->GetComponentLocation();
        CameraRotation = FPSCharacter->GetCameraComponent()->GetComponentRotation();
    }
    
    const FVector TraceDirection = CameraRotation.Vector();
    const FVector TraceEndLocation = CameraLocation + TraceDirection * InteractDistance;

    // Checking if we hit something with our line trace
    if (GetWorld()->LineTraceSingleByChannel(InteractionHit, CameraLocation, TraceEndLocation, ECC_WorldStatic, TraceParams))
    {
        // Checking if the actor we hit implements the interaction interface
        if(InteractionHit.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
        {
            bCanInteract = true;

            // Checking between classes that derive from ASInteract and updating variables accordingly
            const AInteractionActor* InteractionActor = Cast<AInteractionActor>(InteractionHit.GetActor());
            const AWeaponPickup* InteractedPickup = Cast<AWeaponPickup>(InteractionHit.GetActor());
            AAmmoPickup* AmmoPickup = Cast<AAmmoPickup>(InteractionHit.GetActor());
            if (InteractionActor)
            {
                InteractText = InteractionActor->PopupDescription;
            }
            else if (AmmoPickup)
            {
                InteractText = AmmoPickup->GetPickupName();
            }
            else if (InteractedPickup)
            {
                bInteractionIsWeapon = true;
                InteractText = InteractedPickup->GetWeaponName();
            }
            else
            {
                InteractText = FText::GetEmpty();
            }
        }
    }
}

// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Checks to see if we are facing something to interact with, and updates the interaction indicator accordingly
    InteractionIndicator();
}

// Binds functionality to the owner's Input Component
void UInteractionComponent::SetupInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
    if (InteractAction)
    {            
        // Interacting with the world
        PlayerInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &UInteractionComponent::WorldInteract);
    }
}

