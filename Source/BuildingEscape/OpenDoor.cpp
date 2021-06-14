// Copyright Joshua Johansen 2021

#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "Math/TransformNonVectorized.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenRotationAngle += InitialYaw; // OpenRotationAngle = InitialYaw + OpenRotationAngle;
	InitialZ = GetOwner()->GetActorLocation().Z;
	CurrentZ = InitialZ;
	OpenZTranslation += InitialZ;

	CheckPressurePlate();
	FindAudioComponent();
}

void UOpenDoor::CheckPressurePlate() const
{
	if(!Trigger)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component assigned to it, but no PressurePlate set!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner() -> FindComponentByClass<UAudioComponent>();

	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Missing Audio component!"), *GetOwner()->GetName());
	}

}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TotalMassOfActors() >= PressurePlateActivationWeightInKg)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		//if the door has been open longer than x seconds
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenRotationAngle, DeltaTime, DoorOpeningSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	CurrentZ = FMath::FInterpTo(CurrentZ, OpenZTranslation, DeltaTime, DoorOpeningSpeed);
	FVector DoorZTranslation = GetOwner()->GetActorLocation();
	DoorZTranslation.Z = CurrentZ;
	GetOwner()->SetActorLocation(DoorZTranslation);

	if (!AudioComponent) {return;}
	if (DoorSoundPlayed == false)
	{
		AudioComponent -> Play();
		DoorSoundPlayed = true;
	}
	// CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	// FRotator DoorOpen(0.f, 0.f, 0.f);
	// DoorOpen.Yaw = FMath::FInterpTo(CurrentYaw, OpenRotationAngle, DeltaTime, 1);
	// GetOwner() -> SetActorRotation(DoorOpen);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorClosingSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if (!AudioComponent) {return;}
	if (DoorSoundPlayed == true)
	{
		AudioComponent -> Play();
		DoorSoundPlayed = false;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	if (!Trigger) {return TotalMass;}
	TArray<AActor*> OverlappingActors;
	Trigger->GetOverlappingActors(OUT OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}