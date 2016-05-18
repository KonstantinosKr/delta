// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org.
package org.peano.pdt;

import java.util.Arrays;
import java.util.Vector;

public class Main {
  public static void printUsage() {
    System.out.println("Peano Developement Toolkit (PDT)");
    System.out.println("(C) 2010-2015 Tobias Weinzierl");
    System.out.println("");
    System.out.println("There are two modes to use the toolkit. You can either invoke the toolkit with");
    System.out.println("");
    System.out.println("    java -jar pdt.jar --create-project project-name output-directory");
    System.out.println("");
    System.out.println("or you use the toolkit with");
    System.out.println("");
    System.out.println("    java -jar pdt.jar --generate-gluecode peano-specification-file output-directory [template-directories]" );
    System.out.println("");
    System.out.println("");
    System.out.println("For --create-project, you specify the project name you want to use (may contain :: for nested " );
    System.out.println("namespaces) and the directory where to write an empty project to. The directory is created if " );
    System.out.println("it had not existed before. No files are overwritten, i.e. ensure you clean up properly before" );
    System.out.println("you use the tool." );
    System.out.println("");
    System.out.println("For --generate-gluecode, the arguments have the following semantics:" );
    System.out.println("  peano-specification-file    Mandatory; specification file describing your project. " );
    System.out.println("  component-output-directory  Mandatory; typically the directory holding the spec file as well. All paths" );
    System.out.println("                              within the spec file are relative to the output directory." );
    System.out.println("  template-directories        Optional; points to the directories holding user-defined templates. Multiple directories are separated by colon" );
  }

