// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RenderTextureFunctionLibrary.generated.h"

class UTextureRenderTarget2D;

/**
 * 
 */
UCLASS()
class SEARADIO_API URenderTextureFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Utility")
	static TArray<FColor> ReadPixelBuffer(UTextureRenderTarget2D* Texture);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	static TArray<FLinearColor> ReadLinearPixelBuffer(UTextureRenderTarget2D* RenderTarget);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	static FColor ReadPixelFromBuffer(UPARAM(ref) TArray<FColor>& ColorBuffer, FVector2D UV, FIntPoint TargetSize);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	static FLinearColor ReadLinearPixelFromBuffer(UPARAM(ref) TArray<FLinearColor>& ColorBuffer, FVector2D UV, FIntPoint TargetSize);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	static FLinearColor ReadLinearPixelFromBuffer_Bilinear(UPARAM(ref) TArray<FLinearColor>& ColorBuffer, FIntPoint TargetSize, FVector2D UV);

	static uint32 GetNearestPixelIndex(FIntPoint TargetSize, FVector2D UV);
};
