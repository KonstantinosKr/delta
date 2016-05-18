// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_SERVICES_SERVICE_REPOSITORY_H_
#define _TARCH_SERVICES_SERVICE_REPOSITORY_H_

#ifdef Parallel
#include <mpi.h>
#endif
#include "tarch/services/Service.h"

namespace tarch {
  namespace services {
    class ServiceRepository;
  }
}

#include <vector>
#include <string>


/**
 * Service Repository
 *
 * See the interface Service for a detailed description. The registry is a
 * service itself, but it does not register as a service.
 *
 * @author Tobias Weinzierl
 */
class tarch::services::ServiceRepository: public tarch::services::Service {
  private:
    struct ServiceEntry {
      std::string  _name;
      Service*     _service;
    };

    typedef  std::vector<ServiceEntry>      ServiceContainer;

    ServiceContainer         _services;

    ServiceRepository();
  public:
    virtual ~ServiceRepository();

    static ServiceRepository& getInstance();

    /**
     * @param service Pointer to service
     * @param name    Name of service (mandatory, not empty)
     */
    void addService( Service* const service, const std::string& name );
    bool hasService( Service* service ) const;

    /**
     * Answer to MPI Messages
     *
     * Tell all registered services to answer to MPI messages that are still
     * pending in the MPI queues.
     */
    virtual void receiveDanglingMessages();

    /**
     * @return List of registered services separated by whitespaces
     */
    std::string getListOfRegisteredServices() const;
};


#endif
