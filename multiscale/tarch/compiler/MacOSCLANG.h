// Default variant of CompilerSpecificSettings.h for GCC on MacOS.
// For a detailed description of the semantics of the settings, please
// consult LinuxIntel.h which is the most elaborate documentation.

/**
 * Switch off Optimisation
 *
 * Some compilers (icc 10.x, e.g.) run into problems compiling the test cases
 * as they run out of memory. In this case, one can use these two defined
 * within the implementation files. Unfortunately, the corresponding pragmas
 * are not supported by all compilers (gcc 4.x.x, e.g.). Therefore, I
 * encapsulated them within my own defines.
 *
 * To make define work, see the documentation of the test super class.
 */
//#define UseTestSpecificCompilerSettings

#define CompilerCLANG
//#define CompilerICC
//#define CompilerHasProcStat
#define CompilerHasUTSName
#define CompilerHasTimespec
//#define CompilerHasSysinfo
//#define CompilerDefinesMPIMaxNameString
//#define DaStGenPackedPadding 1      // 32 bit version
// #define DaStGenPackedPadding 2   // 64 bit version


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


