/*
 * write.h
 *
 *  Created on: 2 Apr 2017
 *      Author: konstantinos
 */

#ifndef DELTA_IO_WRITE_H_
#define DELTA_IO_WRITE_H_

#include <vector>
#include <string>
#include "stdio.h"
#include "string.h"


#include "../../core/data/ParticleRecord.h"
#include "../../geometry/structure/Mesh.h"
#include "../../world/structure/Object.h"

#include <vtkSmartPointer.h>
#include <vtkTetra.h>
#include <vtkLine.h>
#include <vtkCellArray.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkDataSetMapper.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>

namespace delta {
  namespace core {
	namespace io {

	void writeGeometryToVTKVTK(
		std::string 									path,
		int 											step,
		std::vector<delta::core::data::ParticleRecord>& geometries);

	void writeGridGeometryToVTKVTK(
		std::string 						path,
		int 								step,
		std::vector<std::array<iREAL, 6>> 	boundary);

	void writeGeometryToVTK(
		std::string										path,
		int 											step,
		std::vector<delta::core::data::ParticleRecord>& geometries);

	void writeGridGeometryToVTK(
		std::string							path,
		int 								step,
		std::vector<std::array<iREAL, 6>> 	boundary);

	  void writeScenarioSpecification(std::string 	fileName);
	}
  }
}

#endif /* DELTA_CORE_WRITE_H_ */
