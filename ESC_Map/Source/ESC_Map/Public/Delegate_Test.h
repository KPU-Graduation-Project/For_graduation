// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegate_Test.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExcute_Movement);


/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class ESC_MAP_API UDelegate_Test : public UObject
{
public:
	GENERATED_BODY()
	
	UDelegate_Test();
	virtual ~UDelegate_Test() override;


	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Test")
	FExcute_Movement OnMovement;
};
