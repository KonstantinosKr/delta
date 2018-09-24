// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _SHARED_MEMORY_ORAClES_ORACLE_FOR_ONE_PHASE_WITH_SHRINKING_GRAIN_SIZE_H_
#define _SHARED_MEMORY_ORAClES_ORACLE_FOR_ONE_PHASE_WITH_SHRINKING_GRAIN_SIZE_H_


#include "tarch/logging/Log.h"
#include "tarch/multicore/BooleanSemaphore.h"
#include "peano/datatraversal/autotuning/OracleForOnePhase.h"
#include "peano/datatraversal/autotuning/MethodTrace.h"


#include <map>
#include <set>
#include <vector>


namespace sharedmemoryoracles {
  class OracleForOnePhaseWithShrinkingGrainSize;
}


/**
 * Oracle With Shrinking Grain Size
 *
 * This is a very simple oracle realising some autotuning. It holds a database
 * which allows it to run per method trace, i.e. per calling code region, and
 * per problem size through various phases:
 *
 * - First, it sets the grain size to the problem size and tries to get the
 *   serial cost per entity.
 * - Whenever a valid time is found, it tries out some smaller grain size.
 *   If this smaller grain size pays off, we continue with an even smaller
 *   grain size. If it does not pay off, we roll back to the previous
 *   measurement, try to get some more accurate data there and then test
 *   a grain size in-between the previous (working) grain size and the new
 *   one that turned out to be not so good.
 * - Once this difference between previous and new grain size is zero, we have
 *   found a working grain size. We might now decide (randomly) to restart the
 *   search to ensure that we have not ran in to a local minimum.
 *
 * The fundamental assumption for the runtime here is that the speedup over
 * grain size is something like a parabula, i.e. has an unique maximum somewhere
 * between 0 and the total grain size. We assume that a scaling code part
 * performs better if we set the grain size to half of the problem size and then
 * we try to approximate the best grain size iteratively from the right, i.e. our
 * best grain size decreases monotonically.
 *
 * This oracle relies on a workload per cell that is more or less constant. The
 * oracle does not work for particle methods, e.g., where the workload per cell
 * changes all the time.
 *
 *
 * <h2> Usage </h2>
 *
 * The usage per se is trivial: simple pass an instance of this class to the
 * oracle:
 *
 * <pre>
peano::datatraversal::autotuning::Oracle::getInstance().setOracle(
//    new peano::datatraversal::autotuning::OracleForOnePhaseDummy(true)
  new sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize()
);
   </pre>
 *
 * This solution however has one major flaw: The oracle starts to search for
 * good grain sizes everytime even if there have good grain sizes been
 * identified already. Notably, it does not store insight persistently
 * in-between two program runs.
 *
 * To allow the oracle to store its data persistently and to reload properties
 * from older runs, you have to use the oracle's load and plotStatistics
 * commands. Just after you have invoked setOracle(), add a statement alike
 * <pre>
  peano::datatraversal::autotuning::Oracle::getInstance().loadStatistics( "sharedmemory.grain-sizes" );
 </pre>
 *
 * In return, you have to store your data before you quit the program.
 * plotStatistics() allows you to pipe all information directly into a file.
 *
 * <pre>
  peano::datatraversal::autotuning::Oracle::getInstance().plotStatistics( "sharedmemory.grain-sizes" );
   </pre>
 *
 *
 * If you apply your code to multiple problems, it might be reasonable to run
 * the autotuning for different characteristic problem sets. Often, a good
 * grain size depends on the input parameters, too.
 *
 * Please note that an identification can be freakingly slow for bigger problem
 * runs. You have to wait for ages until even a serial runtime can be
 * determined. To avoid this, I suggest that you start your experiments with
 * very small test runs on characteristic grid setups. Once you increase the
 * problem size afterwards successively, the oracle requires new data for the
 * bigger setup. It does hwoever not start from scratch now but extrapolates
 * previous parameter choices that proofed to pay off.
 *
 *
 * <h2> Analysing the output </h2>
 *
 * The output file (sharedmemory.gain-sizes in the above example) is a plain
 * text file. It is however non-trivial to read. Therefore, I provide a Python
 * script in this directory to analyse the files. It accepts the grain size
 * file (you may actually pass a whole set of files, i.e. use a wildcard) and
 * creates per input file a HTML file that you can read with your browser.
 *
 *
 * <h2> MPI+X </h2>
 *
 * If you use the oracle in an MPI environment, please ensure that only one
 * rank does the learning. Ensure it is not the rank 0 as rank 0 typically
 * has non-compute jobs. It would be a waste of resources to allow all MPI
 * ranks to learn in parallel. Instead, I suggest that one (randomly) chosen
 * rank does the learning, pipes the grain sizes into a file from time to time
 * and the other ranks read out this file and use the grain sizes therein.
 *
 *
 * <h2> Some rationale </h2>
 *
 * - I did experiment quite a while with a setup where I tried to find one
 *   grain size per method trace. This however does not work properly. For
 *   large problem sizes, large grain sizes are good as they yield big chunks
 *   and, hence, small administrative overhead. For small problem sizes,
 *   small grain sizes pay off. I thus decided to use a bucketing. A maximum
 *   problem size of 2 is analysed separately from a maximum problem size of
 *   4 and 8 and so forth.
 *
 *
 *
 *
 *
 * @author Tobias Weinzierl
 */
