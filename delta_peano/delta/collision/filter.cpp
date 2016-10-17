#include "delta/collision/filter.h"

void delta::collision::filter(std::vector<contactpoint>& dataStoredContactPoints, std::vector<contactpoint>& newContactPoints, double hMin )
{
	std::vector<contactpoint> tmp = newContactPoints;

	//search similarities in new contacts
	for (int i = 0 ; i < newContactPoints.size(); i++)
	{
		for (int j = i+1 ; j < tmp.size(); j++)
		{
			double ax[3], bx[3], sub[3];
			ax[0] = newContactPoints[i].x[0];
			ax[1] = newContactPoints[i].x[1];
			ax[2] = newContactPoints[i].x[2];

			bx[0] = tmp[j].x[0];
			bx[1] = tmp[j].x[1];
			bx[2] = tmp[j].x[2];

			sub[0] = bx[0] - ax[0];
			sub[1] = bx[1] - ax[1];
			sub[2] = bx[2] - ax[2];

			double distance = sqrt(sub[0]*sub[0]+sub[1]*sub[1]+sub[2]*sub[2]);

			//look how parallel they are
			double parallel = sqrt(ax[0]*bx[0]+ax[1]*bx[1]+ax[2]*bx[2]);

			if (distance <= 0 || distance <= hMin)
			{
				//	printf("PARALLEL:%f\n", parallel);
				//printf("hMIN:%f, distance:%f\n", hMin, distance);
				newContactPoints.erase(newContactPoints.begin()+i);
				j = tmp.size();

			}
		}
	}

	//search similarities between new contacts and already stored
	for (int i = 0 ; i < newContactPoints.size(); i++)
	{
		for (int j = 0 ; j < dataStoredContactPoints.size(); j++)
		{
			double ax[3], bx[3], sub[3];
			ax[0] = newContactPoints[i].x[0];
			ax[1] = newContactPoints[i].x[1];
			ax[2] = newContactPoints[i].x[2];

			bx[0] = dataStoredContactPoints[j].x[0];
			bx[1] = dataStoredContactPoints[j].x[1];
			bx[2] = dataStoredContactPoints[j].x[2];

			sub[0] = bx[0] - ax[0];
			sub[1] = bx[1] - ax[1];
			sub[2] = bx[2] - ax[2];

			double distance = sqrt(sub[0]*sub[0]+sub[1]*sub[1]+sub[2]*sub[2]);
			if (distance < hMin)
			{
				newContactPoints.erase(newContactPoints.begin()+i);
				j = dataStoredContactPoints.size();
			}
		}
	}
}
