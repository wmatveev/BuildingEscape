// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// Find the owning Actor
	Owner = GetOwner();

	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *Owner->GetName())
	}
	
	// ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Pool the Trigger volume
	// If the ActorThatOpens is in the volume we will open the door

	if( GetTotalMassOfActorsOnPlate() > TriggerMass ) // @todo make into a parametr
	{
		// Вещаем на это событие
		OnOpenRequest.Broadcast();
	}
	else
	{
		OnCloseRequest.Broadcast();		
	}
	
/*
	if( PressurePlate->IsOverlappingActor(ActorThatOpens) )
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if( (GetWorld()->GetTimeSeconds() - LastDoorOpenTime) > DoorCloseDelay )
			CloseDoor();
	}
*/
	// Check if it's time to close the door
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() const
{
	float TotalMass = 0.f;

	if(!PressurePlate) { return TotalMass; }
	
	// Ищем всех акторов, находящихся в пересечении с триггером
	TArray<AActor*> OverlappingActors;
	
	PressurePlate->GetOverlappingActors(OUT OverlappingActors); 

	// Плюсуем их массу
	for(const auto &Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	
	return TotalMass;
}
