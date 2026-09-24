#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyObject.generated.h"

UCLASS()

class MYGAME_API MyObject : public AActor
{

    GENERATE_BODY()

    public:
        MyObject();

    protected:
        virtual void BeginPlay() override;

    private:
        virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere) 
    float Speed = 5.0f;                   
}