class sharedmemoryoracles::OracleForOnePhaseWithShrinkingGrainSize: public peano::datatraversal::autotuning::OracleForOnePhase {
  public:
    enum class SelectNextStudiedMeasureTraceStrategy {
      Randomised,
      Cyclic,
      RandomisedWithHigherPrioritiesForSmallProblemSizes
    };
  private:
    static tarch::logging::Log                           _log;

    static const double                                  _MaxAccuracy;
    static const double                                  _WideningFactor;

    /**
     * @see parallelSectionHasTerminated()
     */
    static const double                                  _InitialRelativeAccuracy;

    static bool                                          _hasLearnedSinceLastQuery;

    static tarch::multicore::BooleanSemaphore            _semaphore;

    /**
     * We never do optimise all traces. We only do it with one trace at a time.
     * This field identifies which trace currently is studied.
     */
    peano::datatraversal::autotuning::MethodTrace        _activeMethodTrace;

    /**
     * We can switch off the learning. I keep this flag just to allow a user to
     * read in a configuration and then keep it. This feature comes in very
     * handy in an MPI+X context. See class description.
     */
    const bool                                           _learn;
    const bool                                           _restart;
    const SelectNextStudiedMeasureTraceStrategy          _selectNextStudiedMeasureTraceStrategy;

    /**
     * Per method trace, we hold multiple problem analyses. See rationale
     * section in class description. Basically, we realise a binning and
     * all entries in the database are sorted ascendingly, The first database
     * entry is relevant if, for a given maximum problem size n, n is smaller
     * than 2 as the first entry's _biggestProblemSize always is set to 2.
     * The next database entry is relevant if n is smaller or equal to the
     * subsequent's entry _biggestProblemSize and so forth.
     *
     * The current grain size identifies which grain size is to be used for
     * problems that fit into the database, i.e. are smaller than
     * _biggestProblemSize though not covered by the previous database entry.
     * If the grain size equals the biggest problem size handled, then
     * parallelisation is switched off. We run serially.
     *
     * Whenever we have obtained a valid measurement for a grain size a, we
     * continue with a smaller grain size b. To track measurement data, we
     * rely on an instance of Measurement which provides us with all kinds of
     * statistics. The only thing we keep from the previous measurment, i.e.
     * the one of point a, is the measured time. Once we have a statistically
     * meaningful measurement for the new grain size choice, we compare the
     * new average time per entity to _previousMeasuredTime. As a result,
     * whenever we set _previousMeasuredTime to _TimingMax, we ensure that a
     * new measurement for _currentGrainSize is made and then definitely
     * accepted as the new best-case parameter choice.
     *
     * The search delta has no impact on the current measurement. It encodes
     * however by which quantity we have to reduce _currentGrainSize once we
     * have obtained a valid new measurement and found that this one is better
     * than the previous measurement.
     */
    struct DatabaseEntry {
      private:
        int                          _biggestProblemSize;
        int                          _currentGrainSize;
        int                          _searchDelta;
        bool                         _measurementsAreAccurate;
        double                       _accuracy;
        double                       _accumulatedSerialMeasurement;
        double                       _accumulatedParallelMeasurement;
        double                       _numberOfSerialMeasurements;
        double                       _numberOfParallelMeasurements;
        double                       _previousSpeedup;

