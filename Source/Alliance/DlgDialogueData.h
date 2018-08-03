#pragma once

#include "CoreMinimal.h"
#include "DlgDialogueData.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct FDlgDialogueData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TMap<FName, int32> Integers;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TMap<FName, int32> Floats;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TMap<FName, FName> Names;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TSet<FName> TrueBools;
};