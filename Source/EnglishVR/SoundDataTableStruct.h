#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "SoundDataTableStruct.generated.h"

USTRUCT(BlueprintType)
struct FSoundDataTableStruct : public FTableRowBase {
    GENERATED_BODY()

    public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Path;
	
};
