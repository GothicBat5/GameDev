#include "MyObject.h"

MyObject::MyObject();
{
    PrimaryActorTick.bCanEverTick = true;
}

void MyObject::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("Hola Hola!"));
}

void  MyObject::Tick(float DeltaTime)
{
    Super::Tick(float DeltaTime);

    AddActorLocalRotation(FRotator(0.0f, Speed * DeltaTime, 0.0f));
}