      public:
        /**
         * Insert/reset entries into search database
         *
         * When we insert a new entry we either set the search size such that the
         * next size is half the problem i.e. we could exploit two cores. If the
         * problem however is large, it does make sense to split it up more
         * aggressively right from the start. If this is not a good idea, the
         * code will reduce the search step size anyway.
         */
        DatabaseEntry(int problemSize);

        /**
         * Constructs entry from string. String should be written through
         * toString(). This is basically a parse constructor interpreting one
         * line from an input file.
         *
         * We do not realise any error handling here and rely solely on std lib
         * operations, i.e., we inherit their error management.
         */
        DatabaseEntry( std::string  myString );

        /**
         * Construct new data base entry using another one as prototype
         *
         * This operation may be used if and only if prototype refers to a
         * database entry corresponding to a smaller problem size than
         * newProblemSize. Two variants do exist:
         *
         * - prototype represents a measurement where no scaling parameter
         *   choice for prototype's maximum problem size has been found yet.
         *   In this case, this constructor equals DatabaseEntry(int). It
         *   starts a completely new search for good grain sizes.
         * - prototype represents a measurements where a scaling parameter
         *   has already been found. As prototype handles a maximum problem
         *   size smaller than newProblemSize, we may assume that prototype's
         *   grain size does a reasonable job for newProblemSize, too. We
         *   thus inherit prototype's problem size and invoke restart(). This
         *   induces a grain size extrapolation.
         */
        DatabaseEntry( const DatabaseEntry& prototype, int newProblemSize );

        /**
         * Restart the search for a problem setup
         *
         * This operation is used in two contexts. We use it if we hav e
         * terminated the search for a given problem size and (through a
         * random trigger, e.g.) decide that we want to restart the search
         * once again to ensure we did not run into a local minimum. In this
         * case, the grain size might be smaller than the maximum problem
         * size. Yet, it might also be that grain size equals the maximum
         * problem size if the oracle had the impression that parallelisation
         * did not pay off at all.
         *
         * If the entry results from an extrapolation call, i.e. from the
         * constructor, then grain size definitely is smaller then the
         * maximum problem size.
         *
         * The restart makes the new grain size the average between the
         * previous grain size and the maximum problem size. If a restart
         * is triggered on a non-scaling code part, this grain size will
         * equal the maximum problem size. If it is triggered on a scaling
         * code fragment (either one that turned out to be so good that the
         * oracle stopped to search further or one well-suited for a smaller
         * maximum problem size), then we choose a new problem size that is
         * bigger than this working grain size.
         *
         * Next, we set _previousMeasuredTime to _TimingMax, i.e. this new
         * grain size will, once the measurement has terminated, be accepted
         * as new 'optimal' grain size. Please note that the constructor calls
         * restart() if and only if the prototype measurement handed in did
         * already scale.
         *
         * We set the accuracy to zero. It will be adopted in
         * parallelSectionHasTerminated() later on automatically the first time
         * this database entry is subject of the autotuning.
         *
         * Finally, we set the grain size such that the entry tested after
         * the new grain size will be the previous one. That means we will
         * first test a new (bigger) grain size and then next compare this
         * one to the grain size that did work properly before. If the best-case
         * grain size is bigger than the old grain size, we'll automatically get
         * close to this one. If increasing the grain size is not a good idea,
         * we'll fall back automatically to the previous (smaller) grain size
         * once two measurements have completed.
         */
        void restart();

        /**
         * Pipes an entry into a string. Can be used to pipe the database
         * into a file. Is made the counterpart of DatabaseEntry(std::string),
         * i.e. if we use this operation to dump data, the constructor can
         * read this data again.
         */
        std::string toString() const;

