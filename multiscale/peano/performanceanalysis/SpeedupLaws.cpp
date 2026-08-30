#include "peano/performanceanalysis/SpeedupLaws.h"

#include "tarch/la/Matrix.h"
#include "tarch/la/LUDecomposition.h"


#include <sstream>

tarch::logging::Log peano::performanceanalysis::SpeedupLaws::_log( "peano::performanceanalysis::SpeedupLaws" );


const double  peano::performanceanalysis::SpeedupLaws::Weight  = 0.9;
const double  peano::performanceanalysis::SpeedupLaws::MaxF   = 1.0-1e-2;
const double  peano::performanceanalysis::SpeedupLaws::MinF   = 1e-2;
const double  peano::performanceanalysis::SpeedupLaws::MinT1  = 1e-4;
const double  peano::performanceanalysis::SpeedupLaws::MinS   = 1e-2;


peano::performanceanalysis::SpeedupLaws::SpeedupLaws(double f):
  _f(f),
  _t_1(0.0),
  _s(0.0),
  _samples(0) {
}


void peano::performanceanalysis::SpeedupLaws::addMeasurement( int p, double t ) {
  assertion( p>0 );

  #if defined(PerformanceAnalysis)
  logInfo( "addMeasurement()", "p=" << p << ", t=" << t );
  #endif

  if (_samples==0) {
    _t   = tarch::la::Vector<Entries, double>(t);
    _p   = tarch::la::Vector<Entries, double>(p);
    _t_1 = t * p;
    _samples++;
  }
  else {
    // If you want the algorithm just to erase the oldest measurement, simply
    // keep this entry to its initial value and do not modify it further.
    int measurementToBeDropped = Entries-1;

    // Find last=oldest p entry
    for (int i=1; i<Entries; i++) {
      if ( std::abs( _p(i)-p ) < 1e-5 ) {
        measurementToBeDropped = i;
      }
    }

    for (int i=measurementToBeDropped; i>=1; i--) {
      _p(i) = _p(i-1);
      _t(i) = _t(i-1);
    }

    _p(0) = p;
    _t(0) = t;

    if (_samples<Entries) {
      _t_1 = t;
    }
    _samples++;
  }
}


void peano::performanceanalysis::SpeedupLaws::relaxAmdahlsLaw() {
  if (_samples>Entries) {
    const int NewtonIterations = Entries;
    for (int it=0; it<NewtonIterations; it++) {
      tarch::la::Matrix<2,Entries,double>  gradJ(0.0);
      tarch::la::Vector<Entries,double>    y(0.0);

      for (int n=0; n<static_cast<int>(_p.size()); n++) {
        double amdahlTerm = _f * _t_1 + (1.0-_f) * _t_1/_p(n) - _t(n);
        double dtdf       =      _t_1 -            _t_1/_p(n);
        double dtdt_1      = _f       + (1.0-_f)      /_p(n);

        gradJ(0,n) = std::pow(Weight,n/2.0) * dtdf;
        gradJ(1,n) = std::pow(Weight,n/2.0) * dtdt_1;
        y(n)       = std::pow(Weight,n/2.0) * amdahlTerm;
      }

      tarch::la::Matrix<2,2,double> gradJgradJT;
      gradJgradJT = gradJ * tarch::la::transpose(gradJ);

      tarch::la::Vector<2,double>    rhs;
      rhs = -1.0 * (gradJ * y);


      if ( std::abs(det(gradJgradJT))>1e-5 ) {
        tarch::la::Vector<2,double>    shifts;
        shifts = tarch::la::invert( gradJgradJT ) * rhs;

        // Problem can be ill-posed
        if (
          !std::isnan(shifts(0)) &&
          !std::isnan(shifts(1))
        ) {

          _f   += shifts(0);
          _t_1 += shifts(1);
        }
      }

      // Problem can be ill-posed or too non-smooth, so we manually add
      // these constraints
      _f   = std::max(_f,MinF);
      _f   = std::min(_f,MaxF);
      _t_1 = std::max(_t_1,MinT1);
    }

    assertion4( _f>=0,    _f, _t_1, _p, _t );
    assertion4( _f<=1.0,  _f, _t_1, _p, _t );
    assertion4( _t_1>0.0, _f, _t_1, _p, _t );
  }

  #if defined(PerformanceAnalysis)
  logInfo( "relaxAmdahlsLaw()", "f=" << _f << ", t_1=" << _t_1 );
  #endif
}