  /**
   * @param args
   */
  public static void main(String[] args) {
    if (args.length < 3 || args.length > 4) {
      printUsage();
      
      System.err.println("" );
      System.err.println("Error: got " + args.length + " arguments" );
      
      return;
    };
    
   
    if (args[0].equals("--create-project") ) {
        System.out.println("Run mode: create project");
    	
        boolean success = (new java.io.File(args[2])).mkdirs();
        if (!success) {
          System.err.println("Warning: Was not able to create directories " + args[2]  + ". Probably directory has existed before");
        }
        
        System.out.print("- Generate vertex blueprint ...");
        convertTemplateFile( "BlueprintVertexDefinition.template", args[2] + "/Vertex.def", args[1] );
        System.out.println("done");

        System.out.print("- Generate cell blueprint ...");
        convertTemplateFile( "BlueprintCellDefinition.template",   args[2] + "/Cell.def",   args[1] );
        System.out.println("done");
        
        System.out.print("- Generate state blueprint ...");
        convertTemplateFile( "BlueprintStateDefinition.template",  args[2] + "/State.def",  args[1] );
        System.out.println("done");
        
        System.out.print("- Generate specification blueprint ...");
        convertTemplateFile( "BlueprintSpecification.template",    args[2] + "/project.peano-specification",  args[1] );
        System.out.println("done");

        System.out.println("- Project setup successful. Please rerun with options \"--generate-gluecode " + args[2] + "/project.peano-specification " + args[2] + " yourpath/usrtemplates\"");
    } 
    else if (args[0].equals("--generate-gluecode") ) {
      System.out.println("Run mode: generate gluecode");
      try {
        String inputFileName = args[1];
        String outputDirectoryName = args[2];
        String templateDirectoryName = args.length == 4 ? args[3] : null;

        java.io.File outputDirectory = new java.io.File(outputDirectoryName);
        if (!outputDirectory.isDirectory()) {
          System.err.println("output directory "
              + outputDirectory.getAbsolutePath() + " is no valid directory");
          System.exit(-1);
        }

        java.util.Vector<String> templateDirectory = new java.util.Vector<String>();
        if (templateDirectoryName != null) {
          System.out.println("parse template directory specification "
              + templateDirectoryName );
          templateDirectory = new Vector<String>();
          
          for ( String d: Arrays.asList(templateDirectoryName.split(":")) ) {
            java.io.File checkFile = new java.io.File(d);
            if (checkFile.isDirectory()) {
              System.out.println("template directory "
                  + checkFile.getAbsolutePath()
                  + " added to user-defined template search path");
              templateDirectory.add( checkFile.getAbsolutePath() );
            }
            else {
              System.err.println("template directory "
                  + checkFile.getAbsolutePath()
                  + " is no valid directory");
              System.exit(-1);
            }
          }
        }

        System.out.print("read input file " + inputFileName);
        org.peano.pdt.parser.Parser parser = new org.peano.pdt.parser.Parser(
            new org.peano.pdt.lexer.Lexer(new java.io.PushbackReader(
                new java.io.FileReader(inputFileName))));
        org.peano.pdt.node.Start document = parser.parse();
        System.out.println(" ... ok");

        System.out.print("fill translation table");
        TranslationTable translationTable = new TranslationTable(
            templateDirectory);
        document.apply(translationTable);
        System.out.println(" ... ok");

        System.out.print("generate output directories");
        org.peano.pdt.generators.DirectoryGenerator dirGenerator = new org.peano.pdt.generators.DirectoryGenerator(
            outputDirectory, translationTable);
        document.apply(dirGenerator);
        System.out.println(" ... ok");

        System.out.print("run DaStGen");
        document.apply(new org.peano.pdt.generators.DaStGenGenerator(
          dirGenerator, translationTable, templateDirectory
        ));
        System.out.println(" ... ok");
        
        System.out.print("generate blueprints for vertices, cells, states, repositories, and mappings");
        document.apply(new org.peano.pdt.generators.BluePrintGenerator(
          dirGenerator, translationTable
        ));
        System.out.println(" ... ok");
        
        System.out.print("generate repositories");
        document.apply(new org.peano.pdt.generators.RepositoryGenerator(
          dirGenerator, translationTable
        ));
        System.out.println(" ... ok");
        
         System.out.print("generate runners");
         document.apply(new org.peano.pdt.generators.RunnerGenerator(
           dirGenerator, translationTable
         ));
         System.out.println(" ... ok");
        
         System.out.print("generate adapters");
         document.apply(new org.peano.pdt.generators.AdapterGenerator(
           dirGenerator, translationTable
         ));
         System.out.println(" ... ok");
        
         System.out.print("generate makefiles");
         document.apply(new org.peano.pdt.generators.MakefileGenerator(
           dirGenerator, translationTable
         ));
         System.out.println(" ... ok");
        
         System.out.print("generate tests");
         document.apply(new org.peano.pdt.generators.TestCaseGenerator(
           dirGenerator, translationTable
         ));
         System.out.println(" ... ok");
      } catch (Exception e) {
        System.out.println(e.getClass() + " " + e.getMessage() + "\n");
        e.printStackTrace();
        System.out.println("Use option -h to display help message.");
      }
    }
    else {
      printUsage();

      System.err.println("" );
      System.err.println("Error: got run mode " + args[0] );
      return;
    }
  }


  static public void convertTemplateFile(
    String    templateFile, 
    String    destinationFile, 
    String    qualifiedProjectName
  ) {
  	destinationFile = destinationFile.replaceAll("\\\\", "/");
		
	if ((new java.io.File(destinationFile)).exists()) {
		System.out.print("\n  (blueprint of " + destinationFile
		+ " already exists)");
	} else {
		try {
			java.io.OutputStreamWriter writer = new java.io.OutputStreamWriter(
					new java.io.FileOutputStream(destinationFile));
			String text = new String();
		  	
			java.io.BufferedReader reader = null;
            reader = TranslationTable.getBufferedReaderForStdTemplate(templateFile);

			while ((text = reader.readLine()) != null) {
              String modifiedText = text.replaceAll("__PROJECT_NAME__", qualifiedProjectName);

              writer.write(modifiedText);
    		  writer.write("\n");
			}

			writer.close();
			reader.close();
		} catch (Exception e) {
			System.out.println(e.getClass() + " " + e.getMessage() + "\n");
       		e.printStackTrace();
		}
	}
  }
}
