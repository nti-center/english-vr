#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "AudioDataTableStruct.generated.h"

USTRUCT(BlueprintType)
struct FAudioDataTableStruct : public FTableRowBase
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Path;

};