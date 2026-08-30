#ifndef MYSCOREP_H
#define MYSCOREP_H

#if defined(UseScoreP)

#include <scorep/SCOREP_User.h>

#else


#define SCOREP_RECORDING_ON()
#define SCOREP_RECORDING_OFF()
#define SCOREP_RECORDING_IS_ON() 0
#define SCOREP_USER_REGION( name, type )



#define SCOREP_USER_REGION_DEFINE( handle )
#define SCOREP_USER_OA_PHASE_BEGIN( handle, name, type  )
#define SCOREP_USER_OA_PHASE_END( handle )
#define SCOREP_USER_REGION_BEGIN( handle, name, type )
#define SCOREP_USER_REGION_INIT( handle, name, type )
#define SCOREP_USER_REGION_END( handle )
#define SCOREP_USER_REGION_ENTER( handle )
#define SCOREP_USER_FUNC_BEGIN()
#define SCOREP_USER_FUNC_END()
#define SCOREP_GLOBAL_REGION_DEFINE( handle )
#define SCOREP_GLOBAL_REGION_EXTERNAL( handle )
#define SCOREP_USER_PARAMETER_INT64( name, value )
#define SCOREP_USER_PARAMETER_UINT64( name, value )
#define SCOREP_USER_PARAMETER_STRING( name, value )
#define SCOREP_USER_METRIC_GLOBAL( metricHandle )
#define SCOREP_USER_METRIC_EXTERNAL( metricHandle )
#define SCOREP_USER_METRIC_LOCAL( metricHandle )
#define SCOREP_USER_METRIC_INIT( metricHandle, name, unit, type, context )
#define SCOREP_USER_METRIC_INT64( metricHandle, value )
#define SCOREP_USER_METRIC_UINT64( metricHandle, value )
#define SCOREP_USER_METRIC_DOUBLE( metricHandle, value )

#endif

#endif // MYSCOREP_H
