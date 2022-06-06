// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"

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
	
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor() const
{
	// Set the door rotation into pressure plate
	Owner->SetActorRotation( FRotator(0.f, OpenAngle, 0.f) );
}

void UOpenDoor::CloseDoor() const
{
	// Set the door rotation after leave pressure plate
	Owner->SetActorRotation( FRotator(0.f, 0.f, 0.f) );
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Pool the Trigger volume
	// If the ActorThatOpens is in the volume we will open the door
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

	// Check if it's time to close the door
}

