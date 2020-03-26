
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "FruitSoundDataTableStruct.generated.h"


USTRUCT(BlueprintType)
struct FFruitSoundDataTableStruct : public FTableRowBase {

GENERATED_BODY()

public:
UPROPERTY(EditAnywhere, BlueprintReadWrite)
FString Path;

UPROPERTY(EditAnywhere, BlueprintReadWrite)
FString FruitType;
	
};