        /**
         * This operator is a relict. Technically, the database holds only
         * sets. If we used std::set however, we'd have issues with updating
         * entries. The C++ set is very restrictive and typically does not
         * allow you to alter entries as you might alter the set ordering
         * (though we know we don't here). As a result, it returns only
         * const references by default and this means that we can't make
         * set entries learn. I therefore use the vector instead.
         */
        bool operator<(const DatabaseEntry& cmp) const;

        /**
         * Make a database entry learn.
         *
         * This operation may be called if and only if _currentMeasurement holds
         * a valid value. That means the previous few measurements obtained did
         * not alter the mean value more than the prescrived accuracy. There are
         * various combinations that now can occur:
         *
         * - If the new measurement is smaller than the previous measurement, the
         *   current grain size performs better than the grain size before. So we
         *   keep it. Before we discuss the details, lets highlight that a maximum
         *   problem size N might initialise a search delta of N/2. If N/2 is
         *   better than a serial run, we might run from N into N/2 into 0 as
         *   grain size.
         *   -- We first check whether another decrease of the grain size by
         *      _searchDelta would yields a grain size smaller or equal than 0.
         *      If this would happen, we reduce half _searchDelta.
         *   -- We set a new grain size that is smaller than grain size by this
         *      delta. This is the grain size the oracle will study next. If it
         *      turns out to be a stupid idea, we'll roll back.
         *   -- We finally erase the measurement and increase the accuracy. The
         *      previous grain size choice has paid off, so we'll try a new
         *      grain size. And we want to get the measurements correct, so we
         *      increase the accuracy, i.e. we are more picky.
         * - If the currently studied grain size yields a runtime that is worse
         *   than the runtime before, we have to roll back. We know that
         *   _searchDelta holds the step size how we ended up with the present
         *   grain size.
         *   -- We add _searchDelta to the grain size. This is effectively a
         *      roll back to the grain size studied before.
         *   -- We set _previousMeasuredTime to _TimingMax. We do not know the
         *      timing results for the previous grain size anymore. We actually
         *      do know its average timing result, but we do not care - it will
         *      be reanalysed again next anyway. Yet, we set
         *      _previousMeasuredTime for these following tests to the maximum to
         *      ensure that this one will be accepted in any case.
         *   -- We clear the measurement and increase the accuracy to get more
         *      accurate data.
         *   -- We reduce the search delta. We know that the 'next' measurement,
         *      which is actually a redo of one grain size with an increase
         *      accuracy, will be accepted and so we'll then reduce this bigger
         *      grain size again. It should however not go back to the current
         *      grain size straight away as this would be an infinite oscillation.
         *      So we decrease _searchDelta. If the maximum problem size is small
         *      compared to the number of threads, we do decrement the search
         *      interval. We have to assume that any timinig is very sensitive.
         *      Otherwise, if the grain size is still big, we divide it by the
         *      number of threads. At least, we divide by two.
         */
        void learn();

        bool isAccurate() const;

        bool isSearching() const;

        bool isScaling() const;

        bool isStudyingScalingSetup() const;

        int  getBiggestProblemSize() const;
        int  getCurrentGrainSize() const;

        void setValue( int grainSize, double value );

        /**
         * @return Has widened
         */
        bool widenAccuracy();
        void initAccuracy(double serialComputeTime);
        bool isAccuracyInitialised() const;

        /**
         * A stable entry is one that either has terminated its search or
         * doesn't hold any entries yet.
         */
        bool isStable() const;
    };



    /**
     * Please consult the DatabaseEntry's smaller operator for details
     * why we may not use
     *
     * typedef std::set<DatabaseEntry>  MethodTraceData;
     *
     * here.
     */
    typedef std::vector<DatabaseEntry>  MethodTraceData;

    /**
     * @see DatabaseEntry
     */
    std::map<peano::datatraversal::autotuning::MethodTrace,MethodTraceData >               _measurements;

