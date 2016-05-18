package org.peano.pdt.generators;

import org.peano.pdt.analysis.DepthFirstAdapter;
import org.peano.pdt.TranslationTable;
import org.peano.pdt.node.*;


public class DaStGenGenerator extends DepthFirstAdapter {
  private org.peano.pdt.generators.DirectoryGenerator _directoryGenerator;
  private org.peano.pdt.TranslationTable              _translationTable;
  private java.util.Vector<String>                    _templateDirectory;
  
  final private boolean                                _QuietDaStGen = true;

  public DaStGenGenerator(
    org.peano.pdt.generators.DirectoryGenerator directoryGenerator,
    org.peano.pdt.TranslationTable              translationTable,
    java.util.Vector<String>                    templateDirectory
  ) {
    _directoryGenerator = directoryGenerator;
    _translationTable   = translationTable;
    _templateDirectory  = templateDirectory;
  }
  

  private String[] getDaStGenArguments( String specificationFile, boolean isHeap ) {
	String includeDirectories = _directoryGenerator.getProjectDirectoryAbsolute();

	for (String add: _templateDirectory) {
      includeDirectories += ":";
      includeDirectories += add;
	}
	
	  // Doppelpunkt noch rein
    if (_QuietDaStGen) {
      String[] args = { 
        "--plugin", isHeap ? "PeanoHeapSnippetGenerator" : "PeanoSnippetGenerator", 
        "--naming", isHeap ? "PeanoHeapNameTranslator" : "PeanoNameTranslator", 
        "--include", includeDirectories, 
        "--quiet", "--pragmas", "--align", "--inline",
        specificationFile, 
        _directoryGenerator.getRecordsDirectory()
      };
            
      return args;
    }
    else {
      String[] args = { 
        "--plugin", isHeap ? "PeanoHeapSnippetGenerator" : "PeanoSnippetGenerator", 
        "--naming", isHeap ? "PeanoHeapNameTranslator" : "PeanoNameTranslator", 
        "--include", includeDirectories, 
        "--pragmas", "--align", "--inline",
        specificationFile, 
        _directoryGenerator.getRecordsDirectory()
      };

      return args;
    }
  }

  
  private void printArguments(String[] args) {
    System.err.print("used DaStGen arguments: ");
    for (String p : args) {
      System.err.print(p + " ");
    }
    System.err.println("");
  }

  
  public void inADescription(ADescription node) {
    String fileName                    = _directoryGenerator.getDaStGenDirectory()  
                                       + "/" 
                                       + TranslationTable.getRepositoryStateUnqualifiedName() 
                                       + ".def";
    String repositoryStateTemplateName = "DaStGenRepositoryState.template";

    _translationTable.convertTemplateFile(
      repositoryStateTemplateName,
      fileName, 
      true, true
    );

    printArguments(getDaStGenArguments(fileName,false));

    de.tum.in.dast.DaStGen.main( getDaStGenArguments(fileName,false) );
  }

  
  public void inAVertex(AVertex node) {
    String vertexFileName     = _directoryGenerator.getDaStGenDirectory()
                              + "/"
                              + TranslationTable.getVertexUnqualifiedName() 
                              + ".def";
    String vertexTemplateName = "DaStGenVertex.template";

    _translationTable.convertTemplateFile(
      vertexTemplateName,
      vertexFileName, 
      true, true
    );

    printArguments(getDaStGenArguments(vertexFileName,false));

    de.tum.in.dast.DaStGen.main( getDaStGenArguments(vertexFileName,false) );
  }

  
  public void inACell(ACell node) {
    String cellFileName     = _directoryGenerator.getDaStGenDirectory()
                            + "/"
                            + TranslationTable.getCellUnqualifiedName() 
                            + ".def";
    String cellTemplateName = "DaStGenCell.template";

    _translationTable.convertTemplateFile(
      cellTemplateName,
      cellFileName, 
      true, true
    );

    printArguments(getDaStGenArguments(cellFileName,false));

    de.tum.in.dast.DaStGen.main( getDaStGenArguments(cellFileName,false) );
  }


  public void inAState(AState node) {
    String stateFileName     = _directoryGenerator.getDaStGenDirectory()
                             + "/"
                             + TranslationTable.getStateUnqualifiedName() 
                             + ".def";
    String stateTemplateName = "DaStGenState.template";

    _translationTable.convertTemplateFile(
      stateTemplateName,
      stateFileName, 
      true, true
    );
    
    printArguments(getDaStGenArguments(stateFileName,false));

    de.tum.in.dast.DaStGen.main( getDaStGenArguments(stateFileName,false) );
  }     
  
  
  public void inAHeapDastgenFile(AHeapDastgenFile node) {
    String heapSpecification = _directoryGenerator.getProjectDirectoryAbsolute() + "/" + node.getFilename().getText();

    printArguments(getDaStGenArguments(heapSpecification,true));

    System.out.println( "process heap file " + heapSpecification );
    de.tum.in.dast.DaStGen.main( getDaStGenArguments(heapSpecification,true) );
  }
}
