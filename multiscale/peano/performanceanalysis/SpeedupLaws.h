// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _PEANO_PERFORMANCE_ANALYIS_SPEEDUP_LAWS_H_
#define _PEANO_PERFORMANCE_ANALYIS_SPEEDUP_LAWS_H_


#include "peano/performanceanalysis/Analyser.h"
#include "tarch/logging/Log.h"
#include "tarch/la/Vector.h"


#include <vector>


namespace peano {
  namespace performanceanalysis {
    class SpeedupLaws;

    namespace tests {
      class SpeedupLawsTest;
    }
  }
}



/**
 * Class to compute standard speedup laws
 *
 * Currently, I support solely Amdahl's law, i.e. strong speedup.
 *
 * The class is designed to allow users to determine the parameters of
 * Amdahl's law (strong speedup) from multiple measurements. The class
 * requires to be fed with at least two measurements and then determines
 * the serial fraction f of the overall code and the time the code would
 * need to run on a single core.
 *
 * As soon as we have more than two measurements, the problem is obviously
 * overdetermined. Which makes sense, as all measurement data is typically
 * noisy. So we want to construct a model
 *
 * @f$ t(p) = f \cdot t(1) + (1-f) \cdot \frac{t(1)}{p} @f$
 *
 * from measurement tuples @f$ ( p_i, t(p_i) ) @f$.
 *
 *
 * @author Tobias Weinzierl
 */
class peano::performanceanalysis::SpeedupLaws {
  private:
    /**
     * Number of entries (measurements) hold in database
     */
    static constexpr int     Entries = 64;

    /**
     * Weighting of entries. First one has weight (validity) 1.0, the second
     * has weight Weight( 0.9 ), then Weight times Weight (0.81), and so forth.
     */
    static const double  Weight;

    static const double  MaxF;
    static const double  MinF;
    static const double  MinT1;
    static const double  MinS;

    static tarch::logging::Log _log;

    friend class peano::performanceanalysis::tests::SpeedupLawsTest;

    /**
     * Serial code fraction
     */
    double     _f;
    /**
     * Serial runtime
     */
    double     _t_1;
    /**
     * Startup cost for the threads
     */
    double     _s;

    tarch::la::Vector<Entries, double> _t;
    tarch::la::Vector<Entries, double> _p;

    int        _samples;
  public:
    /**
     * @param MaxEntries Maximum number of measurements taken into account.
     *        Everytime more entries than this one are added, we remove the
     *        oldest measurement.
     */
    SpeedupLaws(double f=0.5);

    /**
     * My original ideas has been to make this function enqueue the new
     * measurement into the dataset, i.e. to append them to _t and _p,
     * respectively. As both sets are sequences (with associated weights)
     * which are truncated - the number of entries is fixed to Entries -
     * every add means that one measurement is dropped.
     *
     * Such an approach is dangerous: If a long sequence of entries is
     * added that all refer to the same p count, our underlying dataset
     * would degenerate to a noisy measurement of one data point. Fitting
     * a performance model into this is impossible. The problem is ill-posed,
     * or the model degenerates, too.
     *
     * So we changed addMeasurement such that it replaced the oldest entry
     * tracking p rather than the overall oldest entry.
     *
     * The whole thing faces one problem: If we have Entries measurement
     * entries, we have to initialise _p and _t somehow. If we initialise
     * them with (0,0) pairs, e.g., we kind of tell the linear algebra that
     * 0,0 were a valid measurement point. However, we know that our model
     * cannot even predict a point 0,0. So we use the very first piece of
     * data dropping in to initialise our code and then squeeze out this
     * initial data successively until we have Entries measurements. From
     * hereon, every new data point replaces the oldest data point available
     * so far.
     */
    void addMeasurement( int p, double t );

