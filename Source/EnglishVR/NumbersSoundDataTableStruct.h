
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "NumbersSoundDataTableStruct.generated.h"


USTRUCT(BlueprintType)
struct FNumbersSoundDataTableStruct : public FTableRowBase {
    GENERATED_BODY()

public:
UPROPERTY(EditAnywhere, BlueprintReadWrite)
FString Path;

UPROPERTY(EditAnywhere, BlueprintReadWrite)
int32 Count;
	
};
