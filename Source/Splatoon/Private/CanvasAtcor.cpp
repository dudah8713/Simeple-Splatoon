// Fill out your copyright notice in the Description page of Project Settings.


#include "CanvasAtcor.h"
#include "Elements/Columns/TypedElementAlertColumns.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Util/ColorConstants.h"

ACanvasAtcor::ACanvasAtcor()
{
	PrimaryActorTick.bCanEverTick = false;

	CanvasMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CanvasMesh"));
	RootComponent = CanvasMeshComponent;

	// static ConstructorHelpers::FObjectFinder<UStaticMesh> Plane(TEXT("/Engine/BasicShapes/Plane.Plane"));
	// if (Plane.Succeeded())
	// {
	// CanvasMeshComponent->SetStaticMesh(Plane.Object);
	// }
}

void ACanvasAtcor::BeginPlay()
{
	Super::BeginPlay();
	GetWidthHeight(BrushTextureWidth, BrushTextureHeight);

	CreateRenderTarget2D(this, BrushTextureWidth, BrushTextureHeight);
	SetParamValAndMaterial();
	SetBrushMaterial();
	SetBrushColor();
}

void ACanvasAtcor::CreateRenderTarget2D(UObject* WorldContextObject, int32 Width, int32 Height,
                                        ETextureRenderTargetFormat Format, FLinearColor ClearColor,
                                        bool bAutoGenerateMipMaps, bool bSupportUAVs)
{
	RenderTarget = NewObject<UTextureRenderTarget2D>(WorldContextObject);
	RenderTarget->ClearColor = ClearColor;
	RenderTarget->InitCustomFormat(Width, Height, PF_B8G8R8A8, false);
	RenderTarget->UpdateResourceImmediate(true);

	RenderTargetMask = NewObject<UTextureRenderTarget2D>(WorldContextObject);
	RenderTargetMask->ClearColor = ClearColor;
	RenderTargetMask->InitCustomFormat(Width, Height, PF_B8G8R8A8, false);
	RenderTargetMask->UpdateResourceImmediate(true);
}

void ACanvasAtcor::DrawBrush(UTexture2D* BrushTexture, float BPBrushSize, FVector2D DrawUVLocation,
                             const FHitResult& HitResult)
{
	BrushSize = BPBrushSize * 1.f;
	InitBrushParmValue(BrushTexture);
	DrawRT(DrawUVLocation);
	DrawRTM(DrawUVLocation);
}

void ACanvasAtcor::InitBrushParmValue(UTexture2D* BrushTexture)
{
	if (!BrushMaterial || !BrushTexture)
	{
		UE_LOG(LogTemp, Warning, TEXT("Brush Material or BrushTexture is null"));
		return;
	}

	BrushMaterial->SetTextureParameterValue(FName("BrushTexture"), BrushTexture);
	WhiteBrushMaterial->SetTextureParameterValue(FName("WhiteBrushTexture"), BrushTexture);
}

