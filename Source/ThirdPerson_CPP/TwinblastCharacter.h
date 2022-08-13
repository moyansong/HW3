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

	// Ҫ���ɵķ������࣬����ģ�嶨���Ҫ����ͼ���ҵ���ӦCategoryѡ���Ӧ��
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

	//ͨ�������ֻ���Ļƽ����ת�ӽ�
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

//����----------------------------------------------------------------------------------------------------
	//����һ��ת���̧ͷ�ĽǶȴ�С
	UPROPERTY(EditAnywhere)
		float BaseTurnRate;
	UPROPERTY(EditAnywhere)
		float BaseLookUpRate;

	//�����ֻ������ֿ�ǹ
	bool FireByLeft;

	//����ٶ�
	float MaxWalkSpeed;
	float MaxRunSpeed;

	//�ӵ�����λ������������λ�õ�ƫ�ơ�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector ProjectileOffset;
	//��ǹ�ڻ�ƫ��λ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector LeftSparkOffset;
	//��ǹ�ڻ�ƫ��λ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector RightSparkOffset;
	//��̬ʮ��׼�ǵķ�ɢ�̶�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float CrosshairSpread;
//ParticleSystem----------------------------------------------------------------------------------------------
	//��ǹ��
	UPROPERTY(EditAnywhere, Category = "Emitter")
		UParticleSystem* GunSpark;
	//����
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
	//������ACharacter�н�Mesh,���ﲻ����������ֱ�Ӹ�ֵ������ȥ��ͼ��༭

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
		UUserWidget* AttributeWidget;//ֻʹ��UserWidgetָ������ͼ���޷�����ѡ�ֻ��ͨ��ģ�崴����

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> MoblieControlWidgetClass;
	UPROPERTY()
		UUserWidget* MoblieControlWidget;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> HealthBarWidgetClass;
	UPROPERTY()
		UWorldUserWidget* HealthBarWidget;*/

//����-------------------------------------------------------------------------------------------------------------------
	UFUNCTION(BlueprintCallable)
		void Fire();

	UFUNCTION(BlueprintCallable)
		void OnHealthChange(AActor* InstigatocActor, UAttributeComponent* OwningCopmonent, float NewHealth, float Damage);

	//UFUNCTION(BlueprintCallable)
		//void SpawnProjectile(UWorld* World, UClass* Class, const FTransformt& Transform, const FVector& Direction, const FActorSpawnParameters& SpawnParams);
};
