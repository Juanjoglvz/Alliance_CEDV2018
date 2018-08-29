/* Copyright (C) 2018 Iván García, Juan José Corroto and Javier Córdoba - All Rights Reserved
* You may use, distribute and modify this code under the
* terms of the GNU GPLv3 license.
*
* You should have received a copy of the GNU GPLv3 license with
* this file. If not, please write to: ivan.garcia16@alu.uclm.es
*/

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