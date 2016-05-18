// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_GRID_CHECKPOINT_H_
#define _PEANO_GRID_CHECKPOINT_H_


namespace peano {
  namespace grid {
    template <class Vertex, class Cell>
    class Checkpoint;
  }
}


#include <map>
#include <vector>
#include <iostream>

#include "tarch/logging/Log.h"


/**
 * Abstract Checkpoint
 *
 * !!! Design
 *
 * A Checkpoint consists of the following parts:
 *
 * - A header.
 * - A table holding a map of strings to strings. These strings encode program
 *   parameters, global counters, simple states, the traversal automaton, or
 *   multigrid solver states for example.
 * - The cell data.
 * - The vertex data.
 *
 * !!! Adopt the Checkpoint to Your PDE Solver
 *
 * To support the Checkpoint mechanism for your PDE component, just implement
 * your PDE state's store and read Checkpoint operations, and ensure that your
 * program's state really only depends on the state of this very object. Within
 * your implementation, use the operations storeValueAs... and loadValueAs... of
 * the Checkpoint passed as argument. The state's checkpointing operations are
 * already provided as empty operations due to PeProt, i.e. open the state's
 * implementation file and replace the todo tags.
 *
 * All the values your state doesn't stream to the Checkpoint are lost whenever
 * you store/read a Checkpoint. Consequently, these values have to be
 * reconstructed from somewhere else if you wanna restart your application.
 * Typically there's three different things to do to make an application
 * reconfigurable.
 *
 * - There are some global settings such as the computational geometry, boundary
 *   conditions, load balancing strategy, etc.
 * - There is a state of the solver which comprises global residuals, time
 *   stepping, and so forth.
 * - There's two different configuration objects/runners.
 *
 * The first configuration comprises the global settings from above and the
 * initial conditions. It sets up the simulation, does some runs, and is able
 * to write Checkpoints.
 *
 * The second configuration also comprises the global settings. However, it does
 * not have initial conditions and so forth. It just accepts a Checkpoint. This
 * configuration's runner sets up the global state (connects to the geometry
 * interface, e.g.), loads a Checkpoint, and then switches to the first active
 * adapter.
 *
 * Both runners rely on the same algorithm. However, they start from a different
 * adapter. As writing a Checkpoint is (almost always) one step in the overall
 * algorithm, the Checkpoint usually does not comprise the active adapter, i.e.
 * the second configuration has to reset the active adapter. If this is not what
 * you want, you have to extend the Checkpoint accordingly.
 *
 * !!! Remarks
 *
 * The Checkpoints are realised as binary serialization, i.e. it might not be
 * portable to other machines. Consequently, load a Checkpoint only on the same
 * machine you've written it.
 *
 * @author Tobias Weinzierl
 */
template <class Vertex, class Cell>
class peano::grid::Checkpoint {
  public:
    typedef std::vector<Vertex>                 VertexContainer;
    typedef std::vector<Cell>                   CellContainer;

  private:
    /**
     * Logging device.
     */
    static tarch::logging::Log _log;

    typedef std::map<std::string, std::string>  ValueMap;

    ValueMap         _valueMap;
    VertexContainer  _vertexContainer;
    CellContainer    _cellContainer;
    bool             _isValid;

    static const char        CheckpointFileEntrySeparator;
    static const std::string CheckpointFileHeader;
    static const std::string CheckpointFileValueMapEntriesKeyword;
    static const std::string CheckpointFileVertexEntriesKeyword;
    static const std::string CheckpointFileCellEntriesKeyword;
    static const std::string ValueKeyIsPacked;
    static const std::string ValueKeyIsDebug;
    static const std::string ValueKeyIsParallel;
    static const std::string ValueKeyIsAsserts;
    static const std::string ValueKeyIsPersistentRecords;

    /**
     * Read the string until the next CheckpointFileEntrySeparator token occurs.
     */
    static std::string readStringEntryFromInputStream(std::istream& in);

    /**
     * Read one CheckpointFileEntrySeparator token.
     */
    static void skipEntrySeparatorInInputStream(std::istream& in);

    /**
     * Depending on the current compile flag, operation adds entries to value map.
     *
     * The following compile flags are hold by the map:
     *
     * - PackedRecords
     * - StoreOnlyPersistentAttributes
     * - Asserts
     * - Debug
     */
    void createBuiltInValueMapEntries();
    void validateBuiltInValueMapEntries();

    /**
     * Read the Checkpoint header and the number of value map entries. The
     * latter value is returned.
     */
    ValueMap::size_type readCheckpointHeader(std::istream& in);
    /**
     * Read value map from input stream.
     */
    void readValueMap(const typename ValueMap::size_type& numberOfValueMapEntries, std::istream& in);
    void readVertices(const typename VertexContainer::size_type& numberOfVertexEntries, std::istream& in);
    void readCells(const typename CellContainer::size_type& numberOfCellEntries, std::istream& in);

    void writeCheckpointHeader(std::ostream& out) const;
    void writeValueMap(std::ostream& out) const;
    void writeVertices(std::ostream& out) const;
    void writeCells(std::ostream& out) const;

  public:
    static const std::string StandardFilenameExtension;

    /**
     * Create empty Checkpoint.
     */
    Checkpoint();

    ~Checkpoint();

    void readFromFile( const std::string& fullQualifiedFileName );

    /**
     * Write Checkpoint to a file. In the Peano group, we usually add filename
     * a postfix
     */
    void writeToFile( const std::string& fullQualifiedFileName ) const;

    /**
     * Store a user-defined value into the Checkpoint. Neither the value nor the
     * key may contain a CheckpointFileEntrySeparator. Please avoid the string
     * termination symbol '\0', too.
     */
    void storeValueAsString(const std::string& valueKey, const std::string& value);
    void storeValueAsBool(const std::string& valueKey,   bool               value);
    void storeValueAsInt(const std::string& valueKey,    int                value);
    void storeValueAsDouble(const std::string& valueKey, double             value);

    /**
     * Take individual user-defined value from Checkpoint. Counterpart of
     * storeValue().
     */
    std::string getValueAsString(const std::string& valueKey) const;
    bool        getValueAsBool(const std::string& valueKey) const;
    int         getValueAsInt(const std::string& valueKey) const;
    double      getValueAsDouble(const std::string& valueKey) const;

    bool hasValue(const std::string& valueKey) const;

    int getNumberOfVertices() const;
    int getNumberOfCells() const;

    /**
     * Store a block of vertex data into the Checkpoint stream.
     */
    void storeVertex( const Vertex& vertex );

    /**
     * Counterpart of storeVertex.
     */
    Vertex getVertex(const typename VertexContainer::size_type & i) const;

    void storeCell( const Cell& cell );

    Cell getCell(const typename CellContainer::size_type & i) const;

    /**
     * Does object represent a valid Checkpoint?
     */
    bool isValid() const;
};



#include "peano/grid/Checkpoint.cpph"

#endif
