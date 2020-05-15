#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "CharacterDataTableStruct.generated.h"

USTRUCT(BlueprintType)
struct FCharacterDataTableStruct : public FTableRowBase {
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PathToBP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PathToMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PathToTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PathToCrossfadeTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PathToSound;
	
};