void ACanvasAtcor::DrawRT(FVector2D DrawUVLocation)
{
	if (!RenderTarget || !BrushMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("DrawRT failed: missing RenderTarget or BrushMaterial"));
		return;
	}

	UCanvas* Canvas = nullptr;
	FVector2D Size(BrushTextureWidth, BrushTextureHeight);
	FDrawToRenderTargetContext Context;

	if (RenderTargetMask)
	{
		Size = FVector2D(RenderTarget->GetSurfaceWidth(), RenderTarget->GetSurfaceHeight());
	}

	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, RenderTarget, Canvas, Size, Context);

	if (RenderTarget)
	{
		// UV 위치를 화면 크기와 렌더 타겟 크기에 맞게 조정
		FVector2D UVScaledPosition = FVector2D(DrawUVLocation.X * Size.X, DrawUVLocation.Y * Size.Y);

		FVector2D HalfBrushSize(BrushSize / 2.f, BrushSize / 2.f);

		// ScreenPosition은 텍스처가 화면에 올바르게 그려지도록 위치를 결정
		FVector2D ScreenPosition = UVScaledPosition - HalfBrushSize;

		UE_LOG(LogTemp, Log, TEXT("Drawing at: %s Size: %f"), *ScreenPosition.ToString(), BrushSize);

		// 텍스처를 화면에 그리기
		Canvas->K2_DrawMaterial(BrushMaterial, ScreenPosition, FVector2D(BrushSize, BrushSize),
		                        FVector2D(0.f, 0.f), FVector2D(1.f, 1.f),
		                        0.f, FVector2D(0.5f, 0.5f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Canvas is null after BeginDrawCanvasToRenderTarget"));
	}

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
}

void ACanvasAtcor::DrawRTM(FVector2D DrawUVLocation)
{
	if (!RenderTargetMask || !BrushMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("DrawRT failed: missing RenderTarget or BrushMaterial"));
		return;
	}

	UCanvas* Canvas = nullptr;
	FVector2D Size(BrushTextureWidth, BrushTextureHeight);
	FDrawToRenderTargetContext Context;

	if (RenderTargetMask)
	{
		Size = FVector2D(RenderTargetMask->GetSurfaceWidth(), RenderTargetMask->GetSurfaceHeight());
	}

	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, RenderTargetMask, Canvas, Size, Context);

	if (RenderTargetMask)
	{
		// UV 위치를 화면 크기와 렌더 타겟 크기에 맞게 조정
		FVector2D UVScaledPosition = FVector2D(DrawUVLocation.X * Size.X, DrawUVLocation.Y * Size.Y);
		
		FVector2D HalfBrushSize(BrushSize / 2.f, BrushSize / 2.f);

		// ScreenPosition은 텍스처가 화면에 올바르게 그려지도록 위치를 결정
		FVector2D ScreenPosition = UVScaledPosition - HalfBrushSize;

		UE_LOG(LogTemp, Log, TEXT("Drawing at: %s Size: %f"), *ScreenPosition.ToString(), BrushSize);

		// 텍스처를 화면에 그리기
		Canvas->K2_DrawMaterial(WhiteBrushMaterial, ScreenPosition, FVector2D(BrushSize, BrushSize),
		                        FVector2D(0.f, 0.f), FVector2D(1.f, 1.f),
		                        0.f, FVector2D(0.5f, 0.5f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Canvas is null after BeginDrawCanvasToRenderTarget"));
	}

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
	CalculateArea();
}

void ACanvasAtcor::SetParamValAndMaterial()
{
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(
		NULL, TEXT("/Game/Painters/Materials/M_Canvas.M_Canvas"));
	CanvasMaterial = UMaterialInstanceDynamic::Create(Material, this);

	if (CanvasMaterial)
	{
		if (RenderTarget)
		{
			CanvasMaterial->SetTextureParameterValue(FName("RenderTarget"), RenderTarget);
		}

		if (RenderTargetMask)
		{
			CanvasMaterial->SetTextureParameterValue(FName("RenderTargetMask"), RenderTargetMask);
		}

		CanvasMeshComponent->SetMaterial(0, CanvasMaterial);
	}
}

void ACanvasAtcor::SetBrushMaterial()
{
	UMaterialInterface* Material = LoadObject<UMaterialInterface>(
		nullptr, TEXT("/Game/Painters/Materials/M_Brush.M_Brush"));
	BrushMaterial = UMaterialInstanceDynamic::Create(Material, this);

	Material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Painters/Materials/M_WhiteBrush.M_WhiteBrush"));
	WhiteBrushMaterial = UMaterialInstanceDynamic::Create(Material, this);
}

void ACanvasAtcor::SetBrushColor()
{
	if (BrushMaterial)
	{
		BrushMaterial->SetVectorParameterValue(FName("PaintColor"), FLinearColor(0.f, 0.f, 1.f, 1.f));
	}
}

void ACanvasAtcor::GetWidthHeight(int32& OutWidth, int32& OutHeight) const
{
	if (!CanvasMeshComponent) return;

	// CanvasMesh의 월드 스케일 가져오기
	FVector WorldScale = CanvasMeshComponent->GetComponentTransform().GetScale3D();

	// 텍스쳐의 크기를 보정해줄 값
	const float UnitPreMeter = 100.f;

	// RenderTarget 해상도로 사용할 Width, Height 구함
	OutWidth = FMath::TruncToInt(WorldScale.X * UnitPreMeter);
	OutHeight = FMath::TruncToInt(WorldScale.Y * UnitPreMeter);
}

float ACanvasAtcor::CalculateArea()
{
	TArray<FColor> PixelData;
	FTextureRenderTargetResource* RTResource = RenderTargetMask->GameThread_GetRenderTargetResource();
	FReadSurfaceDataFlags readPixelFlags(RCM_UNorm);

	RTResource->ReadPixels(PixelData);

	int32 CoveredPixels = 0;
	const int32 TotalPixels = PixelData.Num();

	for (const FColor& Pixel : PixelData)
	{
		// 알파 채널 기준 50% 이상인 픽셀을 커버된 것으로 판단
		if (Pixel.A < 128)
		{
			CoveredPixels++;
		}
	}
	float CoveragePercent = (CoveredPixels / (float)TotalPixels) * 100.0f;
	UE_LOG(LogTemp, Log, TEXT("Painted Area Percentage: %f"), CoveragePercent);

	return CoveragePercent;
}
