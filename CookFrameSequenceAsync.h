// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OVRLipSyncContextWrapper.h"
#include "OVRLipSyncFrame.h"
#include "CookFrameSequenceAsync.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFrameSequenceCoocked, UOVRLipSyncFrameSequence*, FrameSequence, bool, Success);

/**
 * 
 */
UCLASS()
class OVRLIPSYNC_API UCookFrameSequenceAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintAssignable, Category = "LipSync")
        FFrameSequenceCoocked onFrameSequenceCooked;

    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "LipSync")
        static UCookFrameSequenceAsync* CookFrameSequence(const TArray<uint8>& RawSamples, bool UseOfflineModel = false);

    TArray<uint8> RawSamples;
    bool UseOfflineModel;

    virtual void Activate() override;
};
