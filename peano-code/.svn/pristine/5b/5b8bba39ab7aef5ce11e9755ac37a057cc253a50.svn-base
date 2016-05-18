package org.peano.pdt.generators;

import org.peano.pdt.analysis.DepthFirstAdapter;

import org.peano.pdt.node.*;


public class AdapterGenerator extends DepthFirstAdapter {
  private org.peano.pdt.generators.DirectoryGenerator _directoryGenerator;
  private org.peano.pdt.TranslationTable              _translationTable;
  private java.util.Vector<String>                    _mappings;
  private java.util.Vector<String>                    _fullQualifiedMappings;
  private java.util.Vector<String>                    _mappingsPath;
  private String                                      _currentAdapterName;
  

  public AdapterGenerator(
    org.peano.pdt.generators.DirectoryGenerator directoryGenerator,
    org.peano.pdt.TranslationTable              translationTable
  ) {
    _directoryGenerator = directoryGenerator;
    _translationTable   = translationTable;
  }

  
  /**
   * 1. Parse the arguments and add the corresponding parameters to the parameter list.
   * 2. Create an adapter class using the template with the parameters specified.
   * 3. Add a new aggregate to the mappings connecting to the newly generated adapter.
   * 
   */
/*  public void inAPredefinedAdapter(APredefinedAdapter node) {
    String  predefinedAdapterType = node.getName().toString().trim();
    String  targetName            = _currentAdapterName + "2" + predefinedAdapterType + "_" + _predefinedMappings.size();
    _predefinedMappings.add( targetName );

    java.util.ArrayList<String> paramList = new java.util.ArrayList<String>();
    java.util.ArrayList<String> valueList = new java.util.ArrayList<String>();
    
    for (int i=0; i<node.getParameters().size(); i++) {
      paramList.add( "PARAM" + i );
      valueList.add( node.getParameters().get(i).toString().trim() );
    }

    _translationTable.setThisTypenameToAdapterTypename(targetName);
    
    _translationTable.convertTemplateFile( 
      predefinedAdapterType + "Header.template",
      _directoryGenerator.getAdaptersDirectory() + "/" + targetName + ".h",
      paramList,
      valueList,
      true,
      false
    );
    _translationTable.convertTemplateFile( 
      predefinedAdapterType + "Implementation.template",
      _directoryGenerator.getAdaptersDirectory() + "/" + targetName + ".cpp",
      paramList,
      valueList,
      true,
      false
    );
  }
*/

  public void inAAdapter(AAdapter node) {
    _mappings              = new java.util.Vector<String>();
    _fullQualifiedMappings = new java.util.Vector<String>();
    _mappingsPath          = new java.util.Vector<String>();

/*    for (int i=0; i<node.getUserDefined().size(); i++) {
      _mappings.add( node.getUserDefined().get(i).getText().trim() );
    }
*/    
    _currentAdapterName = node.getName().getText().trim();
  }

  
  public void inAPredefinedUseMapping(APredefinedUseMapping node)
  {
    String  adapterType = node.getName().toString().trim();
    String  adapterName = _currentAdapterName + "2" + adapterType + "_" + _mappings.size();

	_mappings.add( adapterName );
	_fullQualifiedMappings.add( "adapters::" + adapterName );
	_mappingsPath.add( "adapters/" + adapterName );

    java.util.ArrayList<String> paramList = new java.util.ArrayList<String>();
    java.util.ArrayList<String> valueList = new java.util.ArrayList<String>();
    
    for (int i=0; i<node.getParameters().size(); i++) {
      paramList.add( "PARAM" + i );
      valueList.add( node.getParameters().get(i).toString().trim() );
    }

    _translationTable.setThisTypenameToAdapterTypename(adapterName);
    
    _translationTable.convertTemplateFile( 
      adapterType + "Header.template",
      _directoryGenerator.getAdaptersDirectory() + "/" + adapterName + ".h",
      paramList,
      valueList,
      true,
      false
    );
    _translationTable.convertTemplateFile( 
      adapterType + "Implementation.template",
      _directoryGenerator.getAdaptersDirectory() + "/" + adapterName + ".cpp",
      paramList,
      valueList,
      true,
      false
    );
  }

  
  public void inAUserdefinedUseMapping(AUserdefinedUseMapping node)
  {
    String  adapterType = node.getName().toString().trim();

	_mappings.add( adapterType );
	_fullQualifiedMappings.add( "mappings::" + adapterType );
	_mappingsPath.add( "mappings/" + adapterType );
  }

  /**
   * Has to be in the out as I wanna get informed about all predefined adapters used
   * as well, i.e. we have to do this throughout the backtracking.
   */
  public void outAAdapter(AAdapter node) {
    _translationTable.setThisTypenameToAdapterTypename( _currentAdapterName, _mappings, _fullQualifiedMappings, _mappingsPath );

    _translationTable.convertTemplateFile( 
      "AdapterHeader.template",
      _directoryGenerator.getAdaptersDirectory() + "/" + _currentAdapterName + ".h",
      true, true
    );
    _translationTable.convertTemplateFile( 
      "AdapterImplementation.template",
      _directoryGenerator.getAdaptersDirectory() + "/" + _currentAdapterName + ".cpp",
      true, true
    );
  }
}
