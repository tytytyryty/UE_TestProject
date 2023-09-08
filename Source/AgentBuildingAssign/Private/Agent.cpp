// Fill out your copyright notice in the Description page of Project Settings.

#include "Agent.h"
#include "Building.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAgent::AAgent()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAgent::MoveToBuilding(uint8 buildingNumber)
{
	//move to building if it's other than already assigned
	if (buildingNumber != assignedBuildingNumber) {
		TArray<AActor*> buildings;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuilding::StaticClass(), buildings);
		for (AActor* buildingActor : buildings)
		{
			ABuilding* building = Cast<ABuilding>(buildingActor);
			bool isTargetBuilding = building && building->number == buildingNumber;
			if (isTargetBuilding)
			{
				//before moving agent needs to stop jumping, playing sound etc
				stopAllActions();

				isMoving = true;
				FVector moveFrom = GetActorLocation();
				FVector moveTo = building->GetActorTransform().GetLocation();
				movementVector = moveTo - moveFrom;
				movementVector.Normalize();

				assignedBuildingNumber = buildingNumber;
				break;
			}
		}
	}
}

void AAgent::StopMoving()
{
	isMoving = false;
}

void AAgent::Jump()
{
	isJumping = true;
	jumpStartLocationZ = GetActorLocation().Z;
	jumpEndLocationZ = jumpStartLocationZ + jumpHeight;
}

void AAgent::ChangeColor()
{	
	//changing color is possible only once at single use, so no need to run below code more times
	if (!isColorChanged)
	{
		UStaticMeshComponent* mesh = FindComponentByClass<UStaticMeshComponent>();
		UMaterialInterface* material = mesh->GetMaterial(0);
		UMaterialInstanceDynamic* dynamicMaterial = UMaterialInstanceDynamic::Create(material, NULL);
		dynamicMaterial->SetVectorParameterValue(FName(TEXT("Color")), colorToChange);
		mesh->SetMaterial(0, dynamicMaterial);
		isColorChanged = true;
	}
}

void AAgent::PlaySound()
{
	spawnedSound = UGameplayStatics::SpawnSoundAtLocation(this, soundToPlay, GetActorLocation());
}

void AAgent::stopAllActions()
{
	//stop jumping
	isJumping = false;
	
	//stop playing sound
	if (spawnedSound)
	{
		spawnedSound->Stop();
		spawnedSound = NULL;
	}
}

// Called when the game starts or when spawned
void AAgent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//move if assigned and after finishing current jump
	if (isMoving && isCurrentJumpFinished)
	{
		FVector location = GetActorLocation();
		location.X += movementVector.X * moveSpeed * DeltaTime;
		location.Y += movementVector.Y * moveSpeed * DeltaTime;
		SetActorLocation(location);
	}

	//jump if assigned or if agent did not yet get down from currently started jump
	if (isJumping || !isCurrentJumpFinished)
	{
		isCurrentJumpFinished = false;
		FVector location = GetActorLocation();
		if (isJumpUp) 
		{
			location.Z += jumpSpeed * DeltaTime;
			if (location.Z > jumpEndLocationZ)
			{
				location.Z = jumpEndLocationZ;
				isJumpUp = false;
			}
		}
		else 
		{
			location.Z -= jumpSpeed * DeltaTime;
			if (location.Z < jumpStartLocationZ)
			{
				location.Z = jumpStartLocationZ;
				isJumpUp = true;
				isCurrentJumpFinished = true;
			}
		}
		SetActorLocation(location);
	}
}

void AAgent::SetupPlayerInputComponent(class UInputComponent* AgentInputComponent)
{
	Super::SetupPlayerInputComponent(AgentInputComponent);
}