    /**
     * Searches for the right data base entry (bin) and returns a reference.
     *
     * For each method trace, we hold multiple database entry. Our choice of
     * entry depends on the maximum problem size for which we currently search
     * a grain size. However, we do not hold entries for all possible maximum
     * problem sizes, but we stick to a binning approach. We hold one entry
     * for a maximum problem size of 2, we hold an entry for a size of 4, 8,
     * and so forth.
     *
     * If we query this routine for a method trace where we do not have any
     * entry at all, the routine first of all creates an (empty) entry for a
     * maximum problem size of 2. This is the starting point.
     *
     * If we query this routine for a method trace and a problem size that we
     * haven't known before, i.e., if the problem size we need a grain size for
     * is bigger than any maximum problem size we've seen ever before, we
     * look at the last measurement, and create a new one with twice the
     * problem size. To create a new entry, we do use the copy constructor that
     * extrapolates reasonable grain sizes automatically.
     *
     * @todo
     */
    DatabaseEntry& getDatabaseEntry(int problemSize, peano::datatraversal::autotuning::MethodTrace askingMethod);

    /**
     * If not availble, return biggest one.
     */
    DatabaseEntry  getDatabaseEntry(int problemSize, peano::datatraversal::autotuning::MethodTrace askingMethod) const;
    bool hasDatabaseEntry(int problemSize, peano::datatraversal::autotuning::MethodTrace askingMethod) const;
    void createDatabaseEntries(int problemSize, peano::datatraversal::autotuning::MethodTrace askingMethod);

    /**
     * This operation analyses whether a set of entries for a given method
     * trace is stable. A set is stable if and only if all entries
     * have grain size 0, i.e., we are not searching for good grain
     * sizes anymore.
     *
     * If the property holds, it might be reasonable to restart the search.
     * At least, we should skip the method trace while we search for valid
     * grain sizes.
     *
     * @see changeMeasuredMethodTrace() where it is used by the random
     *      realisation variant.
     */
    bool isStableDatabaseEntrySet( const peano::datatraversal::autotuning::MethodTrace&  askingMethod) const;

    /**
     * Is called by activateOracle() after it has chosen a new method trace that is to be sampled.
     *
     * We often find searches not converging at all. Our code computes an
     * accuracy per new entry from the serial runtime that we want to be
     * met by the average runtime: any new measurement should not change the
     * average runtime for a certain grain size by more than this accuracy
     * anymore - we then assume that the mean value is accurate; see
     * parallelSectionHasTerminated(). When we refine our search, we
     * increase this accuracy.
     *
     * Timing measurements are subject to too much noise, so
     * we never end up with reasonable data. Therefore, whenever we decide to
     * measure timings for a particular method trace, we slightly decrease the
     * accuracy we wanna have for this setup. This means, the longer we do not
     * find any converged timing for a method trace, the more we relax what we
     * define to be accurate.
     *
     * Once this widening is performed, we add a second check. If no widening is
     * done at all, this implies that entries either never have been used
     * (which renders any follow-up action on these entries irrelevant anyway)
     * or all have converged. So we do a rand on top and in one percent of all
     * cases decide for a restart().
     *
     * Widening increases the probability
     * that we end up in a local minimum. We therefore have to combine
     * widening with a restart mechanism as described above, and we have to
     * reset the widening whenever we've found some better scaling parameter
     * somewhere in the system. See clearAllMeasurementsBesidesActiveOne().
     * Otherwise all the measurements would become arbitrarily fuzzy.
     */
    void widenAccuracyOfCurrentlyStudiedMethodTraceAndRandomlyRestart();

    /**
     * Change measured method trace
     *
     * There are two different strategies implemented currently: randomised
     * choice and simple increments of currently studied trace.
     *
     * <h2>Randomised selection</h2>
     *
     * The fundamental idea of this strategy is to choose a random entry from
     * the traces, until
     *
     * - we find a database entry, i.e. until the method trace identifies an
     *   existing entry from the database and
     * - this entry is not stable. See the definition of
     *   isStableDatabaseEntrySet(). A stable entry set is an entry set where
     *   no searching is required anymore. We do prefer the active method trace
     *   to identify parts of the code that still require tuning.
     *
     * Obviously, the latter criterion may make the loop searching for a
     * random entry run pretty long. We might not even be able to meet the
     * criterion at all. So we try for a couple of times
     * to meet it. If we do not succeed, we neglect it.
     *
     * <h2> Deterministic increase </h2>
     *
     * Very similar to the randomised one but it just increases the method
     * trace up to a couple of times rather than using random traces.
     */
    void changeMeasuredMethodTrace();

