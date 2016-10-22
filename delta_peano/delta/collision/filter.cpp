#include "delta/collision/filter.h"

void delta::collision::filterNewContacts(std::vector<contactpoint>& newContactPoints, double hMin, double rA, double rB)
{
	std::vector<contactpoint> tmp = newContactPoints;
    std::vector<int> duplicates;
    int deleted = 0;

	//search similarities in new contacts
	for (int i = 0; i < tmp.size(); i++)
	{
		for (int j = 0; j < tmp.size(); j++)
		{
			if( i == j) continue;
			double sub[3];
			sub[0] = tmp[j].x[0] - tmp[i].x[0];
			sub[1] = tmp[j].x[1] - tmp[i].x[1];
			sub[2] = tmp[j].x[2] - tmp[i].x[2];

			double distance = std::abs(sqrt(sub[0]*sub[0]+sub[1]*sub[1]+sub[2]*sub[2]));

			double SA = sqrt((rA*2)-(((rA-((tmp[j].depth/2)*2)))*2));
			double SB = sqrt((rB*2)-(((rB-((tmp[j].depth/2)*2)))*2));

			hMin = std::min(SA,SB);

			//printf("rA:%f depth:%f hMin:%f distance:%f\n", rA, tmp[j].depth, hMin, distance);
			if (distance == 0)
			{
				//printf("delete:%d\n", i);
				newContactPoints.erase(newContactPoints.begin()+(i-deleted));
				deleted++;
				break;
			}

			if (distance <= 0.01 && (tmp[i].getDistance() > tmp[j].getDistance()))
			{
				//printf("hMIN:%f\n", hMin);
				newContactPoints.erase(newContactPoints.begin()+(i-deleted));
				deleted++;
				break;
			}
		}
	}
}

void delta::collision::filterOldContacts(std::vector<contactpoint>& dataStoredContactPoints, std::vector<contactpoint>& newContactPoints, double hMin, double rA, double rB)
{
	int deleted = 0;
	//search similarities between new contacts and already stored
	for (int i = 0; i < dataStoredContactPoints.size(); i++)
	{
		for (int j = 0; j < newContactPoints.size(); j++)
		{
			double sub[3];
			sub[0] = newContactPoints[j].x[0] - dataStoredContactPoints[i].x[0];
			sub[1] = newContactPoints[j].x[1] - dataStoredContactPoints[i].x[1];
			sub[2] = newContactPoints[j].x[2] - dataStoredContactPoints[i].x[2];

			double distance =  std::abs(sqrt(sub[0]*sub[0]+sub[1]*sub[1]+sub[2]*sub[2]));

			double SA = sqrt((rA*2)-(((rA-((dataStoredContactPoints[i].depth/2)*2)))*2));
			double SB = sqrt((rB*2)-(((rB-((dataStoredContactPoints[i].depth/2)*2)))*2));

			hMin = std::min(SA,SB);
			//printf("rA:%f depth:%f hMin:%f\n", rA, dataStoredContactPoints[j].depth, hMin);
			if(distance == 0)
			{
				//printf("hMIN:%f, distance:%f\n", hMin, distance);
				newContactPoints.erase(newContactPoints.begin()+j);
				break;
			}

			//distance between contacts points is smaller than hMin | new contact/body distance is smaller than the one stored  | delete stored
			if (distance <= 0.01 && (newContactPoints[j].getDistance() < dataStoredContactPoints[i].getDistance()))
			{
				dataStoredContactPoints.erase(dataStoredContactPoints.begin()+(i-deleted));
				break;
			} else if(distance <= 0.01 && (newContactPoints[j].getDistance() > dataStoredContactPoints[i].getDistance()))
			{
				newContactPoints.erase(newContactPoints.begin()+j);
				break;
			}
		}
	}
}

void delta::collision::filterNewContacts(std::vector<contactpoint>& newContactPoints, double hMin)
{
	std::vector<contactpoint> tmp = newContactPoints;
    int deleted = 0;

	//search similarities in new contacts
	for (int i = 0; i < tmp.size(); i++)
	{
		for (int j = 0; j < tmp.size(); j++)
		{
			if( i == j) continue;
			double sub[3];
			sub[0] = tmp[j].x[0] - tmp[i].x[0];
			sub[1] = tmp[j].x[1] - tmp[i].x[1];
			sub[2] = tmp[j].x[2] - tmp[i].x[2];

			double distance = std::abs(sqrt(sub[0]*sub[0]+sub[1]*sub[1]+sub[2]*sub[2]));

			if (distance == 0)
			{
				newContactPoints.erase(newContactPoints.begin()+(i-deleted));
				deleted++;
				break;
			}

			if (distance <= hMin && (tmp[i].getDistance() > tmp[j].getDistance()))
			{
				newContactPoints.erase(newContactPoints.begin()+(i-deleted));
				deleted++;
				break;
			}
		}
	}
}

void delta::collision::filterOldContacts(std::vector<contactpoint>& dataStoredContactPoints, std::vector<contactpoint>& newContactPoints, double hMin)
{
	int deleted = 0;
	//search similarities between new contacts and already stored
	for (int i = 0; i < dataStoredContactPoints.size(); i++)
	{
		for (int j = 0; j < newContactPoints.size(); j++)
		{
			double sub[3];
			sub[0] = newContactPoints[j].x[0] - dataStoredContactPoints[i].x[0];
			sub[1] = newContactPoints[j].x[1] - dataStoredContactPoints[i].x[1];
			sub[2] = newContactPoints[j].x[2] - dataStoredContactPoints[i].x[2];

			double distance =  std::abs(sqrt(sub[0]*sub[0]+sub[1]*sub[1]+sub[2]*sub[2]));
			//printf("rA:%f depth:%f hMin:%f\n", rA, dataStoredContactPoints[j].depth, hMin);
			if(distance == 0)
			{
				//printf("hMIN:%f, distance:%f\n", hMin, distance);
				newContactPoints.erase(newContactPoints.begin()+j);
				break;
			}

			//distance between contacts points is smaller than hMin | new contact/body distance is smaller than the one stored  | delete stored
			if (distance <= hMin && (newContactPoints[j].getDistance() < dataStoredContactPoints[i].getDistance()))
			{
				dataStoredContactPoints.erase(dataStoredContactPoints.begin()+(i-deleted));
				deleted++;
				break;
			} else if(distance <= hMin && (newContactPoints[j].getDistance() > dataStoredContactPoints[i].getDistance()))
			{
				newContactPoints.erase(newContactPoints.begin()+j);
				break;
			}
		}
	}
}
