// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor/HRInteractableActorBase.h"
#include "Components/TextBlock.h"

// Sets default values
AHRInteractableActorBase::AHRInteractableActorBase()
{
    USceneComponent* DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DefaultSceneRoot;

    // box collision component
    InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
    InteractionBox->SetupAttachment(RootComponent);

    InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &AHRInteractableActorBase::OnInteractionBoxBeginOverlap);
    InteractionBox->OnComponentEndOverlap.AddDynamic(this, &AHRInteractableActorBase::OnInteractionBoxEndOverlap);

   
        
    //// anchor component
    //InteractionWidgetAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionWidgetAnchor"));
    //InteractionWidgetAnchor->SetupAttachment(RootComponent);
    //InteractionWidgetAnchor->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

    // widget component
    InteractionPromptWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionPromptWidget"));
    InteractionPromptWidget->SetupAttachment(RootComponent);
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

    InteractionText = FText::FromString(TEXT("Default__from_HRint-Base.cpp")); // 기본 상호작용 텍스트
}

void AHRInteractableActorBase::OnInteractionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APawn* PlayerPawn = Cast<APawn>(OtherActor);
    if (PlayerPawn && PlayerPawn->IsLocallyControlled())    // 로컬 플레이어인지 확인 (멀티플레이어 고려)
    {
        OnFocusGained();
    }
}

void AHRInteractableActorBase::OnInteractionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    APawn* PlayerPawn = Cast<APawn>(OtherActor);
    if (PlayerPawn && PlayerPawn->IsLocallyControlled())
    {
        OnFocusLost();
    }
}

void AHRInteractableActorBase::OnFocusGained()
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