void peano::performanceanalysis::SpeedupLaws::relaxAmdahlsLawWithThreadStartupCost() {
  if (_samples>Entries) {
    const int NewtonIterations = Entries;
    for (int it=0; it<NewtonIterations; it++) {
      tarch::la::Matrix<3,Entries,double>  gradJ(0.0);
      tarch::la::Vector<Entries,double>    y(0.0);

      for (int n=0; n<static_cast<int>(_p.size()); n++) {
        double amdahlTerm = _f * _t_1 + (1.0-_f) * _t_1/_p(n) + _s * _p(n)  - _t(n);
        double dtdf       =      _t_1 -            _t_1/_p(n);
        double dtdt_1     = _f        + (1.0-_f)      /_p(n);
        double dtds       = _p(n);

        gradJ(0,n) = std::pow(Weight,n/2.0) * dtdf;
        gradJ(1,n) = std::pow(Weight,n/2.0) * dtdt_1;
        gradJ(2,n) = std::pow(Weight,n/2.0) * dtds;
        y(n)       = std::pow(Weight,n/2.0) * amdahlTerm;
      }

      tarch::la::Matrix<3,3,double> gradJgradJT;
      gradJgradJT = gradJ * tarch::la::transpose(gradJ);

      tarch::la::Vector<3,double>    rhs;
      rhs = -1.0 * (gradJ * y);

      if ( std::abs(det(gradJgradJT))>1e-5 ) {
        tarch::la::Vector<3,double>    shifts;
        shifts = tarch::la::invert( gradJgradJT ) * rhs;

        // Problem can be ill-posed
        if (
          !std::isnan(shifts(0)) &&
          !std::isnan(shifts(1))
        ) {
          _f   += shifts(0);
          _t_1 += shifts(1);
          _s   += shifts(2);
        }
      }

      // Problem can be ill-posed or too non-smooth, so we manually add
      // these constraints
      _f   = std::max(   _f,MinF  );
      _f   = std::min(   _f,MaxF  );
      _t_1 = std::max( _t_1,MinT1 );
      _s   = std::max(   _s,MinS  );
    }

    assertion4( _f>=0,    _f, _t_1, _p, _t );
    assertion4( _f<=1.0,  _f, _t_1, _p, _t );
    assertion4( _t_1>0.0, _f, _t_1, _p, _t );
  }

  #if defined(PerformanceAnalysis)
  logInfo( "relaxAmdahlsLawWithThreadStartupCost()", "f=" << _f << ", t_1=" << _t_1 << ", s=" << _s );
  #endif
}


double peano::performanceanalysis::SpeedupLaws::getSerialTime() const {
  return _t_1;
}


double peano::performanceanalysis::SpeedupLaws::getSerialCodeFraction() const {
  return _f;
}


double peano::performanceanalysis::SpeedupLaws::getStartupCostPerThread() const {
  return _s;
}


std::string peano::performanceanalysis::SpeedupLaws::toString() const {
  std::ostringstream out;

  out << "(f="  << _f
      << ",t_1=" << _t_1
      << ",s="  << _s
      << ",s="  << _samples
      << ",t="  << _t
      << ",p="  << _p
      << ")";

  return out.str();
}


