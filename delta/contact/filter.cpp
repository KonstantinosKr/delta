#include "filter.h"

void delta::contact::filterOldContacts(
    std::vector<contactpoint>& dataStoredContactPoints,
    std::vector<contactpoint>& newContactPoints,
    iREAL rA,
    iREAL rB)
{
  std::vector<contactpoint> tmp;

  for (unsigned i = 0; i < newContactPoints.size(); i++)
  {
	bool ignore = false;
	for (unsigned j = 0; j < dataStoredContactPoints.size(); j++)
	{
	  iREAL A = newContactPoints[i].x[0] - dataStoredContactPoints[j].x[0];
	  iREAL B = newContactPoints[i].x[1] - dataStoredContactPoints[j].x[1];
	  iREAL C = newContactPoints[i].x[2] - dataStoredContactPoints[j].x[2];

	  iREAL eps = 0.0001;
	  if(std::abs(A) < eps && std::abs(B) < eps && std::abs(C) < eps) ignore = true;
	  //iREAL SA = sqrt((rA*2)-(((rA-((dataStoredContactPoints[i].depth/2)*2)))*2));
	  //iREAL SB = sqrt((rB*2)-(((rB-((dataStoredContactPoints[i].depth/2)*2)))*2));

	  //iREAL hMin = std::min(SA,SB);
	  //printf("rA:%f depth:%f hMin:%f\n", rA, dataStoredContactPoints[j].depth, hMin);
	}
	if(!ignore) tmp.push_back(newContactPoints[i]);
  }
  newContactPoints = tmp;
}

void delta::contact::filterOldContacts(
    std::vector<contactpoint>& dataStoredContactPoints,
    std::vector<contactpoint>& newContactPoints)
{
  std::vector<contactpoint> tmp;

  for (unsigned i = 0; i < newContactPoints.size(); i++)
  {
    bool ignore = false;
    for (unsigned j = 0; j < dataStoredContactPoints.size(); j++)
    {
      iREAL A = newContactPoints[i].x[0] - dataStoredContactPoints[j].x[0];
      iREAL B = newContactPoints[i].x[1] - dataStoredContactPoints[j].x[1];
      iREAL C = newContactPoints[i].x[2] - dataStoredContactPoints[j].x[2];

      iREAL eps = 0.0001;
      if(std::abs(A) < eps && std::abs(B) < eps && std::abs(C) < eps) ignore = true;
    }
    if(!ignore) tmp.push_back(newContactPoints[i]);
  }
  newContactPoints = tmp;
}

void delta::contact::filterOldContacts(
    std::vector<contactpoint>& dataStoredContactPoints,
    std::vector<contactpoint>& newContactPoints,
    iREAL hMin)
{
  std::vector<contactpoint> tmpContactPoints;
  //search similarities in new and stored contacts
  for (unsigned i = 0; i < newContactPoints.size(); i++)//loop through stored
  {
	bool ignore = false;//found or ignore
	for (unsigned j = 0; j < dataStoredContactPoints.size(); j++)//loop through new, if similar then don't write in new-new contacts
	{
	  iREAL A = newContactPoints[i].x[0] - dataStoredContactPoints[j].x[0];
	  iREAL B = newContactPoints[i].x[1] - dataStoredContactPoints[j].x[1];
	  iREAL C = newContactPoints[i].x[2] - dataStoredContactPoints[j].x[2];

	  iREAL eps = 0.0001;
	  if(std::abs(A) < eps && std::abs(B) < eps && std::abs(C) < eps) ignore = true;
	}
	if(!ignore) tmpContactPoints.push_back(newContactPoints[i]);
  }
  newContactPoints = tmpContactPoints;
}

void delta::contact::filterNewContacts(std::vector<contactpoint>& newContactPoints)
{//filter multiples within new contacts
  std::vector<contactpoint> tmpContactPoints;

  //search similarities in new contacts
  for(auto i:newContactPoints)
  {
    bool ignore = false;//found or ignore
    for (auto j:tmpContactPoints)
    {
      iREAL A = i.x[0] - j.x[0];
      iREAL B = i.x[1] - j.x[1];
      iREAL C = i.x[2] - j.x[2];

      //iREAL distance = std::abs(sqrt(sub[0]*sub[0]+sub[1]*sub[1]+sub[2]*sub[2]));
      iREAL eps = 0.0001;
      if(std::abs(A) < eps && std::abs(B) < eps && std::abs(C) < eps) ignore = true;
    }
    if(!ignore) tmpContactPoints.push_back(i);
  }
  newContactPoints = tmpContactPoints;
}
