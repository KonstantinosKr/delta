package org.peano.pdt;

import java.io.File;
import java.io.FileNotFoundException;

import org.peano.pdt.analysis.DepthFirstAdapter;
import org.peano.pdt.node.*;

public class TranslationTable extends DepthFirstAdapter {
	private static final String MULTIPLE_REPLACEMENT_LINE_PREFIX = "__x";
	private static final String MULTIPLE_REPLACEMENT_LINE_PREFIX_WITH_COMMA = "__xc";

	private static final String _KEYWORD_COMPONENT_NAME = "__COMPONENT_NAME__";

	private static final String _KEYWORD_INCLUDE_GUARD_PREFIX = "__INCLUDE_GUARD_PREFIX__";

	private static final String _KEYWORD_DOF_DASTGEN_DEFINITION_CELL = "__DOF_DASTGEN_DEFINITION_CELL__";
	private static final String _KEYWORD_DOF_DASTGEN_DEFINITION_STATE = "__DOF_DASTGEN_DEFINITION_STATE__";
	private static final String _KEYWORD_DOF_DASTGEN_DEFINITION_VERTEX = "__DOF_DASTGEN_DEFINITION_VERTEX__";

	private static final String _KEYWORD_NAMESPACE = "__NAMESPACE__";
	private static final String _KEYWORD_OPEN_NAMESPACE = "__OPEN_NAMESPACE__";
	private static final String _KEYWORD_CLOSE_NAMESPACE = "__CLOSE_NAMESPACE__";

	private static final String _KEYWORD_PROJECT_PATH = "__PROJECT_PATH__";

	private static final String _KEYWORD_REPOSITORY_ACTION_ENUM = "__REPOSITORY_ACTION_ENUM__";

	private static final String _MULTIPLE_LINE_KEYWORD_FULL_QUALIFIED_ADAPTER_TYPE = "__FULL_QUALIFIED_ADAPTER_TYPE__";
	private static final String _MULTIPLE_LINE_KEYWORD_NONQUALIFIED_ADAPTER_TYPE = "__NONQUALIFIED_ADAPTER_TYPE__";

	private static final String _MULTIPLE_LINE_KEYWORD_READ_SCALAR_OPERATION_DOUBLE = "__READ_SCALAR_OPERATION_DOUBLE__";
	private static final String _MULTIPLE_LINE_KEYWORD_READ_VECTOR_OPERATION_DOUBLE = "__READ_VECTOR_OPERATION_DOUBLE__";
	private static final String _MULTIPLE_LINE_KEYWORD_READ_VECTOR_TWO_POW_D_OPERATION_DOUBLE = "__READ_VECTOR_TWO_POW_D_OPERATION_DOUBLE__";
	private static final String _MULTIPLE_LINE_KEYWORD_READ_STENCIL_OPERATION_DOUBLE = "__READ_STENCIL_OPERATION_DOUBLE__";
	private static final String _MULTIPLE_LINE_KEYWORD_READ_INTERGRID_OPERATOR_OPERATION_DOUBLE = "__READ_INTERGRID_OPERATOR_OPERATION_DOUBLE__";
	private static final String _MULTIPLE_LINE_KEYWORD_WRITE_SCALAR_OPERATION_DOUBLE = "__WRITE_SCALAR_OPERATION_DOUBLE__";
	private static final String _MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_OPERATION_DOUBLE = "__WRITE_VECTOR_OPERATION_DOUBLE__";
	private static final String _MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_TWO_POW_D_OPERATION_DOUBLE = "__WRITE_VECTOR_TWO_POW_D_OPERATION_DOUBLE__";
	private static final String _MULTIPLE_LINE_KEYWORD_WRITE_STENCIL_OPERATION_DOUBLE = "__WRITE_STENCIL_OPERATION_DOUBLE__";
	private static final String _MULTIPLE_LINE_KEYWORD_WRITE_INTERGRID_OPERATOR_OPERATION_DOUBLE = "__WRITE_INTERGRID_OPERATOR_OPERATION_DOUBLE__";

