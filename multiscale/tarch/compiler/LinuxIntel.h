// Default variant of CompilerSpecificSettings.h for IBM's XLC compiler.
// For a detailed description of the semantics of the settings, please
// consult LinuxIntel.h which is the most elaborate documentation.


#define UseTestSpecificCompilerSettings


#ifndef noCompilerICC
  #define CompilerICC
#endif

#define CompilerHasProcStat

#define CompilerHasUTSName

#define CompilerHasTimespec

#define CompilerHasSysinfo

//#define CompilerDefinesMPIMaxNameString


//#define DaStGenPackedPadding 1      // 32 bit version
// #define DaStGenPackedPadding 2   // 64 bit version

//#define SpecialiseVectorTemplatesForIntegers


#if !defined(noMultipleThreadsMayTriggerMPICalls) && !defined(MultipleThreadsMayTriggerMPICalls)
#define MultipleThreadsMayTriggerMPICalls
#endif


#if !defined(UseManualInlining) &&  !defined(noUseManualInlining)
#define UseManualInlining
#endif

#define LittleEndian


#if !defined(UseTBBsParallelForAndReduce) &&  !defined(noUseTBBsParallelForAndReduce)
#define UseTBBsParallelForAndReduce
#endif


#if !defined(IprobeEveryKIterations)
#define IprobeEveryKIterations 1024
#endif


