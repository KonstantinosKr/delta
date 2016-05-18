// Default variant of CompilerSpecificSettings.h for Linux with Intel compiler
// if we translate for the MIC architecture.
//
// Basically, this architecture setup equals the standard Linux Intel setup.
// However, the MIC tends to crash if we switch packing on. This is due to the
// particular memory alignment required no the Xeon Phi.

#include "tarch/compiler/LinuxIntel.h"

#define noPackedRecords
