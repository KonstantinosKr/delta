// Default variant of CompilerSpecificSettings.h for the Intel compiler running on Windows.
// For a detailed description of the semantics of the settings, please
// consult LinuxIntel.h which is the most elaborate documentation.


/**
 * The Intel compiler based upon Microsoft's Visual C++ does not support the
 * __align__ keyword. We thus disable this feature though you might try to
 * set it in PeanoOptimisations.h
 */
#define noManualAlignment
#define noManualInlining


#define UseTestSpecificCompilerSettings
//#define CompilerCLX
#define CompilerICC
//#define CompilerHasProcStat
//#define CompilerHasUTSName
#define CompilerHasTimespec
//#define CompilerDefinesMPIMaxNameString
//#define DaStGenPackedPadding 1      // 32 bit version
// #define DaStGenPackedPadding 2   // 64 bit version
//#define SpecialiseVectorTemplatesForIntegers

#ifndef noMultipleThreadsMayTriggerMPICalls
#define MultipleThreadsMayTriggerMPICalls
#endif

#define SendWorkerMasterMessagesBlocking     0
#define SendMasterWorkerMessagesBlocking     0
#define ReceiveMasterMessagesBlocking        0
#define SendAndReceiveLoadBalancingMessagesBlocking    0
#define ReceiveIterationControlMessagesBlocking        0
#define BroadcastToIdleNodesBlocking                   0
#define BroadcastToWorkingNodesBlocking                0
#define SendHeapMetaDataBlocking                       0
#define SendAndReceiveHeapSynchronousDataBlocking      0


#ifndef noManualInlining
#define UseManualInlining
#endif


#define LittleEndian
