package org.peano.pdt.generators;

import org.peano.pdt.analysis.DepthFirstAdapter;

import org.peano.pdt.node.*;

public class RepositoryGenerator extends DepthFirstAdapter {
  private org.peano.pdt.generators.DirectoryGenerator _directoryGenerator;
  private org.peano.pdt.TranslationTable              _translationTable;

  public RepositoryGenerator(
    org.peano.pdt.generators.DirectoryGenerator directoryGenerator,
    org.peano.pdt.TranslationTable              translationTable
  ) {
    _directoryGenerator  = directoryGenerator;
    _translationTable    = translationTable;
  }

  public void inADescription(ADescription node) {
    _translationTable.convertTemplateFile( 
      "RepositoryFactoryHeader.template",
      _directoryGenerator.getRepositoriesDirectory() + "/RepositoryFactory.h",
      true, true
    );
    _translationTable.convertTemplateFile( 
      "RepositoryFactoryImplementation.template",
      _directoryGenerator.getRepositoriesDirectory() + "/RepositoryFactory.cpp",
      true, true
    );

    _translationTable.convertTemplateFile( 
      "RepositoryHeader.template",
      _directoryGenerator.getRepositoriesDirectory() + "/Repository.h",
      true, true
    );

    _translationTable.convertTemplateFile( 
      "RepositoryArrayStackHeader.template",
      _directoryGenerator.getRepositoriesDirectory() + "/RepositoryArrayStack.h",
      true, true
    );
    _translationTable.convertTemplateFile( 
      "RepositoryArrayStackImplementation.template",
      _directoryGenerator.getRepositoriesDirectory() + "/RepositoryArrayStack.cpp",
      true, true
    );

    _translationTable.convertTemplateFile( 
      "RepositorySTDStackHeader.template",
      _directoryGenerator.getRepositoriesDirectory() + "/RepositorySTDStack.h",
      true, true
    );
    _translationTable.convertTemplateFile( 
      "RepositorySTDStackImplementation.template",
      _directoryGenerator.getRepositoriesDirectory() + "/RepositorySTDStack.cpp",
      true, true
    );
  }
  
  
  public void inAAdapter(AAdapter node) {
    String  predefinedAdapterType = node.getName().toString().trim();
    String  targetName            = "RepositoryExplicitGridTemplateInstantiation4" + predefinedAdapterType;
    
    _translationTable.setThisTypenameToAdapterTypename(predefinedAdapterType);
    _translationTable.convertTemplateFile( 
      "RepositoryExplicitGridTemplateInstantiationImplementation.template",
      _directoryGenerator.getRepositoriesDirectory() + "/" + targetName + ".cpp",
      true, true
    );
  }
}
