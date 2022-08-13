// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TwinblastCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAttributeComponent;
class AProjectile;
class UUserWidget;
class UWorldUserWidget;

UCLASS()
class THIRDPERSON_CPP_API ATwinblastCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATwinblastCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 要生成的发射物类，类似模板定义的要在蓝图中找到对应Category选择对应类
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AProjectile> ProjectileClass;

protected:
//Input---------------------------------------------------------------------------------------------------------
	UFUNCTION()
		void OnResetVR();

	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION(BlueprintCallable)
		void ChangeWalkSpeed();

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION()
		void TurnAtRate(float Rate);

	UFUNCTION()
		void LookUpAtRate(float Rate);

	UFUNCTION()
		void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	//通过按着手机屏幕平移来转视角
	UFUNCTION()
		void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);

	struct TouchData
	{
		TouchData() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	TouchData	TouchItem;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

//参数----------------------------------------------------------------------------------------------------
	//设置一次转向和抬头的角度大小
	UPROPERTY(EditAnywhere)
		float BaseTurnRate;
	UPROPERTY(EditAnywhere)
		float BaseLookUpRate;

	//用左手还是右手开枪
	bool FireByLeft;

	//最大速度
	float MaxWalkSpeed;
	float MaxRunSpeed;

	//子弹生成位置相对于摄像机位置的偏移。
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector ProjectileOffset;
	//左枪口火花偏移位置
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector LeftSparkOffset;
	//右枪口火花偏移位置
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector RightSparkOffset;
	//动态十字准星的分散程度
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float CrosshairSpread;
//ParticleSystem----------------------------------------------------------------------------------------------
	//开枪火花
	UPROPERTY(EditAnywhere, Category = "Emitter")
		UParticleSystem* GunSpark;
	//弹壳
	UPROPERTY(EditAnywhere, Category = "Emitter")
		UParticleSystem* ShellCase;

//Component----------------------------------------------------------------------------------------------
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UAttributeComponent* AttributeComponent;
	//网格在ACharacter中叫Mesh,这里不用声明可以直接赋值，或者去蓝图里编辑

//Montage--------------------------------------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage* FireByLeftMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage* FireByRightMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage* FireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
		UAnimMontage* StartMontage;

//Widget--------------------------------------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> AttributeWidgetClass;
	UPROPERTY()
		UUserWidget* AttributeWidget;//只使用UserWidget指针在蓝图中无法保存选项，只能通过模板创建？

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> MoblieControlWidgetClass;
	UPROPERTY()
		UUserWidget* MoblieControlWidget;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> HealthBarWidgetClass;
	UPROPERTY()
		UWorldUserWidget* HealthBarWidget;*/

//动作-------------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
		void Fire();

	UFUNCTION(BlueprintCallable)
		void OnHealthChange(AActor* InstigatocActor, UAttributeComponent* OwningCopmonent, float NewHealth, float Damage);

	//UFUNCTION(BlueprintCallable)
		//void SpawnProjectile(UWorld* World, UClass* Class, const FTransformt& Transform, const FVector& Direction, const FActorSpawnParameters& SpawnParams);
};