	private static final String _MULTIPLE_LINE_KEYWORD_READ_SCALAR_OPERATION_INT = "__READ_SCALAR_OPERATION_INT__";
	private static final String _MULTIPLE_LINE_KEYWORD_READ_VECTOR_OPERATION_INT = "__READ_VECTOR_OPERATION_INT__";
	private static final String _MULTIPLE_LINE_KEYWORD_READ_VECTOR_TWO_POW_D_OPERATION_INT = "__READ_VECTOR_TWO_POW_D_OPERATION_INT__";
	private static final String _MULTIPLE_LINE_KEYWORD_READ_STENCIL_OPERATION_INT = "__READ_STENCIL_OPERATION_INT__";
	private static final String _MULTIPLE_LINE_KEYWORD_READ_INTERGRID_OPERATOR_OPERATION_INT = "__READ_INTERGRID_OPERATOR_OPERATION_INT__";
	private static final String _MULTIPLE_LINE_KEYWORD_WRITE_SCALAR_OPERATION_INT = "__WRITE_SCALAR_OPERATION_INT__";
	private static final String _MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_OPERATION_INT = "__WRITE_VECTOR_OPERATION_INT__";
	private static final String _MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_TWO_POW_D_OPERATION_INT = "__WRITE_VECTOR_TWO_POW_D_OPERATION_INT__";
	private static final String _MULTIPLE_LINE_KEYWORD_WRITE_STENCIL_OPERATION_INT = "__WRITE_STENCIL_OPERATION_INT__";
	private static final String _MULTIPLE_LINE_KEYWORD_WRITE_INTERGRID_OPERATOR_OPERATION_INT = "__WRITE_INTERGRID_OPERATOR_OPERATION_INT__";

	private static final String _MULTIPLE_LINE_KEYWORD_READ_SCALAR_OPERATION_COMPLEX = "__READ_SCALAR_OPERATION_COMPLEX__";
	private static final String _MULTIPLE_LINE_KEYWORD_READ_VECTOR_OPERATION_COMPLEX = "__READ_VECTOR_OPERATION_COMPLEX__";
	private static final String _MULTIPLE_LINE_KEYWORD_READ_VECTOR_TWO_POW_D_OPERATION_COMPLEX = "__READ_VECTOR_TWO_POW_D_OPERATION_COMPLEX__";
	private static final String _MULTIPLE_LINE_KEYWORD_READ_STENCIL_OPERATION_COMPLEX = "__READ_STENCIL_OPERATION_COMPLEX__";
	private static final String _MULTIPLE_LINE_KEYWORD_READ_INTERGRID_OPERATOR_OPERATION_COMPLEX = "__READ_INTERGRID_OPERATOR_OPERATION_COMPLEX__";
	private static final String _MULTIPLE_LINE_KEYWORD_WRITE_SCALAR_OPERATION_COMPLEX = "__WRITE_SCALAR_OPERATION_COMPLEX__";
	private static final String _MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_OPERATION_COMPLEX = "__WRITE_VECTOR_OPERATION_COMPLEX__";
	private static final String _MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_TWO_POW_D_OPERATION_COMPLEX = "__WRITE_VECTOR_TWO_POW_D_OPERATION_COMPLEX__";
	private static final String _MULTIPLE_LINE_KEYWORD_WRITE_STENCIL_OPERATION_COMPLEX = "__WRITE_STENCIL_OPERATION_COMPLEX__";
	private static final String _MULTIPLE_LINE_KEYWORD_WRITE_INTERGRID_OPERATOR_OPERATION_COMPLEX = "__WRITE_INTERGRID_OPERATOR_OPERATION_COMPLEX__";

	private static final String _MULTIPLE_LINE_KEYWORD_MAPPINGS = "__MAPPINGS__";
	private static final String _MULTIPLE_LINE_KEYWORD_FULL_QUALIFIED_MAPPINGS = "__FULL_QUALIFIED_MAPPINGS__";
	private static final String _MULTIPLE_LINE_KEYWORD_MAPPINGS_PATH = "__MAPPINGS_PATH__";

