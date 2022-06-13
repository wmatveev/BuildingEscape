// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void OpenDoor() const;
	void CloseDoor() const;

	// Возвращает общую массу в кг
	float GetTotalMassOfActorsOnPlate() const;
	
	UPROPERTY(EditAnywhere)
	float OpenAngle = -90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume *PressurePlate;

	// Задержка закрытия двери
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;
	float LastDoorOpenTime; // Время последнего открывания двери
	
	AActor *Owner;			// Актор обладающий дверью
	// AActor *ActorThatOpens; // Remember pawn inherits from actor
};
