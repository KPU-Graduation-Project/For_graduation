// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

/**
 * 
 */
UCLASS()
class ESC_MAP_API AFPSHUD : public AHUD
{
	GENERATED_BODY()

protected:
	// 크로스 헤어 텍스처
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairTexture;

public:
	// HUD 에 대한 주요 드로 콜
	virtual void DrawHUD() override;
};
