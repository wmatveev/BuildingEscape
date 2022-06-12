// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// На сколько далеко от игрока мы можем достать в см
	float Reach = 100.f;

	UPhysicsHandleComponent *PhysicsHandle = nullptr;
	UInputComponent *InputComponent = nullptr;

	// Посылает луч и хватает то, до чего достал
	void Grab();

	// Вызывается, когда клавиша хватания отпущена
	void Release();

	// Находит прикрепленную физическую ручку
	void FindPhysicsHandleComponent();

	// Устанавливае прикрепленный input компонент
	void SetupInputComponent();

	// Возвращает удар (hit) для первого физического тела в досягаемости
	const FHitResult GetFirstPhysicsBodyInReach();
};
