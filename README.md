# UE4 OVR lipsync cook frame sequence

Async node to build ovr frame sequence during runtime.
Written to be used with [Google Speech Kit](https://github.com/IlgarLunin/UE4GoogleSpeechKit-docs) plugin.

![](pics/ovrframesequence.png)

Demo:

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/B78aQly2wrI/0.jpg)](https://www.youtube.com/watch?v=B78aQly2wrI)

# Raw samples content

Node expects to get uncompressed wave pcm data with 44 bytes header.

# Installation

1) Install oculus [ovr lipsync integration](https://developer.oculus.com/downloads/package/oculus-lipsync-unreal/) plugin to you project
2) Copy **CookFrameSequenceAsync.h** to `PROJECT_ROOT/Plugins/OVRLipSync/Source/OVRLipSync/Public`
3) Copy **CookFrameSequenceAsync.cpp** to `PROJECT_ROOT/Plugins/OVRLipSync/Source/OVRLipSync/Private`
4) Compile and run
