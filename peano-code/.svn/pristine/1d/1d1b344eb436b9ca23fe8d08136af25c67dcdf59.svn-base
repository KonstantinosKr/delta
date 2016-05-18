package org.peano.pdt.generators;

import org.peano.pdt.analysis.DepthFirstAdapter;

import org.peano.pdt.node.*;


public class RunnerGenerator extends DepthFirstAdapter {
  private org.peano.pdt.generators.DirectoryGenerator _directoryGenerator;
  private org.peano.pdt.TranslationTable              _translationTable;

  public RunnerGenerator(
    org.peano.pdt.generators.DirectoryGenerator directoryGenerator,
    org.peano.pdt.TranslationTable              translationTable
  ) {
    _directoryGenerator   = directoryGenerator;
    _translationTable     = translationTable;
  }
  
  public void inADescription(ADescription node) {
    _translationTable.convertTemplateFile( 
      "Main.template",
      _directoryGenerator.getProjectDirectoryAbsolute() + "/main.cpp",
      false, true
    );
    _translationTable.convertTemplateFile( 
      "RunnerHeader.template",
      _directoryGenerator.getRunnersDirectory() + "/Runner.h",
      false, true
    );
    _translationTable.convertTemplateFile( 
      "RunnerImplementation.template",
      _directoryGenerator.getRunnersDirectory() + "/Runner.cpp",
      false, true
    );
    _translationTable.convertTemplateFile( 
      "RunnerParallelWorkerImplementation.template",
      _directoryGenerator.getRunnersDirectory() + "/RunnerParallelWorker.cpp",
      false, true
    );
  }
}
