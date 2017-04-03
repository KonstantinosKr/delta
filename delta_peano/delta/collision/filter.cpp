#include "delta/collision/filter.h"

void delta::collision::filterNewContacts(std::vector<contactpoint>& newContactPoints, double rA, double rB)
{ //search similarities in new contacts
	std::vector<contactpoint> tmp;

	for (unsigned i = 0; i < newContactPoints.size(); i++)
	{
		bool ignore = false;
		for (unsigned j = 0; j < tmp.size(); j++)
		{
			double sub[3];
			sub[0] = newContactPoints[i].x[0] - tmp[j].x[0];
			sub[1] = newContactPoints[i].x[1] - tmp[j].x[1];
			sub[2] = newContactPoints[i].x[2] - tmp[j].x[2];

			double distance = std::abs(sqrt(sub[0]*sub[0]+sub[1]*sub[1]+sub[2]*sub[2]));

			if (distance == 0)
			{
				ignore = true;
			}

			iREAL A[3], B[3];

			A[0] = newContactPoints[i].normal[0];
			A[1] = newContactPoints[i].normal[1];
			A[2] = newContactPoints[i].normal[2];

			B[0] = tmp[j].normal[0];
			B[1] = tmp[j].normal[1];
			B[2] = tmp[j].normal[2];

			double dot = A[0]*B[0]+
						 A[1]*B[1]+
						 A[2]*B[2];

			if (dot >= 0.70)
			{
				ignore = true;
			}
		}
		if(!ignore)
		{
			tmp.push_back(newContactPoints[i]);
		}
	}
	newContactPoints = tmp;
}

void delta::collision::filterOldContacts(std::vector<contactpoint>& dataStoredContactPoints, std::vector<contactpoint>& newContactPoints, double rA, double rB)
{
	std::vector<contactpoint> tmp;

	for (unsigned i = 0; i < newContactPoints.size(); i++)
	{
		bool ignore = false;
		for (unsigned j = 0; j < dataStoredContactPoints.size(); j++)
		{
			double sub[3];
			sub[0] = newContactPoints[i].x[0] - dataStoredContactPoints[j].x[0];
			sub[1] = newContactPoints[i].x[1] - dataStoredContactPoints[j].x[1];
			sub[2] = newContactPoints[i].x[2] - dataStoredContactPoints[j].x[2];

			double distance =  std::abs(sqrt(sub[0]*sub[0]+sub[1]*sub[1]+sub[2]*sub[2]));

			//double SA = sqrt((rA*2)-(((rA-((dataStoredContactPoints[i].depth/2)*2)))*2));
			//double SB = sqrt((rB*2)-(((rB-((dataStoredContactPoints[i].depth/2)*2)))*2));

			//double hMin = std::min(SA,SB);
			//printf("rA:%f depth:%f hMin:%f\n", rA, dataStoredContactPoints[j].depth, hMin);
			if(distance == 0)
			{
				ignore = true;
			}
/*
			iREAL A[3], B[3];
			A[0] = newContactPoints[i].normal[0];
			A[1] = newContactPoints[i].normal[1];
			A[2] = newContactPoints[i].normal[2];

			B[0] = dataStoredContactPoints[j].normal[0];
			B[1] = dataStoredContactPoints[j].normal[1];
			B[2] = dataStoredContactPoints[j].normal[2];

			double dot = A[0]*B[0]+
						 A[1]*B[1]+
						 A[2]*B[2];*/
		}
		if(!ignore)
		{
			tmp.push_back(newContactPoints[i]);
		}
	}
	newContactPoints = tmp;
}

void delta::collision::filterNewContacts(std::vector<contactpoint>& newContactPoints)
{//filter multiples within new contacts
	std::vector<contactpoint> tmpContactPoints;

	//search similarities in new contacts
	for(auto i:newContactPoints)
	{
		bool ignore = false;//found or ignore
		for (auto j:tmpContactPoints)
		{
			double sub[] = {i.x[0] - j.x[0], i.x[1] - j.x[1], i.x[2] - j.x[2]};
			double distance = std::abs(sqrt(sub[0]*sub[0]+sub[1]*sub[1]+sub[2]*sub[2]));
			if(distance == 0) ignore = true;
		}

		if(!ignore) tmpContactPoints.push_back(i);
	}
	newContactPoints = tmpContactPoints;
}

void delta::collision::filterOldContacts(std::vector<contactpoint>& dataStoredContactPoints, std::vector<contactpoint>& newContactPoints, double hMin)
{
	std::vector<contactpoint> tmpContactPoints;
	//search similarities in new and stored contacts
	for (unsigned i = 0; i < newContactPoints.size(); i++)//loop through stored
	{
		bool ignore = false;//found or ignore
		for (unsigned j = 0; j < dataStoredContactPoints.size(); j++)//loop through new, if similar then don't write in new-new contacts
		{
			double sub[3];
			sub[0] = newContactPoints[i].x[0] - dataStoredContactPoints[j].x[0];
			sub[1] = newContactPoints[i].x[1] - dataStoredContactPoints[j].x[1];
			sub[2] = newContactPoints[i].x[2] - dataStoredContactPoints[j].x[2];

			double distance = std::abs(sqrt(sub[0]*sub[0]+sub[1]*sub[1]+sub[2]*sub[2]));
			if (distance == 0)
			{
				ignore = true;
			}
		}

		if(!ignore)
		{
			tmpContactPoints.push_back(newContactPoints[i]);
		}
	}
	newContactPoints = tmpContactPoints;
}
