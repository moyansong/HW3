// Fill out your copyright notice in the Description page of Project Settings.


#include "TwinblastCharacter.h"
//#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "AttributeComponent.h"
#include "Blueprint/UserWidget.h"
#include "WorldUserWidget.h"

// Sets default values
ATwinblastCharacter::ATwinblastCharacter()
{
	// Set size for collision capsule碰撞胶囊大小
	//胶囊体比脚低会让人悬浮在空中，因为胶囊体提前触地
	GetCapsuleComponent()->InitCapsuleSize(42.f, 86.0f);

	//该角色每帧都调用Tick
	PrimaryActorTick.bCanEverTick = true;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	MaxWalkSpeed = 500.0f;
	MaxRunSpeed = 1200.0f;
	FireByLeft = true;

	//人物随着Controll转
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	

	// 设置offset
	ProjectileOffset.Set(80.0f, 0.0f, 80.0f);
	LeftSparkOffset.Set(80.0f, -20.0f, 85.0f);
	RightSparkOffset.Set(75.0f, 25.0f, 75.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f); 
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxFlySpeed = 1000.f;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	//GetCharacterMovement()->SetUpdatedComponent(CollisionComponent);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->SocketOffset = FVector(0.0f, 80.0f, 65.0f);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; // Camera rotate relative to arm

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
}

// Called when the game starts or when spawned
void ATwinblastCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayAnimMontage(StartMontage, 1.0f);
	if (AttributeWidgetClass != nullptr)
	{
		AttributeWidget = CreateWidget<UUserWidget>(GetWorld(), AttributeWidgetClass);
		if (AttributeWidget != nullptr)
		{
			AttributeWidget->AddToViewport();
		}
	}
	if (MoblieControlWidgetClass != nullptr)
	{
		MoblieControlWidget = CreateWidget<UUserWidget>(GetWorld(), MoblieControlWidgetClass);
		if (MoblieControlWidget != nullptr)
		{
			MoblieControlWidget->AddToViewport();
		}
	}
	/*if (HealthBarWidgetClass != nullptr)
	{
		HealthBarWidget = CreateWidget<UWorldUserWidget>(this, HealthBarWidgetClass);
		if (HealthBarWidget != nullptr)
		{
			HealthBarWidget->AddToViewport();
		}
	}*/
}

// Called every frame
void ATwinblastCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//float v = GetVelocity().Size();
	CrosshairSpread = UKismetMathLibrary::FMin(CrosshairSpread + GetVelocity().Size() / 15.0f, 40.0f);
	if (CrosshairSpread > 0.0f) CrosshairSpread -= 1.0f;
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::FromInt(AttributeComponent->Health));
}

void ATwinblastCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ATwinblastCharacter::OnHealthChange);
}

// Called to bind functionality to input绑定输入
void ATwinblastCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	//"第一个参数是在项目设置的input中设置的名字(可以与回调函数名字不一样)，第二个参数是设置的键位，后面的func是回调函数"
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATwinblastCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATwinblastCharacter::MoveRight);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ATwinblastCharacter::ChangeWalkSpeed);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnAtRate", this, &ATwinblastCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpAtRate", this, &ATwinblastCharacter::LookUpAtRate);

	//手机的触摸绑定
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATwinblastCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATwinblastCharacter::TouchStopped);
	PlayerInputComponent->BindTouch(IE_Repeat, this, &ATwinblastCharacter::TouchUpdate);
	
	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATwinblastCharacter::OnResetVR);

	//Fire
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATwinblastCharacter::Fire);
}

void ATwinblastCharacter::OnResetVR()
{
	//UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

//FingerIndex按下的手指数
void ATwinblastCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ATwinblastCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void ATwinblastCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y * BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}

void ATwinblastCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//绕着z轴转才是向左/右转
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATwinblastCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATwinblastCharacter::MoveForward(float Value)
{
	//Value是在input中设置的scale
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward找当前方向的正前方
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);//旋转到当前方向的旋转向量

		// get forward vector
		//FRotationMatrix(YawRotation)先绕着z轴转到当前方向再取x方向
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);//或许可以实现别的函数实现更快的移动(速度)
	}
}

void ATwinblastCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		//AddMovementInput(GetActorRightVector(), Value);//获取人物右边的向量，如果一直按着就会转圈
		AddMovementInput(Direction, Value);//这里的Value也只能接收-1到1的，再大没用
	}
}

