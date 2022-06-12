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

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Ищем прикрепленный PhysicsHandle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("%s компонент не содежит PhysicsHandle"), *GetOwner()->GetName());
	}
}

// Ищем прикрепленный InputComponent
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s компонент не содежит InputComponent"), *GetOwner()->GetName());
	}
	else {
		InputComponent->BindAction("Grab", IE_Pressed,  this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}


void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))

	// Выпустим луч и посмотрим, дотянемся ли мы до
	// какого либо актора с установленным каналом столкновения физического тела
	const FHitResult HitResult = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();
	AActor *ActorHit = HitResult.GetActor();
	
	// Если физическая ручка прикреплена
	if(ActorHit)
	{
		// Двигаем объект, который мы держим
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),	// Местоположение захвата
			ComponentToGrab->GetOwner()->GetActorRotation());	// Вращение
	}
}

void UGrabber::Release()
{
	if(PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Grab released"))
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

	// Если физическая ручка прикреплена
	if(PhysicsHandle->GrabbedComponent)
	{
		// То мы будем двигать объект, который держим
		PhysicsHandle->SetTargetLocation(LineTraceEnd);	// Установить местонахождение цели
	}

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
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
	// DrawDebugLine(
	// 	GetWorld(),
	// 	PlayerViewPointLocation,
	// 	LineTraceEnd,
	// 	FColor(255, 0, 0),
	// 	false,			// Чтобы в пространстве не оставался след, мы хотим чтобы она перересовывалась каждый кадр
	// 	0.f,			// Время жизни 0, чтобы не сохранялся луч в пространстве какое-то время
	// 	0.f,
	// 	10.f			// Толщина 
	// );


	// Установить параметры запроса
	FCollisionQueryParams TraceParametrs(FName(TEXT("")), false, GetOwner());
	
	FHitResult Hit;
	
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit, // Результат удара
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParametrs
	);

	AActor *ActorHit = Hit.GetActor();
	if(ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Actor is: %s"), *ActorHit->GetName());
	}

	return Hit;
}

