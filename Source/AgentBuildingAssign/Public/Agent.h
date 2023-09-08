// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/AudioComponent.h"
#include "Agent.generated.h"

UCLASS()
class AGENTBUILDINGASSIGN_API AAgent : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAgent();

	UPROPERTY(EditAnywhere)
		float moveSpeed;

	UPROPERTY(EditAnywhere)
		float jumpSpeed;

	UPROPERTY(EditAnywhere)
		float jumpHeight;

	UPROPERTY(EditAnywhere)
		FLinearColor colorToChange;

	UPROPERTY(EditAnywhere)
		USoundBase* soundToPlay;

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void MoveToBuilding(uint8 buildingNumber);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void StopMoving();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void Jump();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void ChangeColor();

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void PlaySound();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	uint8 assignedBuildingNumber;

	bool isMoving;
	FVector movementVector;

	bool isJumping;
	float jumpStartLocationZ;
	float jumpEndLocationZ;
	bool isJumpUp = true;
	bool isCurrentJumpFinished = true;

	bool isColorChanged;
	
	UAudioComponent* spawnedSound;

	void stopAllActions();
};