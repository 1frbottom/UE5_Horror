// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor/HRInteractableActorBase.h"
#include "Components/TextBlock.h"
#include "Character/HRCharacterPlayer.h"

// Sets default values
AHRInteractableActorBase::AHRInteractableActorBase()
{
    bIsInteractable = true;

    USceneComponent* DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DefaultSceneRoot;

    // Static Mesh
    Mesh1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh1"));
    Mesh1->SetupAttachment(RootComponent);
    Mesh1->SetCollisionProfileName(TEXT("InteractableActorBase_Mesh"));

    Mesh2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh2"));
    Mesh2->SetupAttachment(RootComponent);
    Mesh2->SetCollisionProfileName(TEXT("InteractableActorBase_Mesh"));
    Mesh2->SetRelativeLocation(FVector(0.f, 100.f, 0.f));

    // box collision component
    InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
    InteractionBox->SetupAttachment(RootComponent);
    InteractionBox->SetCollisionProfileName(TEXT("InteractableActorBase_Box"));

    InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &AHRInteractableActorBase::OnInteractionBoxBeginOverlap);
    InteractionBox->OnComponentEndOverlap.AddDynamic(this, &AHRInteractableActorBase::OnInteractionBoxEndOverlap);

    // anchor component
    InteractionWidgetAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionWidgetAnchor"));
    InteractionWidgetAnchor->SetupAttachment(RootComponent);
    InteractionWidgetAnchor->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

    // widget component
    InteractionPromptWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionPromptWidget"));
    InteractionPromptWidget->SetupAttachment(InteractionWidgetAnchor);
    InteractionPromptWidget->SetWidgetSpace(EWidgetSpace::Screen);
    InteractionPromptWidget->SetDrawAtDesiredSize(true);
    // InteractionPromptWidget->SetDrawSize(FVector2D(100.f, 30.f));        // 고정 크기 설정 시
    InteractionPromptWidget->SetVisibility(false);
    InteractionPromptWidget->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    // InteractionPromptWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

        // debug
    UE_LOG(LogTemp, Warning, TEXT("CONSTRUCTOR: Widget Relative Location for %s: X=%f, Y=%f, Z=%f"),
        *GetName(), InteractionPromptWidget->GetRelativeLocation().X,
        InteractionPromptWidget->GetRelativeLocation().Y,
        InteractionPromptWidget->GetRelativeLocation().Z);

    static ConstructorHelpers::FClassFinder<UUserWidget> PromptWidgetClassRef(TEXT("/Game/UI/WBP_Interaction.WBP_Interaction_C"));
    if (PromptWidgetClassRef.Class)
    {
        InteractionPromptWidget->SetWidgetClass(PromptWidgetClassRef.Class);
    }

    InteractionText = FText::FromString(TEXT("Default_from_HRint-Base.cpp")); // 기본 상호작용 텍스트
}

void AHRInteractableActorBase::OnInteractionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AHRCharacterPlayer* Player = Cast<AHRCharacterPlayer>(OtherActor))
    {
        if (Player->IsLocallyControlled())
        {
            Player->RegisterInteractableActor(this);
        }
    }
}

void AHRInteractableActorBase::OnInteractionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (AHRCharacterPlayer* Player = Cast<AHRCharacterPlayer>(OtherActor))
    {
        if (Player->IsLocallyControlled())
        {
            Player->UnregisterInteractableActor(this);
        }
    }
}

void AHRInteractableActorBase::OnFocusGained()
{
    if (IsInteractable())
    {
        if (InteractionPromptWidget)
        {
            UUserWidget* UserWidgetObject = InteractionPromptWidget->GetUserWidgetObject();
            if (UserWidgetObject)
            {
                // calling textblock from wbp_interaction
                UTextBlock* PromptTextBlock = Cast<UTextBlock>(UserWidgetObject->GetWidgetFromName(TEXT("InteractionText_WBP")));
                if (PromptTextBlock)
                {
                    PromptTextBlock->SetText(GetInteractionText());
                }
            }
            InteractionPromptWidget->SetVisibility(true);
        }
    }
    else
    {
        OnFocusLost();
    }
}

void AHRInteractableActorBase::OnFocusLost()
{
    if (InteractionPromptWidget)
    {
        InteractionPromptWidget->SetVisibility(false);
    }
}

FText AHRInteractableActorBase::GetInteractionText_Implementation()
{
    return InteractionText;
}

void AHRInteractableActorBase::UpdateInteractionWidgetText()
{
    if (InteractionPromptWidget && InteractionPromptWidget->GetWidget())
    {
        UUserWidget* UserWidget = InteractionPromptWidget->GetWidget();
        if (UserWidget)
        {
            UTextBlock* PromptTextBlock = Cast<UTextBlock>(UserWidget->GetWidgetFromName(TEXT("InteractionText_WBP")));
            if (PromptTextBlock)
            {
                PromptTextBlock->SetText(GetInteractionText());
            }
        }
    }
}

// only for debug
void AHRInteractableActorBase::BeginPlay()
{
    Super::BeginPlay();

    // 이 액터의 InteractionPromptWidget의 상대 위치를 출력
    if (InteractionPromptWidget)
    {
        FVector CurrentRelativeLocation = InteractionPromptWidget->GetRelativeLocation();
        UE_LOG(LogTemp, Warning, TEXT("Widget Relative Location for %s: X=%f, Y=%f, Z=%f"),
            *GetName(), CurrentRelativeLocation.X, CurrentRelativeLocation.Y, CurrentRelativeLocation.Z);
    }
}