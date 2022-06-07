// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "DrawDebugHelpers.h"

#define OUT 

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector  PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Ищем игрока от первого лица в мире: GetWorld()->GetFirstPlayerController()
	// Ищем точку зрения игрока: GetPlayerViewPoint(...)
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation );
	
	// UE_LOG(LogTemp, Warning, TEXT("Player Location: %s | Player Rotation: %s"),
	// 	*PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString())

	// Конечная точка вектора направления
	// FVector LineTraceEnd = PlayerViewPointLocation + FVector(0.f, 0.f, 50.f);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	
	// Визуализируем вектор
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,			// Чтобы в пространстве не оставался след, мы хотим чтобы она перересовывалась каждый кадр
		0.f,			// Время жизни 0, чтобы не сохранялся луч в пространстве какое-то время
		0.f,
		10.f			// Толщина 
	);
}