	private static final String _TEMPORARY_KEYWORD_TYPENAME = "__THIS_TYPENAME__";
	private static final String _TEMPORARY_KEYWORD_FULL_QUALIFIED_TYPENAME = "__THIS_QUALIFIED_TYPENAME__";

	private static final String _KEYWORD_NUMBER_OF_ADAPTERS_ = "__NUMBER_OF_ADAPTERS__";

	private java.util.Vector<String> _templateDirectory;

	/**
	 * All the mappings from keywords to concrete names.
	 */
	private java.util.HashMap<String, String> _mapping;

	/**
	 * All the mappings from keywords to several names.
	 */
	private java.util.HashMap<String, java.util.Vector<String>> _multiMappings;

	/**
	 * List of all the adapters associated to the project.
	 */
	private java.util.Vector<String> _adapters;

	public TranslationTable(java.util.Vector<String> templateDirectory) {
		_mapping = new java.util.HashMap<String, String>();
		_templateDirectory = templateDirectory;
		for (String p : _mapping.keySet()) {
			_mapping.put(p, "UNDEF");
		}
		_adapters = new java.util.Vector<String>();
		_multiMappings = new java.util.HashMap<String, java.util.Vector<String>>();

		_multiMappings.put(_MULTIPLE_LINE_KEYWORD_READ_SCALAR_OPERATION_DOUBLE,
				new java.util.Vector<String>());
		_multiMappings.put(_MULTIPLE_LINE_KEYWORD_READ_VECTOR_OPERATION_DOUBLE,
				new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_READ_VECTOR_TWO_POW_D_OPERATION_DOUBLE,
				new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_READ_STENCIL_OPERATION_DOUBLE,
				new java.util.Vector<String>());
		_multiMappings
				.put(_MULTIPLE_LINE_KEYWORD_READ_INTERGRID_OPERATOR_OPERATION_DOUBLE,
						new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_WRITE_SCALAR_OPERATION_DOUBLE,
				new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_OPERATION_DOUBLE,
				new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_TWO_POW_D_OPERATION_DOUBLE,
				new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_WRITE_STENCIL_OPERATION_DOUBLE,
				new java.util.Vector<String>());
		_multiMappings
				.put(_MULTIPLE_LINE_KEYWORD_WRITE_INTERGRID_OPERATOR_OPERATION_DOUBLE,
						new java.util.Vector<String>());

		_multiMappings.put(_MULTIPLE_LINE_KEYWORD_READ_SCALAR_OPERATION_INT,
				new java.util.Vector<String>());
		_multiMappings.put(_MULTIPLE_LINE_KEYWORD_READ_VECTOR_OPERATION_INT,
				new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_READ_VECTOR_TWO_POW_D_OPERATION_INT,
				new java.util.Vector<String>());
		_multiMappings.put(_MULTIPLE_LINE_KEYWORD_READ_STENCIL_OPERATION_INT,
				new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_READ_INTERGRID_OPERATOR_OPERATION_INT,
				new java.util.Vector<String>());
		_multiMappings.put(_MULTIPLE_LINE_KEYWORD_WRITE_SCALAR_OPERATION_INT,
				new java.util.Vector<String>());
		_multiMappings.put(_MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_OPERATION_INT,
				new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_TWO_POW_D_OPERATION_INT,
				new java.util.Vector<String>());
		_multiMappings.put(_MULTIPLE_LINE_KEYWORD_WRITE_STENCIL_OPERATION_INT,
				new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_WRITE_INTERGRID_OPERATOR_OPERATION_INT,
				new java.util.Vector<String>());

		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_READ_SCALAR_OPERATION_COMPLEX,
				new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_READ_VECTOR_OPERATION_COMPLEX,
				new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_READ_VECTOR_TWO_POW_D_OPERATION_COMPLEX,
				new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_READ_STENCIL_OPERATION_COMPLEX,
				new java.util.Vector<String>());
		_multiMappings
				.put(_MULTIPLE_LINE_KEYWORD_READ_INTERGRID_OPERATOR_OPERATION_COMPLEX,
						new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_WRITE_SCALAR_OPERATION_COMPLEX,
				new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_OPERATION_COMPLEX,
				new java.util.Vector<String>());
		_multiMappings
				.put(_MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_TWO_POW_D_OPERATION_COMPLEX,
						new java.util.Vector<String>());
		_multiMappings.put(
				_MULTIPLE_LINE_KEYWORD_WRITE_STENCIL_OPERATION_COMPLEX,
				new java.util.Vector<String>());
		_multiMappings
				.put(_MULTIPLE_LINE_KEYWORD_WRITE_INTERGRID_OPERATOR_OPERATION_COMPLEX,
						new java.util.Vector<String>());
	}

	@Override
	public void inAReadScalarDoubleOperation(AReadScalarDoubleOperation node) {
		_multiMappings.get(_MULTIPLE_LINE_KEYWORD_READ_SCALAR_OPERATION_DOUBLE)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAReadVectorDoubleOperation(AReadVectorDoubleOperation node) {
		_multiMappings.get(_MULTIPLE_LINE_KEYWORD_READ_VECTOR_OPERATION_DOUBLE)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAReadVectorTwoPowDDoubleOperation(
			AReadVectorTwoPowDDoubleOperation node) {
		_multiMappings.get(
				_MULTIPLE_LINE_KEYWORD_READ_VECTOR_TWO_POW_D_OPERATION_DOUBLE)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAReadStencilDoubleOperation(AReadStencilDoubleOperation node) {
		_multiMappings
				.get(_MULTIPLE_LINE_KEYWORD_READ_STENCIL_OPERATION_DOUBLE).add(
						node.getName().toString().trim());
	}

	@Override
	public void inAReadIntergridOperatorDoubleOperation(
			AReadIntergridOperatorDoubleOperation node) {
		_multiMappings
				.get(_MULTIPLE_LINE_KEYWORD_READ_INTERGRID_OPERATOR_OPERATION_DOUBLE)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAWriteScalarDoubleOperation(AWriteScalarDoubleOperation node) {
		_multiMappings
				.get(_MULTIPLE_LINE_KEYWORD_WRITE_SCALAR_OPERATION_DOUBLE).add(
						node.getName().toString().trim());
	}

	@Override
	public void inAWriteVectorTwoPowDDoubleOperation(
			AWriteVectorTwoPowDDoubleOperation node) {
		_multiMappings.get(
				_MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_TWO_POW_D_OPERATION_INT)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAWriteVectorDoubleOperation(AWriteVectorDoubleOperation node) {
		_multiMappings
				.get(_MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_OPERATION_DOUBLE).add(
						node.getName().toString().trim());
	}

	@Override
	public void inAWriteStencilDoubleOperation(AWriteStencilDoubleOperation node) {
		_multiMappings.get(
				_MULTIPLE_LINE_KEYWORD_WRITE_STENCIL_OPERATION_DOUBLE).add(
				node.getName().toString().trim());
	}

	@Override
	public void inAWriteIntergridOperatorDoubleOperation(
			AWriteIntergridOperatorDoubleOperation node) {
		_multiMappings
				.get(_MULTIPLE_LINE_KEYWORD_WRITE_INTERGRID_OPERATOR_OPERATION_DOUBLE)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAReadScalarIntOperation(AReadScalarIntOperation node) {
		_multiMappings.get(_MULTIPLE_LINE_KEYWORD_READ_SCALAR_OPERATION_INT)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAReadVectorIntOperation(AReadVectorIntOperation node) {
		_multiMappings.get(_MULTIPLE_LINE_KEYWORD_READ_VECTOR_OPERATION_INT)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAReadVectorTwoPowDIntOperation(
			AReadVectorTwoPowDIntOperation node) {
		_multiMappings.get(
				_MULTIPLE_LINE_KEYWORD_READ_VECTOR_TWO_POW_D_OPERATION_INT)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAReadStencilIntOperation(AReadStencilIntOperation node) {
		_multiMappings.get(_MULTIPLE_LINE_KEYWORD_READ_STENCIL_OPERATION_INT)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAReadIntergridOperatorIntOperation(
			AReadIntergridOperatorIntOperation node) {
		_multiMappings.get(
				_MULTIPLE_LINE_KEYWORD_READ_INTERGRID_OPERATOR_OPERATION_INT)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAWriteScalarIntOperation(AWriteScalarIntOperation node) {
		_multiMappings.get(_MULTIPLE_LINE_KEYWORD_WRITE_SCALAR_OPERATION_INT)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAWriteVectorTwoPowDIntOperation(
			AWriteVectorTwoPowDIntOperation node) {
		_multiMappings.get(
				_MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_TWO_POW_D_OPERATION_INT)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAWriteVectorIntOperation(AWriteVectorIntOperation node) {
		_multiMappings.get(_MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_OPERATION_INT)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAWriteStencilIntOperation(AWriteStencilIntOperation node) {
		_multiMappings.get(_MULTIPLE_LINE_KEYWORD_WRITE_STENCIL_OPERATION_INT)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAReadScalarComplexOperation(AReadScalarComplexOperation node) {
		_multiMappings
				.get(_MULTIPLE_LINE_KEYWORD_READ_SCALAR_OPERATION_COMPLEX).add(
						node.getName().toString().trim());
	}

	@Override
	public void inAReadVectorComplexOperation(AReadVectorComplexOperation node) {
		_multiMappings
				.get(_MULTIPLE_LINE_KEYWORD_READ_VECTOR_OPERATION_COMPLEX).add(
						node.getName().toString().trim());
	}

	@Override
	public void inAReadVectorTwoPowDComplexOperation(
			AReadVectorTwoPowDComplexOperation node) {
		_multiMappings.get(
				_MULTIPLE_LINE_KEYWORD_READ_VECTOR_TWO_POW_D_OPERATION_COMPLEX)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAReadStencilComplexOperation(AReadStencilComplexOperation node) {
		_multiMappings.get(
				_MULTIPLE_LINE_KEYWORD_READ_STENCIL_OPERATION_COMPLEX).add(
				node.getName().toString().trim());
	}

	@Override
	public void inAReadIntergridOperatorComplexOperation(
			AReadIntergridOperatorComplexOperation node) {
		_multiMappings
				.get(_MULTIPLE_LINE_KEYWORD_READ_INTERGRID_OPERATOR_OPERATION_COMPLEX)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAWriteScalarComplexOperation(AWriteScalarComplexOperation node) {
		_multiMappings.get(
				_MULTIPLE_LINE_KEYWORD_WRITE_SCALAR_OPERATION_COMPLEX).add(
				node.getName().toString().trim());
	}

	@Override
	public void inAWriteVectorTwoPowDComplexOperation(
			AWriteVectorTwoPowDComplexOperation node) {
		_multiMappings.get(
				_MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_TWO_POW_D_OPERATION_INT)
				.add(node.getName().toString().trim());
	}

	@Override
	public void inAWriteVectorComplexOperation(AWriteVectorComplexOperation node) {
		_multiMappings.get(
				_MULTIPLE_LINE_KEYWORD_WRITE_VECTOR_OPERATION_COMPLEX).add(
				node.getName().toString().trim());
	}

	@Override
	public void inAWriteStencilComplexOperation(
			AWriteStencilComplexOperation node) {
		_multiMappings.get(
				_MULTIPLE_LINE_KEYWORD_WRITE_STENCIL_OPERATION_COMPLEX).add(
				node.getName().toString().trim());
	}

	@Override
	public void inAWriteIntergridOperatorComplexOperation(
			AWriteIntergridOperatorComplexOperation node) {
		_multiMappings
				.get(_MULTIPLE_LINE_KEYWORD_WRITE_INTERGRID_OPERATOR_OPERATION_COMPLEX)
				.add(node.getName().toString().trim());
	}

	public void setThisTypenameToMappingTypename(String typename) {
		_mapping.put(_TEMPORARY_KEYWORD_TYPENAME, typename);
		_mapping.put(_TEMPORARY_KEYWORD_FULL_QUALIFIED_TYPENAME,
				_mapping.get(_KEYWORD_NAMESPACE) + "::mappings::" + typename);
	}

	public void setThisTypenameToAdapterTypename(String typename) {
		_mapping.put(_TEMPORARY_KEYWORD_TYPENAME, typename);
		_mapping.put(_TEMPORARY_KEYWORD_FULL_QUALIFIED_TYPENAME,
				_mapping.get(_KEYWORD_NAMESPACE) + "::adapters::" + typename);
	}

	public void setThisTypenameToAdapterTypename(String typename,
			java.util.Vector<String> mappings,
			java.util.Vector<String> fullQualifiedMappings,
			java.util.Vector<String> mappingsPath) {
		_mapping.put(_TEMPORARY_KEYWORD_TYPENAME, typename);
		_mapping.put(_TEMPORARY_KEYWORD_FULL_QUALIFIED_TYPENAME,
				_mapping.get(_KEYWORD_NAMESPACE) + "::adapters::" + typename);

		_multiMappings.put(_MULTIPLE_LINE_KEYWORD_MAPPINGS, mappings);
		_multiMappings.put(_MULTIPLE_LINE_KEYWORD_FULL_QUALIFIED_MAPPINGS,
				fullQualifiedMappings);
		_multiMappings.put(_MULTIPLE_LINE_KEYWORD_MAPPINGS_PATH, mappingsPath);

		/*
		 * if (!mappings.isEmpty() && !predefinedMappings.isEmpty()) {
		 * _mapping.put
		 * (_KEYWORD_IF_MAPPING_EXISTS_AND_PREDEFINED_MAPPING_EXISTS_INSERT_COMMA
		 * , ","); } else { _mapping.put(
		 * _KEYWORD_IF_MAPPING_EXISTS_AND_PREDEFINED_MAPPING_EXISTS_INSERT_COMMA
		 * , ""); }
		 */
	}

	private String getOpeningNamespace(String fullQualifiedName) {
		String namespace = fullQualifiedName;
		String result = "";
		int count = 0;
		while (namespace.length() != 0) {
			for (int i = 0; i < count; i++) {
				result += " ";
			}
			result += "namespace ";
			if (namespace.indexOf("::") != -1) {
				result += namespace.substring(0, namespace.indexOf("::"));
				namespace = namespace.substring(namespace.indexOf("::") + 2);
				count += 2;
				result += " { \n";
			} else {
				result += namespace;
				namespace = "";
				result += " {";
			}
		}
		return result;
	}

	private String getClosingNamespace(String fullQualifiedName) {
		String namespace = fullQualifiedName;
		String result = "";
		while (namespace.indexOf("::") != -1) {
			result += "}\n";
			namespace = namespace.substring(namespace.indexOf("::") + 2);
		}
		result += "}";
		return result;
	}

	public void inADescription(ADescription node) {
		String namespace = node.getNamespace().getText().trim();
		if (namespace.startsWith("::"))
			namespace = namespace.substring(2);
		_mapping.put(_KEYWORD_NAMESPACE, namespace);
		_mapping.put(_KEYWORD_INCLUDE_GUARD_PREFIX,
				namespace.replaceAll("::", "_").toUpperCase());
		_mapping.put(_KEYWORD_PROJECT_PATH, namespace.replaceAll("::", "/"));

		_mapping.put(_KEYWORD_COMPONENT_NAME, node.getName().getText().trim());

		_mapping.put(_KEYWORD_OPEN_NAMESPACE,
				getOpeningNamespace(_mapping.get(_KEYWORD_NAMESPACE)));
		_mapping.put(_KEYWORD_CLOSE_NAMESPACE,
				getClosingNamespace(_mapping.get(_KEYWORD_NAMESPACE)));
	}

	public void inAAdapter(AAdapter node) {
		_adapters.add(node.getName().getText().trim());
	}

	public void inAVertex(AVertex node) {
		_mapping.put(_KEYWORD_DOF_DASTGEN_DEFINITION_VERTEX, node.getFilename()
				.getText().trim());
	}

	public void inACell(ACell node) {
		_mapping.put(_KEYWORD_DOF_DASTGEN_DEFINITION_CELL, node.getFilename()
				.getText().trim());
	}

	public void inAState(AState node) {
		_mapping.put(_KEYWORD_DOF_DASTGEN_DEFINITION_STATE, node.getFilename()
				.getText().trim());
	}

	public void outStart(Start node) {
		String enumDeclaration = "enum Action { WriteCheckpoint, ReadCheckpoint, Terminate, RunOnAllNodes";
		for (String p : _adapters) {
			enumDeclaration += ",UseAdapter";
			enumDeclaration += p;
		}
		enumDeclaration += ",NumberOfAdapters";
		enumDeclaration += "}";

		_mapping.put(_KEYWORD_REPOSITORY_ACTION_ENUM, enumDeclaration);

		_multiMappings.put(_MULTIPLE_LINE_KEYWORD_FULL_QUALIFIED_ADAPTER_TYPE,
				new java.util.Vector<String>());
		_multiMappings.put(_MULTIPLE_LINE_KEYWORD_NONQUALIFIED_ADAPTER_TYPE,
				new java.util.Vector<String>());

		for (String p : _adapters) {
			String typeName = p;
			String qualifiedTypeName = _mapping.get(_KEYWORD_NAMESPACE)
					+ "::adapters::" + p;
			_multiMappings
					.get(_MULTIPLE_LINE_KEYWORD_NONQUALIFIED_ADAPTER_TYPE).add(
							typeName);
			_multiMappings.get(
					_MULTIPLE_LINE_KEYWORD_FULL_QUALIFIED_ADAPTER_TYPE).add(
					qualifiedTypeName);
		}

		_mapping.put(_KEYWORD_NUMBER_OF_ADAPTERS_,
				java.lang.Integer.toString(_adapters.size()));
	}

	static public String getCellUnqualifiedName() {
		return "Cell";
	}

	static public String getStateUnqualifiedName() {
		return "State";
	}

	static public String getVertexUnqualifiedName() {
		return "Vertex";
	}

	static public String getRepositoryStateUnqualifiedName() {
		return "RepositoryState";
	}

	private String replaceKeywords(String line) {
		String text = line;
		for (String p : _mapping.keySet()) {
			text = text.replaceAll(p, _mapping.get(p));
		}
		return text;
	}

	public void copyTemplateFile(String templateFile, String destinationFile,
			boolean overwrite) {
		destinationFile = destinationFile.replaceAll("\\\\", "/");
		if ((new java.io.File(destinationFile)).exists() && !overwrite) {
			System.out.print("\n " + destinationFile + " already exists)");
		} else {
			try {
				java.io.OutputStreamWriter writer = new java.io.OutputStreamWriter(
						new java.io.FileOutputStream(destinationFile));
				String text = new String();
				java.io.BufferedReader reader = getBufferedReaderForStdTemplate(templateFile);

				while ((text = reader.readLine()) != null) {
					writer.write(text);
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

	/**
	 * The keywords are just replaced by another string.
	 * 
	 * Lines starting with MULTIPLE_REPLACEMENT_LINE_PREFIX are replaced one by
	 * one with the multisets. There's also a variant with comma.
	 */
	public void convertTemplateFile(String templateFile,
			String destinationFile,
			java.util.ArrayList<String> replaceAdditionalString,
			java.util.ArrayList<String> withString, boolean overwrite,
			boolean extractTemplateFromJar) {
		destinationFile = destinationFile.replaceAll("\\\\", "/");

		if ((new java.io.File(destinationFile)).exists() && !overwrite) {
			System.out.print("\n  (blueprint of " + destinationFile
					+ " already exists)");
		} else {
			try {
				java.io.OutputStreamWriter writer = new java.io.OutputStreamWriter(
						new java.io.FileOutputStream(destinationFile));
				String text = new String();

				java.io.BufferedReader reader = null;
				if (extractTemplateFromJar) {
					reader = getBufferedReaderForStdTemplate(templateFile);
				} else {
					reader = getBufferedReaderForUserTemplate(templateFile);
				}

				while ((text = reader.readLine()) != null) {
					for (String p : _multiMappings.keySet()) {
						boolean isPattern = false;
						boolean appendComma = false;
						String textToRemove = "";

						if (text.contains(MULTIPLE_REPLACEMENT_LINE_PREFIX_WITH_COMMA + p)) {
							isPattern = true;
							appendComma = true;
							textToRemove = MULTIPLE_REPLACEMENT_LINE_PREFIX_WITH_COMMA
									+ p;
						} else if (text.contains(MULTIPLE_REPLACEMENT_LINE_PREFIX + p)) {
							isPattern = true;
							appendComma = false;
							textToRemove = MULTIPLE_REPLACEMENT_LINE_PREFIX + p;
						}

						if (isPattern) {
							String modifiedText = text.replaceAll(textToRemove,"");
							text = "";
							int numberOfOustandingReplacements = _multiMappings.get(p).size();
							int replacementCounter = 0;
							for (String replacement : _multiMappings.get(p)) {
								text += modifiedText.replaceAll(p, replacement).replaceAll("#no", Integer.toString(replacementCounter) );
								numberOfOustandingReplacements--;
								if (appendComma	&& numberOfOustandingReplacements > 0) {
									text += ", \n";
								} else {
									text += "\n";
								}
								replacementCounter ++;
							}
						}
					}

					text = replaceKeywords(text);

					if (replaceAdditionalString != null) {
						for (int i = 0; i < replaceAdditionalString.size(); i++) {
							text = text.replaceAll(
									replaceAdditionalString.get(i),
									withString.get(i));
						}
					}

					writer.write(text);
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

	static public java.io.BufferedReader getBufferedReaderForStdTemplate(
			String templateFile) throws FileNotFoundException {
		templateFile = "/stdtemplates/" + templateFile;
		java.io.InputStream stream = Main.class
				.getResourceAsStream(templateFile);
		if (stream == null) {
			throw new FileNotFoundException(templateFile);
		}
		java.io.BufferedReader reader = new java.io.BufferedReader(
				new java.io.InputStreamReader(stream));
		return reader;
	}

	public java.io.BufferedReader getBufferedReaderForUserTemplate(
			String templateFile) throws FileNotFoundException {

		String qualifiedTemplateFile = templateFile;

		for (String d : _templateDirectory) {
			String fullQualifiedPath = d + "/" + templateFile;
			fullQualifiedPath = fullQualifiedPath.replaceAll("\\\\", "/");
			java.io.File testFile = new java.io.File(fullQualifiedPath);
			if (testFile.isFile()) {
				qualifiedTemplateFile = fullQualifiedPath;
			}
		}

		java.io.BufferedReader reader = new java.io.BufferedReader(
				new java.io.FileReader(new java.io.File(qualifiedTemplateFile)));
		return reader;
	}

	/**
	 * Read a template file, replace all the keywords in it with the value, and
	 * write the resulting file to destinationFile.
	 * 
	 * @param destinationFile
	 * @param rootDirectory
	 * @param templateFile
	 */
	public void convertTemplateFile(String templateFile,
			String destinationFile, boolean overwrite,
			boolean extractTemplateFromJar) {
		convertTemplateFile(templateFile, destinationFile, null, null,
				overwrite, extractTemplateFromJar);
	}
}
