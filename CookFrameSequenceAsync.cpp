// Fill out your copyright notice in the Description page of Project Settings.


#include "CookFrameSequenceAsync.h"
#include "Sound/SoundWave.h"
#include "Misc/ScopedSlowTask.h"
#include "Async.h"

constexpr auto LipSyncSequenceUpateFrequency = 100;
constexpr auto LipSyncSequenceDuration = 1.0f / LipSyncSequenceUpateFrequency;

UCookFrameSequenceAsync* UCookFrameSequenceAsync::CookFrameSequence(const TArray<uint8>& RawSamples, bool UseOfflineModel)
{
    UCookFrameSequenceAsync* BPNode = NewObject<UCookFrameSequenceAsync>();

    BPNode->RawSamples = RawSamples;
    BPNode->UseOfflineModel = UseOfflineModel;

    return BPNode;
}

void UCookFrameSequenceAsync::Activate()
{

    if (RawSamples.Num() <= 44)
    {
        onFrameSequenceCooked.Broadcast(nullptr, false);
        return;
    }

    FWaveModInfo waveInfo;
    uint8 *waveData = const_cast<uint8*>(RawSamples.GetData());

    if (waveInfo.ReadWaveInfo(waveData, RawSamples.Num()))
    {
        int32 NumChannels = *waveInfo.pChannels;
        int32 SampleRate = *waveInfo.pSamplesPerSec;
        auto PCMDataSize = waveInfo.SampleDataSize / sizeof(int16_t);
        int16_t* PCMData = reinterpret_cast<int16_t*>(waveData + 44);
        int32 ChunkSizeSamples = static_cast<int32>(SampleRate * LipSyncSequenceDuration);
        int32 ChunkSize = NumChannels * ChunkSizeSamples;

        FString modelPath = UseOfflineModel ? FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("OVRLipSync"),
            TEXT("OfflineModel"), TEXT("ovrlipsync_offline_model.pb"))
            : FString();

        Async<void>(EAsyncExecution::Thread, [=]()
        {
            UOVRLipSyncFrameSequence* Sequence = NewObject<UOVRLipSyncFrameSequence>();
            UOVRLipSyncContextWrapper context(ovrLipSyncContextProvider_Enhanced, SampleRate, modelPath);
            float LaughterScore = 0.0f;
            TArray<float> Visemes;
            for (int offs = 0; offs + ChunkSize < PCMDataSize; offs += ChunkSize)
            {
                context.ProcessFrame(PCMData + offs, ChunkSizeSamples, Visemes, LaughterScore, NumChannels > 1);
                Sequence->Add(Visemes, LaughterScore);
            }
            AsyncTask(ENamedThreads::GameThread, [Sequence, this]() {
                onFrameSequenceCooked.Broadcast(Sequence, true);
            });
            
        });

    }
    else
        onFrameSequenceCooked.Broadcast(nullptr, false);
}
