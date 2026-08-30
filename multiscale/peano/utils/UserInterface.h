// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_UTILS_USERINTERFACE_H_
#define _PEANO_UTILS_USERINTERFACE_H_

#include "peano/utils/Globals.h"
#include "tarch/logging/Log.h"
#include "tarch/la/Vector.h"


namespace peano {
  namespace utils {
    class UserInterface;
  }
}


/**
 * This class represents the user interface for detailed command line output.
 *
 * \author Tobias Weinzierl, Tobias Neckel
 */
class peano::utils::UserInterface {
  protected:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    /**
     * This method prints the datatype statistics (size of datatypes, numerical
     * epsilon, etc.).
     */
    static void writeDatatypeStatisticsHeader();

    /**
     * This method currently does not print any information.
     */
    static void writeDatatypeStatisticsTrailer();

    /**
     * Formats a text. SpacesPerColumn should equal the floats' size. Result
     * is centered.
     */
    static std::string format( int SpacesPerColumn, const std::string& text );

    /**
     * Formats a number. Result is aligned right.
     */
    static std::string format( int SpacesPerColumn, int number );

    /**
     * Formats a number. Result is aligned right.
     */
    static std::string format( int SpacesPerColumn, double number );

    /**
     * Method for getting memory usage of peano-process at current state in byte.
     *
     * Is used by reportMemoryUsageMB().
     *
     * The implementation is based on C-version of Michael Moltenbrey. Please
     * note that the amount of storage corresponds to the VIRT (visible in top,
     * e.g.). VIRT = SWAP + RES (whatever RES really is).
     *
     * @return memory usage (in bytes?) contained in /proc/<peano-process-ID>/stat.
     */
    static std::size_t getMemoryUsage();

    static const std::string _PeanoHeader;
    static const std::string _PeanoCopyrightURL;

    /**
     * Defines the width of a column.
     *
     * If you want to plot a double into this column, you have to set a
     * precision equaling the constant -6, as there's the e+00 added to the
     * floating point expression, there's this additional dot and the precision
     * defines the number of digits after the dot.
     */
    static const int _WidthOfSolverIterationColumn;
    static const int _WidthOfGridIterationColumn;
    static const int _WidthOfIntegerColumn;

    /**
     * Standard constructor.
     */
    UserInterface();

  public:
    /**
     * Standard destructor.
     */
    ~UserInterface();

    /**
     * Write the static program header to be plotted at startup.
     */
    static void writeHeader(const std::string& experimentName = "");

    /**
     * Write some information about the amount of data / datatypes used.
     */
    template <class Vertex, class Cell, class State>
    static void writeDatatypeStatistics();

    /**
     * Method for getting the application's memory footprint
     *
     * We use the exact version of KiloByte with 1024 bytes.
     *
     * The implementation is based on C-version of Michael Moltenbrey.
     *
     * !!! Rationale
     *
     * The operation should not log directly to the output device (info), as we
     * might want to postprocess the result. For example: Stop further
     * refinement if the memory footprint exceeds a given threshold.
     *
     *
     * \see getMemoryUsage()
     * \see reportMemoryUsageMB()
     */
    static int getMemoryUsageKB();

    /**
     * Method for getting the application's memory footprint
     *
     * We use the exact version of MegaByte with 1024*1024 bytes.
     *
     * The implementation is based on C-version of Michael Moltenbrey.
     *
     * !!! Rationale
     *
     * The operation should not log directly to the output device (info), as we
     * might want to postprocess the result. For example: Stop further
     * refinement if the memory footprint exceeds a given threshold.
     *
     * \see getMemoryUsage()
     * \see reportMemoryUsageKB()
     */
    static int getMemoryUsageMB();

    static void reportRegisteredServices();
};

#include "peano/utils/UserInterface.cpph"

#endif
