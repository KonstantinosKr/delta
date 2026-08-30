#include "tarch/services/ServiceRepository.h"
#include "tarch/Assertions.h"
#include "tarch/multicore/RecursiveLock.h"


#include <sstream>


tarch::services::ServiceRepository::ServiceRepository():
  _services() {
}


tarch::services::ServiceRepository::~ServiceRepository() {
  _services.clear();
}


tarch::services::ServiceRepository& tarch::services::ServiceRepository::getInstance() {
  static tarch::services::ServiceRepository singleton;
  return singleton;
}


void tarch::services::ServiceRepository::addService( Service* const service, const std::string& name ) {
  assertion2( service!=0, name, getListOfRegisteredServices() );
  assertion2( !name.empty(), name, getListOfRegisteredServices() );
  assertion2( !hasService(service), name, getListOfRegisteredServices() );

  ServiceEntry entry;
  entry._name    = name;
  entry._service = service;
  _services.push_back( entry );
}


bool tarch::services::ServiceRepository::hasService( Service* service ) const {
  for (
      ServiceContainer::const_iterator p = _services.begin();
      p != _services.end();
      p++
  ) {
    if (p->_service==service) {
      return true;
    }
  }
  return false;
}


void tarch::services::ServiceRepository::receiveDanglingMessages() {
  tarch::multicore::RecursiveLock lock(Service::receiveDanglingMessagesSemaphore);

  for (
      ServiceContainer::iterator p = _services.begin();
      p != _services.end();
      p++
  ) {
    p->_service->receiveDanglingMessages();
  }
}


std::string tarch::services::ServiceRepository::getListOfRegisteredServices() const {
  std::ostringstream result;
  for (
      ServiceContainer::const_iterator p = _services.begin();
      p != _services.end();
      p++
  ) {
    result << " " << p->_name;
  }
  return result.str();
}