void ATwinblastCharacter::ChangeWalkSpeed()
{
	if (GetCharacterMovement()->MaxWalkSpeed == MaxWalkSpeed)
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxRunSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	}
}

void ATwinblastCharacter::Fire()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Fire")));
	//获取摄像机变换,以摄像机位置加偏移生成子弹
	//这些location都是摄像机空间坐标
	//FVector CameraLocation;//拿摄像机的方向当速度方向只能让子弹射向正前方
	//FRotator CameraRotation = Controller->GetControlRotation();
	//GetActorEyesViewPoint(CameraLocation, CameraRotation);

	CrosshairSpread = 40.0f;

	//奔跑时不能开枪
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;

	//获取枪口位置位置
	FTransform LeftGunTransform = GetMesh()->GetSocketTransform(FName(TEXT("weapon_forestock_bott_l")));
	FTransform RightGunTransform = GetMesh()->GetSocketTransform(FName(TEXT("weapon_forestock_bott_r")));
	FTransform LeftIndexFingerTransform = GetMesh()->GetSocketTransform(FName(TEXT("index_02_l")));
	FTransform RightIndexFingerTransform = GetMesh()->GetSocketTransform(FName(TEXT("index_02_r")));
	FVector LeftProjectileLocation = LeftGunTransform.GetLocation();
	FVector RightProjectileLocation = RightGunTransform.GetLocation();
	FRotator LeftProjectileRotation(LeftGunTransform.GetRotation());
	FRotator RightProjectileRotation(RightGunTransform.GetRotation());

	//指向世界/关卡的指针
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		//在左枪口位置生成发射物。
		AProjectile* LeftProjectile = World->SpawnActor<AProjectile>(ProjectileClass, LeftGunTransform, SpawnParams);
		if (LeftProjectile)
		{
			//速度方向是枪的位置-食指的位置
			FVector LeftLaunchDirection = LeftGunTransform.GetLocation() - LeftIndexFingerTransform.GetLocation();
			LeftProjectile->FireInDirection(LeftLaunchDirection);
			UGameplayStatics::SpawnEmitterAtLocation(World, GunSpark, LeftProjectileLocation, LeftProjectileRotation, 1.0f);
			UGameplayStatics::SpawnEmitterAtLocation(World, ShellCase, LeftProjectileLocation, LeftProjectileRotation, 1.0f);
		}

		AProjectile* RightProjectile = World->SpawnActor<AProjectile>(ProjectileClass, RightGunTransform, SpawnParams);
		if (RightProjectile)
		{
			FVector RightLaunchDirection = RightGunTransform.GetLocation() - RightIndexFingerTransform.GetLocation();
			RightProjectile->FireInDirection(RightLaunchDirection);
			UGameplayStatics::SpawnEmitterAtLocation(World, GunSpark, RightProjectileLocation, RightProjectileRotation, 1.0f);
			UGameplayStatics::SpawnEmitterAtLocation(World, ShellCase, RightProjectileLocation, RightProjectileRotation, 1.0f);
		}
	}
	
	//播放开枪蒙太奇
	PlayAnimMontage(FireMontage, 1.0f);
}

//void ATwinblastCharacter::SpawnProjectile(UWorld* World, UClass* Class, const FTransformt& Transform, const FVector& Direction, const FActorSpawnParameters& SpawnParams)
//{
//	AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, Transform, SpawnParams);
//	FVector ProjectileLocation = Transform.GetLocation(); 
//  FRotator ProjectileRotation(LeftGunTransform.GetRotation());
//	if (Projectile)
//	{
//		//速度方向是枪的位置-食指的位置
//		Projectile->FireInDirection(Direction);
//		UGameplayStatics::SpawnEmitterAtLocation(World, GunSpark, Transform.GetLocation, ProjectileRotation, 1.0f);
//		UGameplayStatics::SpawnEmitterAtLocation(World, ShellCase, ProjectileLocation, ProjectileRotation, 1.0f);
//	}
//}

void ATwinblastCharacter::OnHealthChange(AActor* InstigatocActor, UAttributeComponent* OwningCopmonent, float NewHealth, float Damage)
{
	if (NewHealth <= 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);//不接收任何输入
	}
}