#include "tarch/logging/LogFilterFileReader.h"


#include <fstream>
#include <stdlib.h>
#include <string>


#include "tarch/logging/CommandLineLogger.h"


tarch::logging::Log  tarch::logging::LogFilterFileReader::_log( "tarch::logging::LogFilterFileReader" );


bool tarch::logging::LogFilterFileReader::interpretTokens( const std::string& levelToken, const std::string& classNameToken, const std::string& rankToken, const std::string& onOffToken ) {
  bool result = true;

  if ( levelToken!="debug" && levelToken!="info" ) {
    logError( "interpretTokens(...)", "expected \"debug\" or \"info\" but got " << levelToken );
    result = false;
  }

  int rank = rankToken=="-1" ? -1 : atoi( rankToken.c_str() );
  if ( rank<-1 ) {
    logError( "interpretTokens(...)", "expected positive rank or -1 for all ranks but got " << rankToken );
    result = false;
  }

  bool isFilter = onOffToken=="black";
  if (onOffToken!="black" && onOffToken!="white") {
    logError( "interpretTokens(...)", "expected \"black\" or \"white\" but got " << onOffToken );
    result = false;
  }

  if (result) {
    tarch::logging::CommandLineLogger::getInstance().addFilterListEntry(
      ::tarch::logging::CommandLineLogger::FilterListEntry(
        levelToken, rank, classNameToken, isFilter )
    );
  }

  return result;
}

bool tarch::logging::LogFilterFileReader::parseLine(std::ifstream& file, const std::string& filename, const std::string& line, int linenumber) {
  std::string lineWithNewline = line + '\n';

  //Loop through line
  int characterPosition = 0;
  const int NumberOfTokensPerLine = 4;
  std::string tokens[NumberOfTokensPerLine];
  int currentToken = 0;
  while (currentToken<NumberOfTokensPerLine && !file.eof() && characterPosition < static_cast<int>(lineWithNewline.length())) {
    char buffer = lineWithNewline[characterPosition];

    const bool bufferIsWhiteSpace =
      buffer == ' '  ||
      buffer == '\t' ||
      buffer == '\n';
    if (bufferIsWhiteSpace && !tokens[currentToken].empty()) {
      currentToken++;
    }
    if (!bufferIsWhiteSpace) {
      tokens[currentToken] += buffer;
    }

    characterPosition++;
  }

  if (currentToken != NumberOfTokensPerLine) {
    logError( "parsePlainTextFile(string)", "syntax error in input file " << filename << ", line " << linenumber << ":" << line );
    return false;
  }
  else {
    return interpretTokens(tokens[0],tokens[1],tokens[2],tokens[3]);
  }
}


bool tarch::logging::LogFilterFileReader::parsePlainTextFile( const std::string& filename ) {
  bool result = true;

  tarch::logging::CommandLineLogger::getInstance().clearFilterList();

  std::ifstream file;
  file.open(filename.c_str()); // open a file
  if (!file.good()) {
    logError( "parsePlainTextFile(string)", "was not able to open input file " << filename );
    result = false;
  }

  int linenumber = 1;
  std::string line;
  while (!file.eof() && result) {

    std::getline(file, line);
    if(line.length() > 0 && line[0]!='#') {
      result = parseLine(file, filename, line, linenumber);
    }

    linenumber++;
  }

  if (!result) {
    logWarning( "parsePlainTextFile(string)", "filter file " << filename << " was invalid. Switch on all log statements" );
    tarch::logging::CommandLineLogger::getInstance().addFilterListEntry( ::tarch::logging::CommandLineLogger::FilterListEntry( "info", false ) );
    tarch::logging::CommandLineLogger::getInstance().addFilterListEntry( ::tarch::logging::CommandLineLogger::FilterListEntry( "debug", false ) );
  }

  return result;
}