  public:
    /**
     * Oracle Constructor
     */
    OracleForOnePhaseWithShrinkingGrainSize(
      bool learn,
      bool restart,
      SelectNextStudiedMeasureTraceStrategy selectNextStudiedMeasureTraceStrategy = SelectNextStudiedMeasureTraceStrategy::RandomisedWithHigherPrioritiesForSmallProblemSizes
    );

    /**
     * Nop
     */
    virtual ~OracleForOnePhaseWithShrinkingGrainSize();

    /**
     * Find out whether a certain part of the code should be parallelised and with which grain size
     *
     * The routine runs through a couple of steps:
     *
     * - Through getDatabaseEntry() it finds the right database entry. Consult
     *   getDatabaseEntry() for details - invoking the routine implies that
     *   new database entries are generated on-the-fly.
     * - Find out weather we shall do timings. We track time if and only if the
     *   asking method is the currently tracked one and if the corresponding
     *   database entry holds the property that timings still are necessary.
     * - Create a grain size object that is responsible to feed back any timings
     *   into the oracle.
     */
    peano::datatraversal::autotuning::GrainSize parallelise(int problemSize, peano::datatraversal::autotuning::MethodTrace askingMethod) override;

    /**
     * Accept a new measurement
     *
     * If parallelise() has decided that a measurement is to be made, it
     * creates an instance of GrainSize that does the measuring. This
     * instance then returns its result to this routine once the grain size
     * object is destroyed.
     *
     * By default, the measurement is added to the currently active
     * measurement object encapsulting all statistics. The only important
     * thing yet to be done is to check the accuracy. Whenever the database
     * creates any new entry, the accuracy that we want to have for the first
     * measurement of this entry is set to zero. This is bullshit as it would
     * mean that we wanna have infinitely accurate data at the end of the day.
     * So we use this state as a marker. If the accuracy is set to zero, we
     * kind of reconstruct how long this particular code fragment would have
     * ran in serial in total.
     *
     * We are really interested in the total runtime not in the normalised cost
     * per grid entity. Usually we work with the normalised data as we work
     * with binning, i.e. multiple problem setups might be associated to one
     * measurement. To be able to compare them, we have to normalise by the
     * number of vertices handled, e.g. For the accuracy, we take the real
     * time, we scale it with _InitialRelativeAccuracy and we make this the
     * accuracy we want to see from hereon.
     */
    void parallelSectionHasTerminated(int problemSize, int grainSize, peano::datatraversal::autotuning::MethodTrace askingMethod, double costPerProblemElement) override;

    /**
     * See DatabaseEnty's constructor or toString(), respectively.
     */
    void plotStatistics(std::ostream& out, int oracleNumber) const override;

    /**
     * See DatabaseEnty's constructor or toString(), respectively.
     */
    void loadStatistics(const std::string& filename, int oracleNumber) override;

    /**
     * Nop
     */
    void deactivateOracle() override;

    /**
     * If the method trace we currently are tracking did yield any new valid
     * measurement, we make the corresponding database entry learn(). If any
     * learns are done, we clearAllMeasurementsBesidesActiveOne().
     *
     * Afterwards, we changeMeasuredMethodTrace() if there are any database
     * entries at all. If the database is empty, we wait as any subsequent
     * parallelise() call will start to befill the database. If we can switch
     * the active trace, we subsequently
     * widenAccuracyOfCurrentlyStudiedMethodTraceAndRandomlyRestart().
     *
     * <h2> Data propagation </h2>
     *
     * If the search has ran into a situation where the search terminates, i.e.
     * when it has found a working setup, we start to propagate it to all
     * subsequent database entries. Database entries are stored ascendingly.
     * All entries next in the array correspond to bigger total problem sizes.
     * So we run into these following entries. If they are also still searching
     * (if they have already converged we stop the propagation), we use the
     * entries specialised copy constructor to propagate data.
     */
    void activateOracle() override;

    peano::datatraversal::autotuning::OracleForOnePhase* createNewOracle() const override;

    /**
     * Ask weather any oracle has learned. Afterwards reset an internal state.
     * So the next time you call this operation, you know whether something
     * had been learned in-between.
     */
    static bool hasLearnedSinceLastQuery();
};


#endif
