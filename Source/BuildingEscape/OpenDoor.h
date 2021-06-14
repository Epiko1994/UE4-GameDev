// Copyright Joshua Johansen 2021

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
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void CheckPressurePlate() const;
		
private:

float InitialYaw;
float CurrentYaw;
float InitialZ;
float CurrentZ;
float DoorLastOpened = 0.f;
bool DoorSoundPlayed = false;

UPROPERTY()
UAudioComponent* AudioComponent = nullptr;

UPROPERTY(EditAnywhere)
float PressurePlateActivationWeightInKg = 1.f;

UPROPERTY(EditAnywhere)
float OpenRotationAngle;

UPROPERTY(EditAnywhere)
float DoorOpeningSpeed = 1.f;

UPROPERTY(EditAnywhere)
float DoorClosingSpeed = 1.f;

UPROPERTY(EditAnywhere)
float DoorCloseDelay = 2.f;

UPROPERTY(EditAnywhere)
float OpenZTranslation;

UPROPERTY(EditAnywhere)
ATriggerVolume* Trigger = nullptr;
};