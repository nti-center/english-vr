// Fill out your copyright notice in the Description page of Project Settings.


#include "BubleTextWidgetClass.h"
#include "Components/TextBlock.h"
#include "Components/ScaleBox.h"
#include "Components/Image.h"

void UBubleTextWidgetClass::NativeConstruct() {
    Super::NativeConstruct();

    World = GetWorld();
    HideWidget();
}

void UBubleTextWidgetClass::HideWidget() {
    if (BubleText) {
        BubleText->SetText(FText::FromString(FString(TEXT(""))));
    }

    if (BubleImage) {
        BubleImage->SetOpacity(0);
    }
}

void UBubleTextWidgetClass::SeeBotAnswer(TArray<FString> InputArray, int32 ErrorIndex) {
    FString TmpString;
    int32 Counter = 0;

    for (auto& name : InputArray) { 
        if (!((ErrorIndex > 0) && ((Counter+1) == ErrorIndex))) {
            name.ReplaceInline(TEXT("_"), TEXT(" "));
            if(Counter != 0)
                TmpString += name.ToLower() + " ";
            else {
                TmpString += name + " ";
                //TmpString += ", ";
            }
        }
        else {
            TmpString += name.ToUpper() + " ";
        }
        if((Counter+1) == InputArray.Num() && InputArray.Num() > 2)
            TmpString += "?";
        Counter++;
    }

    BubleImage->SetOpacity(1);
    BubleText->SetText(FText::FromString(TmpString));

    World->GetTimerManager().SetTimer(FuzeTimerHandle, this, &UBubleTextWidgetClass::HideWidget, 5.0f);
}

FRotator UBubleTextWidgetClass::MyLookRotation(FVector lookAt, FVector upDirection, FVector Location) {
    FVector forward = lookAt - Location;
    FVector up = upDirection;


    forward = forward.GetSafeNormal();
    up = up - (forward * FVector::DotProduct(up, forward));
    up = up.GetSafeNormal();

    FVector vector = forward.GetSafeNormal();
    FVector vector2 = FVector::CrossProduct(up, vector);
    FVector vector3 = FVector::CrossProduct(vector, vector2);
    float m00 = vector.X;
    float m01 = vector.Y;
    float m02 = vector.Z;
    float m10 = vector2.X;
    float m11 = vector2.Y;
    float m12 = vector2.Z;
    float m20 = vector3.X;
    float m21 = vector3.Y;
    float m22 = vector3.Z;

    float num8 = (m00 + m11) + m22;
    FQuat quaternion = FQuat();

    if (num8 > 0.0f)
    {
        float num = (float)FMath::Sqrt(num8 + 1.0f);
        quaternion.W = num * 0.5f;
        num = 0.5f / num;
        quaternion.X = (m12 - m21) * num;
        quaternion.Y = (m20 - m02) * num;
        quaternion.Z = (m01 - m10) * num;
        return FRotator(quaternion);
    }

    if ((m00 >= m11) && (m00 >= m22))
    {
        float num7 = (float)FMath::Sqrt(((1.0f + m00) - m11) - m22);
        float num4 = 0.5f / num7;
        quaternion.X = 0.5f * num7;
        quaternion.Y = (m01 + m10) * num4;
        quaternion.Z = (m02 + m20) * num4;
        quaternion.W = (m12 - m21) * num4;
        return FRotator(quaternion);
    }

    if (m11 > m22)
    {
        float num6 = (float)FMath::Sqrt(((1.0f + m11) - m00) - m22);
        float num3 = 0.5f / num6;
        quaternion.X = (m10 + m01) * num3;
        quaternion.Y = 0.5f * num6;
        quaternion.Z = (m21 + m12) * num3;
        quaternion.W = (m20 - m02) * num3;
        return FRotator(quaternion);
    }

    float num5 = (float)FMath::Sqrt(((1.0f + m22) - m00) - m11);
    float num2 = 0.5f / num5;
    quaternion.X = (m20 + m02) * num2;
    quaternion.Y = (m21 + m12) * num2;
    quaternion.Z = 0.5f * num5;
    quaternion.W = (m01 - m10) * num2;

    return FRotator(quaternion);
}




