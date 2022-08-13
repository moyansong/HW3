// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystem;

UCLASS()
class THIRDPERSON_CPP_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

//Component----------------------------------------------------------------------------------------------
	// 球体碰撞组件
	UPROPERTY(VisibleAnywhere, Category = Projectile)
		USphereComponent* CollisionComponent;

	// 发射物移动组件。
	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	//材料组件
	UPROPERTY(VisibleAnywhere, Category = Material)
		UMaterialInstanceDynamic* ProjectileMaterialInstance;

	//网格组件
	UPROPERTY(VisibleAnywhere, Category = Mesh)
		UStaticMeshComponent* ProjectileMeshComponent;

//ParticleSystem----------------------------------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, Category = "Emitter")
		UParticleSystem* ProjectileSpark;

//回调----------------------------------------------------------------------------------------------
	//打到物体上的回调，打击物和被打击物都可以设置类似回调
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	//发射
	void FireInDirection(const FVector& ShootDirection);

	//仅在第一次打到物体时显示粒子特效
	bool FirstHit;
};
