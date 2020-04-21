#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "CrossfadeDataTableStruct.generated.h"

USTRUCT(BlueprintType)
struct FCrossfadeDataTableStruct : public FTableRowBase {
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FadeInStart;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FadeInEnd;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FadeOutStart;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FadeOutEnd;
	
};
