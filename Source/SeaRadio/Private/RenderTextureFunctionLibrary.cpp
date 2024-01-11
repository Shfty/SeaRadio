// Fill out your copyright notice in the Description page of Project Settings.

#include "RenderTextureFunctionLibrary.h"
#include "Engine/TextureRenderTarget2D.h"

TArray<FColor> URenderTextureFunctionLibrary::ReadPixelBuffer(UTextureRenderTarget2D* RenderTarget)
{
	return ReadPixelBufferRect(RenderTarget, 0, 0, 0, 0);
}

TArray<FColor> URenderTextureFunctionLibrary::ReadPixelBufferRect(UTextureRenderTarget2D* RenderTarget, int32 X0, int32 Y0, int32 X1, int32 Y1)
{
	if (RenderTarget != nullptr)
	{
		TArray<FColor> ColorBuffer;
		FTextureRenderTarget2DResource* TextureResource = StaticCast<FTextureRenderTarget2DResource*>(RenderTarget->Resource);
		if (TextureResource != nullptr)
		{
			TextureResource->ReadPixels(ColorBuffer, FReadSurfaceDataFlags(RCM_UNorm, CubeFace_MAX), FIntRect(X0, Y0, X1, Y1));
		}
	}

	return TArray<FColor>();
}


TArray<FLinearColor> URenderTextureFunctionLibrary::ReadLinearPixelBuffer(UTextureRenderTarget2D* RenderTarget)
{
	return ReadLinearPixelBufferRect(RenderTarget, 0, 0, 0, 0);
}

TArray<FLinearColor> URenderTextureFunctionLibrary::ReadLinearPixelBufferRect(UTextureRenderTarget2D* RenderTarget, int32 X0, int32 Y0, int32 X1, int32 Y1)
{
	if (RenderTarget != nullptr)
	{
		TArray<FLinearColor> ColorBuffer;
		FTextureRenderTarget2DResource* TextureResource = StaticCast<FTextureRenderTarget2DResource*>(RenderTarget->Resource);
		if (TextureResource != nullptr)
		{
			TextureResource->ReadLinearColorPixels(ColorBuffer, FReadSurfaceDataFlags(RCM_MinMax, CubeFace_MAX), FIntRect(X0, Y0, X1, Y1));
		}

		return ColorBuffer;
	}

	return TArray<FLinearColor>();
}

FColor URenderTextureFunctionLibrary::ReadPixelFromBuffer(TArray<FColor>& ColorBuffer, FVector2D UV, FIntPoint TargetSize)
{
	uint32 Index = GetNearestPixelIndex(TargetSize, UV);

	if (ColorBuffer.IsValidIndex(Index))
	{
		return ColorBuffer[Index];
	}

	return FColor::Black;
}

FLinearColor URenderTextureFunctionLibrary::ReadLinearPixelFromBuffer(TArray<FLinearColor>& ColorBuffer, FVector2D UV, FIntPoint TargetSize)
{
	uint32 Index = GetNearestPixelIndex(TargetSize, UV);

	if (ColorBuffer.IsValidIndex(Index))
	{
		return ColorBuffer[Index];
	}

	return FColor::Black;
}

FLinearColor URenderTextureFunctionLibrary::ReadLinearPixelFromBuffer_Bilinear(TArray<FLinearColor>& ColorBuffer, FIntPoint TargetSize, FVector2D UV)
{
	float u = UV.X * TargetSize.X - 0.5f;
	float v = UV.Y * TargetSize.Y - 0.5f;
	float x = FMath::FloorToFloat(u);
	float y = FMath::FloorToFloat(v);
	float u_ratio = u - x;
	float v_ratio = v - y;
	float u_opposite = 1.0f - u_ratio;
	float v_opposite = 1.0f - v_ratio;

	uint32 Index1 = x + y * TargetSize.X;
	uint32 Index2 = (x + 1) + y * TargetSize.X;
	uint32 Index3 = x + (y + 1) * TargetSize.X;
	uint32 Index4 = (x + 1) + (y + 1) * TargetSize.X;

	if (ColorBuffer.IsValidIndex(Index1) &&
		ColorBuffer.IsValidIndex(Index2) &&
		ColorBuffer.IsValidIndex(Index3) &&
		ColorBuffer.IsValidIndex(Index4))
	{

		FVector4 Result = (FVector4(ColorBuffer[Index1]) * u_opposite + FVector4(ColorBuffer[Index2]) * u_ratio) * v_opposite +
			(FVector4(ColorBuffer[Index3]) * u_opposite + FVector4(ColorBuffer[Index4]) * u_ratio) * v_ratio;

		return FLinearColor(Result.X, Result.Y, Result.Z, Result.W);
	}

	return FLinearColor::Black;
}

uint32 URenderTextureFunctionLibrary::GetNearestPixelIndex(FIntPoint TargetSize, FVector2D UV)
{
	int32 x = TargetSize.X * UV.X;
	int32 y = TargetSize.Y * UV.Y;

	if (x < 0)
	{
		x = 0;
	}
	else if (x >= TargetSize.X)
	{
		x = TargetSize.X - 1;
	}

	if (y < 0)
	{
		y = 0;
	}
	else if (y >= TargetSize.Y)
	{
		y = TargetSize.Y - 1;
	}

	uint32 Index = x + y * TargetSize.X;

	UE_LOG(LogTemp, Log, TEXT("Index: %d"), Index);

	return Index;
}