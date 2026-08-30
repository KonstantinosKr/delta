// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_LOGGING_LOG_FILTER_FILE_READER_H_
#define _TARCH_LOGGING_LOG_FILTER_FILE_READER_H_


#include <string>


#include "tarch/logging/Log.h"


namespace tarch {
  namespace logging {
    class LogFilterFileReader;
  }
}


/**
 * Read log filter spec file
 *
 * Very simple utility class offering a couple of static operations to parse
 * different filter files for the command line logger. Each line specifies
 * one filter list entry. Comment lines have to start with #. Here's an example
 * for a valid configuration file:
 * \code

\code

# Level (info or debug)   Trace   Rank (-1 means all ranks)   Black or white list entry

#
# General debug settings
#
debug  tarch                               -1     black
debug  peano                               -1     black

#
# General info settings
#
info  tarch                               -1     white
info  peano                               -1     white

info   my::full::qualified::class::name    -1     black
debug  my::full::qualified::class::name    43     white

\endcode


\endcode
 *
 * @author Tobias Weinzierl, Kristof Unterweger
 */
class tarch::logging::LogFilterFileReader {
  private:
    static tarch::logging::Log  _log;

    /**
     * @return Parsing has been successful
     */
    static bool interpretTokens( const std::string& levelToken, const std::string& classNameToken, const std::string& rankToken, const std::string& onOffToken );

    /**
     * @return Parsing has been successful
     */
    static bool parseLine(std::ifstream& file, const std::string& filename, const std::string& line, int linenumber);
  public:
    /**
     * Tries to parse a simple text file where each line contains an entry
     *
     * If the file does not exist or the parse did not succeed, the operation
     * writes an error and returns false. Otherwise, it returns true.
     *
     * If the routine fails, it switches on all log information automatically.
     * If you want to create a (hard-coded) fall-back logging, you thus might
     * have to call clearFilterList() again before you continue.
     *
     * @return Parsing has been successful
     */
    static bool parsePlainTextFile( const std::string& filename );
};

#endif