    /**
     * This operation determines a new iterate (approximation) for f and t_1
     * following Amdahl's law.
     *
     * Let N=Entries be the number of measurements measurement tuples
     * @f$ ( p_i, t(p_i) ) @f$. We want to compute
     * @f$
         \min _{f,t_1} \frac{1}{2} \sum _i 2^{-i} \left( amdahl(p_i,f,t_1) - t(p_i)  \right) ^2
       @f$
     * with standard strong scaling @f$ amdahl(p_i,f,t_1)=f \cdot t_1 + (1-f) \cdot t_1/p_i @f$.
     *
     * The minimisation implies that we are searching for a solution to the
     * two gradient equations
     * @f$
         \sum _i 2^{-i} \left( amdahl(p_i,f,t_1) - t(p_i)  \right)
         \left(
          \begin{array}{c}
             t_1-t_1/p_i \\
             f+(1-f)/p_i
          \end{array}
         \right)
         =
         \left(
          \begin{array}{c}
             0 \\
             0
          \end{array}
         \right)
       @f$
     *
     *
     * We rewrite the equation in matrix form
     * @f$
       \nabla J
         \left(
          \begin{array}{c}
             2^{0}    \cdot ( amdahl(p_0,f,t_1) - t(p_0) ) \\
             2^{-1/2} \cdot ( amdahl(p_1,f,t_1) - t(p_1) ) \\
             2^{-2/2} \cdot ( amdahl(p_2,f,t_1) - t(p_2) ) \\
             2^{-3/2} \cdot ( amdahl(p_3,f,t_1) - t(p_3) ) \\
             \ldots
          \end{array}
         \right)
         = 0
       @f$
     * so @f$ \nabla J @f$ is a @f$ 2 \times N @f$ matrix with the entries
     * @f$
        (\nabla J)_{1,l} = 2^{(-l+1)/2} \cdot t_1-t_1/p_i \\
        (\nabla J)_{2,l} = 2^{(-l+1)/2} \cdot f+(1-f)/p_i
       @f$
     *
     * In this homogeneous matrix-vector equation, the matrix
     * linearises the system - an approximation as the derivatives within the
     * matrix depend on the solution in reality - and both the unknowns and the
     * matrix depend on the solution.
     * That is, this system is written down as linear equation system, though
     * it is a highly non-linear equation system.
     *
     * In this homogeneous matrix-vector equation, the matrix is
     * linearised - an approximation as the derivaties within the
     * matrix depend on the solution in reality - and the unknowns to
     * the right are non-linear terms, too.
     *
     * As we cannot solve this problem directly, we develop the
     * solution @f$ amdahl(p_i,\hat f,\hat t_1) @f$ from the current
     * estimate @f$ amdahl(p_0,f,t_1) @f$ through a Taylor expansion,
     * i.e. we linearise again.
     * Here, f and t_1 are the current solution, while the hat superscripts
     * identify the real solution.
     * This yields
     * @f$
        amdahl(p_i,\hat f,\hat t_1) = amdahl(p_i,f,t_1)
          + \Delta f  \cdot \left( t_1-t_1/p_i \right)
          + \Delta t_1 \cdot \left( f+(1-f)/p_i \right)
       @f$
     * once we cut off the higher order terms. The development gives us
     * a linearised equation system for the updates on f and t_1, i.e. it expresses
     * the solution tuple @f$ \hat f = f + \Delta f @f$,
     * @f$ \hat t_1 = t_1 + \Delta t_1 @f$ as the current estimate plus a
     * delta. These deltas are called shift vectors.
     *
     * We insert the Taylor expansion into the formula above
     * and obtain
@f$
 \nabla J
  \left(
   \begin{array}{c}
    2^{0} \cdot ( amdahl(p_0,f,t1) - t(p_0) )
    + 2^{0} \cdot \Delta f (t_1-t_1/p_0)
  + 2^{0} \cdot \Delta t_1 (f+(1-f)/p_0)
    \\
    2^{-1/2} \cdot ( amdahl(p_1,f,t1) - t(p_1) )
    + 2^{-1/2} \cdot \Delta f (t_1-t_1/p_1)
  + 2^{-1/2} \cdot \Delta t_1 (f+(1-f)/p_1)
    \\
    2^{-2/2} \cdot ( amdahl(p_2,f,t1) - t(p_2) )
    + 2^{-2/2} \cdot \Delta f (t_1-t_1/p_2)
  + 2^{-2/2} \cdot \Delta t_1 (f+(1-f)/p_2)
    \\
              \ldots
   \end{array}
  \right)
 = 0
@f$
  *
  * We bring the equation parts without any shift vectors to the right-hand side
  * and use our previously introduced quantities to end up with
@f$
\begin{eqnarray*}
\left( \nabla J \right)
\left( \nabla J \right) ^T
  \left(
   \begin{array}{c}
    \Delta f
    \\
    \Delta t_1
   \end{array}
  \right)
 & = &
 - \left( \nabla J \right)
  \left(
   \begin{array}{c}
    2^{0} \cdot ( amdahl(p_0,f,t_1) - t(p_0) )
    \\
    2^{-1/2} \cdot ( amdahl(p_1,f,t_1) - t(p_1) )
    \\
    2^{-2/2} \cdot ( amdahl(p_2,f,t_1) - t(p_2) )
    \\
    2^{-3/2} \cdot ( amdahl(p_3,f,t_1) - t(p_3) )
    \ldots
   \end{array}
  \right)
\end{eqnarray*}
@f$
  *
While the assembly of the linearised equation as well as the vector $y$ to the
right is ``non-trivial'', the resulting $2\times 2$ system can directly be
solved.
However, we have to keep in mind that this is a linearisation, i.e.~the equation system describes the
inner part of a Newton iteration. We therefore need an additional outer loop
comprising the update rule
@f$
  \left(
   \begin{array}{c}
    f
    \\
    t_1
   \end{array}
  \right)
  \gets
   \left(
   \begin{array}{c}
    f
    \\
    t_1
   \end{array}
  \right)
  +
  \left(
   \begin{array}{c}
    \Delta f
    \\
    \Delta t_1
   \end{array}
  \right)
  \quad
  \mbox{with} \
  \left(
   \begin{array}{c}
    \Delta f
    \\
    \Delta t_1
   \end{array}
  \right)
  =
  -
  \left(
    \left( \nabla J \right)
    \left( \nabla J \right) ^T
  \right)^{-1}
    \left( \nabla J \right)
    y
    @f$
     * <h2> Implementation remark </h2>
     *
     * The @f$ 2^{-i} @f$ in the code are replaced with Weight to the power of
     * an integer. Experiments show that Weight=0.5 is too small, i.e. it should
     * be chosen higher.
     */
    void relaxAmdahlsLaw();

    void relaxAmdahlsLawWithThreadStartupCost();

    double getSerialTime() const;
    double getSerialCodeFraction() const;
    double getStartupCostPerThread() const;

    std::string toString() const;

    /**
     * Computes the optimal number of threads for one particular rank if
     * multiple ranks run on the same node and we have an augmented Amdahl
     * model for each of them.
     *
     * <h2> Implementation details </h2>
     *
     * We always start with c=1 for all settings, so we approach the optimal
     * c distribution from below.
     *
     * @param alpha Weight of penalty term
     * @param m     2m is the exponent of the speed weight. The higher m the
     *              closer our algorithm to a minimisation of the maximum
     *              runtime
     * @param eps   Convergence criterion
     */
    static int getOptimalNumberOfThreads(
      int                  rankNumber,
      std::vector<double>  t1,
      std::vector<double>  f,
      std::vector<double>  s,
      int                  totalThreadsAvailable,
      bool                 logResultingDistributionToInto = false,
      double               alpha=0.1,
      int                  m=4,
      double               eps=1e-4
    );
};

#endif
