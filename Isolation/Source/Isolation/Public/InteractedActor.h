// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractedActor.generated.h"

UCLASS()
class ISOLATION_API ASInteractedActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASInteractedActor();

	// The function called from the delegating SInteractionActor
	void Interacted();

	// Function implemented in blueprints from which to perform logic
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteracted();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
