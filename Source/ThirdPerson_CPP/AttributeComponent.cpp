// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	MaxHealth = 100.0f;
	Health = MaxHealth;
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAttributeComponent::ApplyHealthChange(float Damage)
{
	Health -= Damage;
	Health = UKismetMathLibrary::FMax(0.0, Health);
	OnHealthChanged.Broadcast(nullptr, this, Health, Damage);
	return true;
}

bool UAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}