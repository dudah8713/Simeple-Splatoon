// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Actor.h"
#include "CanvasAtcor.generated.h"

UCLASS()
class SPLATOON_API ACanvasAtcor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACanvasAtcor();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Canvas")
	UStaticMeshComponent* CanvasMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Canvas")
	UMaterialInstanceDynamic* CanvasMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Canvas")
	UTextureRenderTarget2D* RenderTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Canvas")
	UTextureRenderTarget2D* RenderTargetMask;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Canvas")
	UMaterialInstanceDynamic* BrushMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Canvas")
	UMaterialInstanceDynamic* WhiteBrushMaterial;

	UFUNCTION(meta=(WorldContext="WorldContextObject"))
	void CreateRenderTarget2D(UObject* WorldContextObject, int32 Width, int32 Height,
		ETextureRenderTargetFormat Format = RTF_RGBA16f, FLinearColor ClearColor = FLinearColor(0.f,0.f,0.f,1.f), bool bAutoGenerateMipMaps = false, bool bSupportUAVs = false);
	
	UFUNCTION(BlueprintCallable, Category = "Canvas")
	void DrawBrush(UTexture2D* BrushTexture, float BPBrushSize, FVector2D DrawUVLocation, const FHitResult& HitResult);
	
	UFUNCTION()
	void InitBrushParmValue(UTexture2D* BrushTexture);

	UFUNCTION()
	void DrawRT(FVector2D DrawUVLocation);

	UFUNCTION()
	void DrawRTM(FVector2D DrawUVLocation);
	
	UFUNCTION()
	void SetParamValAndMaterial();

	UFUNCTION()
	void SetBrushMaterial();

	UFUNCTION()
	void SetBrushColor();
	
	UFUNCTION()
	void GetWidthHeight(int32& OutWidth, int32& OutHeight) const;
	
	UFUNCTION(BlueprintCallable, Category = "Canvas")
	float CalculateArea();

private:
	float BrushSize;
	float TestValue;
	int32 BrushTextureWidth;
	int32 BrushTextureHeight;
};
