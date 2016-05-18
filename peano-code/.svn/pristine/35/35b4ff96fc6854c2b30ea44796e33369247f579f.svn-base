// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org.
package org.peano.pdt.generators;

import java.io.File;

import org.peano.pdt.analysis.DepthFirstAdapter;
import org.peano.pdt.TranslationTable;

import org.peano.pdt.node.Start;

public class DirectoryGenerator extends DepthFirstAdapter {
  private File                _outputDirectory;
  private TranslationTable    _translationTable;

  public DirectoryGenerator(File outputDirectory, TranslationTable translationTable) {
    _outputDirectory  = outputDirectory;
    _translationTable = translationTable;
  }
  
//  static public String replaceColonsByFileSeparator( String originalString ) {
//    String fileSeparator   = File.separator;
//    if (fileSeparator.length()==1 && fileSeparator.charAt(0)=='\\') fileSeparator = "\\\\";
//    return originalString.replaceAll("::", fileSeparator);
//  }
  
  public void outStart(Start node) {
    java.io.File dirRecords = new java.io.File( getRecordsDirectory() );
    if (dirRecords.mkdirs()) {
      System.out.print(dirRecords.getAbsolutePath() + " ");
    }
    _translationTable.copyTemplateFile( "Readme.template", dirRecords.getAbsolutePath() + "/readme.txt", true );

    java.io.File dirMappings = new java.io.File( getMappingsDirectory() );
    if (dirMappings.mkdirs()) {
      System.out.print(dirMappings.getAbsolutePath() + " ");
    }
    
    java.io.File dirRepository = new java.io.File( getRepositoriesDirectory() );
    if (dirRepository.mkdirs()) {
      System.out.print(dirRepository.getAbsolutePath() + " ");
    }
    _translationTable.copyTemplateFile( "Readme.template", dirRepository.getAbsolutePath() + "/readme.txt", true );
    
    java.io.File dirAdapter = new java.io.File( getAdaptersDirectory() );
    if (dirAdapter.mkdirs()) {
      System.out.print(dirAdapter.getAbsolutePath() + " ");
    }
    _translationTable.copyTemplateFile( "Readme.template", dirAdapter.getAbsolutePath() + "/readme.txt", true );
    
    java.io.File dirRunner = new java.io.File( getRunnersDirectory() );
    if (dirRunner.mkdirs()) {
      System.out.print(dirRunner.getAbsolutePath() + " ");
    }
    
    java.io.File dirTests = new java.io.File( getTestsDirectory() );
    if (dirTests.mkdirs()) {
      System.out.print(dirTests.getAbsolutePath() + " ");
    }
    
    java.io.File dirDastgen = new java.io.File( getDaStGenDirectory() );
    if (dirDastgen.mkdirs()) {
      System.out.print(dirDastgen.getAbsolutePath() + " ");
    }
    _translationTable.copyTemplateFile( "Readme.template", dirDastgen.getAbsolutePath() + "/readme.txt", true );
  }
  
  public String getProjectDirectoryAbsolute() {
    return _outputDirectory.getAbsolutePath();
  }
  
  public String getRecordsDirectoryRelativeToComponentDirectory() {
    return "records";
  }
  
  public String getRecordsDirectory() {
    return getProjectDirectoryAbsolute() + File.separator + getRecordsDirectoryRelativeToComponentDirectory();
  }
  
  public String getMappingsDirectoryRelativeToComponentDirectory() {
    return "mappings";
  }

  public String getMappingsDirectory() {
    return getProjectDirectoryAbsolute() + File.separator + getMappingsDirectoryRelativeToComponentDirectory();
  }
  
  public String getRepositoriesDirectoryRelativeToComponentDirectory() {
    return "repositories";
  }

  public String getRepositoriesDirectory() {
    return getProjectDirectoryAbsolute() + File.separator + getRepositoriesDirectoryRelativeToComponentDirectory();
  }

  public String getAdaptersDirectoryRelativeToComponentDirectory() {
    return "adapters";
  }

  public String getAdaptersDirectory() {
    return getProjectDirectoryAbsolute() + File.separator + getAdaptersDirectoryRelativeToComponentDirectory();
  }

  public String getRunnersDirectoryRelativeToComponentDirectory() {
    return "runners";
  }

  public String getRunnersDirectory() {
    return getProjectDirectoryAbsolute() + File.separator + getRunnersDirectoryRelativeToComponentDirectory();
  }

  public String getTestsDirectoryRelativeToComponentDirectory() {
    return "tests";
  }

  public String getTestsDirectory() {
    return getProjectDirectoryAbsolute() + File.separator + getTestsDirectoryRelativeToComponentDirectory();
  }

  public String getDaStGenDirectoryRelativeToComponentDirectory() {
    return "dastgen";
  }

  public String getDaStGenDirectory() {
    return getProjectDirectoryAbsolute() + File.separator + getDaStGenDirectoryRelativeToComponentDirectory();
  }
}
