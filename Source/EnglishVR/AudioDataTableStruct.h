#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "AudioDataTableStruct.generated.h"

USTRUCT(BlueprintType)
struct FAudioDataTableStruct : public FTableRowBase {
    GENERATED_BODY()

    public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FruitType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString VegetablesOrFruits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Path;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Cost;
    
};