int peano::performanceanalysis::SpeedupLaws::getOptimalNumberOfThreads(
  int                  rankNumber,
  std::vector<double>  t_1,
  std::vector<double>  f,
  std::vector<double>  s,
  int                  totalThreadsAvailable,
  bool                 logResultingDistributionToInto,
  double               alpha,
  int                  m,
  double               eps
) {
  m=1;
  assertion(alpha>0.0);
  assertion(m>=1);
  assertionEquals(f.size(),t_1.size());
  assertionEquals(s.size(),t_1.size());
  assertion2(rankNumber<static_cast<int>( s.size() ), rankNumber, s.size() );

  std::vector<double>  c;
  std::vector<double>  M;
  std::vector<double>  M_kdc_k;

  for (int k=0; k<static_cast<int>(f.size()); k++) {
    c.push_back(1.0);
    M.push_back(0.0);
    M_kdc_k.push_back(0.0);
    logDebug( "getOptimalNumberOfThreads(...)", "c[" << k << "]=" << c[k] << ", t_1[k]=" << t_1[k] << ", f[k]=" << f[k] << ", s[k]=" << s[k] );
  }

  double maxM_kdc_k       = eps + 1.0;
  int    newtonIterations = 0;
  const int MaxNewtonIterations = 100;
  while (maxM_kdc_k>eps && newtonIterations<MaxNewtonIterations) {
    // Penalty term is the same for all components
    double penalty    = totalThreadsAvailable;
    for (int k=0; k<static_cast<int>(f.size()); k++) {
      penalty -= c[k];
    }

    for (int k=0; k<static_cast<int>(f.size()); k++) {
      const double amdahlTerm = f[k] * t_1[k] + (1.0-f[k])*t_1[k]/c[k] + s[k]*c[k];
      const double scalingOfAmdahlPow =  -(1.0-f[k])*t_1[k]/c[k]/c[k] + s[k];
      M[k] =
       std::pow(amdahlTerm,2.0 * m - 1.0) * scalingOfAmdahlPow - alpha * penalty;
      M_kdc_k[k] = 1.0 / (2*m - 1.0) *
       std::pow(amdahlTerm,2.0 * m - 2.0) * scalingOfAmdahlPow * scalingOfAmdahlPow +
       std::pow(amdahlTerm,2.0 * m - 1.0) * ( 2.0 * (1.0-f[k]) * t_1[k] / c[k] / c[k] / c[k] ) +
       alpha
       ;
    }

    // update c distribution and also determine termination criteration
    maxM_kdc_k = 0.0;
    for (int k=0; k<static_cast<int>(f.size()); k++) {
      if (std::abs(M_kdc_k[k])>eps/2.0) {
        double update = - M[k] / M_kdc_k[k];
        c[k]  += update / totalThreadsAvailable;
        c[k] = std::max(c[k],1.0);
        c[k] = std::min(c[k], totalThreadsAvailable-static_cast<int>(f.size())+1.0 );
      }
      maxM_kdc_k  = std::max( maxM_kdc_k, std::abs(M_kdc_k[k]) );

      logDebug( "getOptimalNumberOfThreads(...)", "c[" << k << "]=" << c[k] << ", M[k]=" << M[k] << ", M_kdc_k[k]=" << M_kdc_k[k] );
    }
    newtonIterations++;
  }

  if (logResultingDistributionToInto) {
    logInfo(
      "getOptimalNumberOfThreads(...)",
      "identified new optimal core distribution after " << newtonIterations <<
      " Newton iteration(s) reducing the error to " << maxM_kdc_k <<
      " (eps=" << eps << ")"
    );
    for (int k=0; k<static_cast<int>(f.size());k++) {
      logInfo(
        "getOptimalNumberOfThreads(...)", "c[" << k << "]=" << c[k] << "\t" <<
        "(t_1=" << t_1[k] <<
        ",f=" << f[k] <<
        ",s=" << s[k] << ")"
      );
    }
  }

  return std::floor(c[rankNumber]+0.5);
}
