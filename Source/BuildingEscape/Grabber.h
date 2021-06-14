// Copyright Joshua Johansen 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Sets default values for this component's properties
	UGrabber();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Reach = 150.f;
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;
	
	void Grab();
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();

	// return the first actor within reach with a physics body
	FHitResult GetFirstPhysicsBodyInReach() const;
	// return the Line Trace End for grab
	FVector GetLineTraceEnd() const;
	// Get Players Position in World
	FVector GetPlayerWorldPosition() const;
};
