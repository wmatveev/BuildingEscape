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

	// Ищем прикрепленный PhysicsHandle к пешке
	FindPhysicsHandleComponent();

	// Ищем прикрепленный InputComponent для захвата и отпускания
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

// ReSharper disable once CppMemberFunctionMayBeConst
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))

	// Выпустим луч и посмотрим, дотянемся ли мы до
	// какого либо актора с установленным каналом столкновения физического тела
	const FHitResult HitResult = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();
	const AActor *ActorHit = HitResult.GetActor();
	
	// Если физическая ручка прикреплена
	if(ActorHit)
	{
		// Двигаем объект, который мы держим
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,											// Никакие кости не нужны
			ComponentToGrab->GetOwner()->GetActorLocation(),	// Местоположение захвата
			ComponentToGrab->GetOwner()->GetActorRotation());	// Вращение
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UGrabber::Release()
{
	if(PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Если физическая ручка прикреплена
	if(PhysicsHandle->GrabbedComponent)
	{
		// То мы будем двигать объект, который держим
		PhysicsHandle->SetTargetLocation( GetReachLineEnd() );	// Установить местонахождение цели
	}
}

// Ищем и возвращаем первый встретившийся нам физический компонент
FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult	HitResult;
	FVector		PlayerViewPointLocation;
	FRotator	PlayerViewPointRotation;

	// Ищем игрока от первого лица в мире
	GetReachLineStart(PlayerViewPointLocation, PlayerViewPointRotation);

	// Установить параметры запроса
	FCollisionQueryParams TraceParametrs(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult, // Результат удара
		PlayerViewPointLocation,
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParametrs
	);

	return HitResult;
}

// Ищем игрока от первого лица в мире: GetWorld()->GetFirstPlayerController()
// Ищем точку зрения игрока: GetPlayerViewPoint(...)
// Ищем поворот игрока
void UGrabber::GetReachLineStart(FVector &PlayerLocation, FRotator &PlayerRotation) const
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocation,
		OUT PlayerRotation );
}

FVector UGrabber::GetReachLineEnd() const
{
	FVector  PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetReachLineStart(PlayerViewPointLocation, PlayerViewPointRotation);
	
	// Конечная точка вектора направления
	// FVector LineTraceEnd = PlayerViewPointLocation + FVector(0.f, 0.f, 50.f);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
