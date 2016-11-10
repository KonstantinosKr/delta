#include "forces.h"

//particle parameters
#define SPRING 1E5
#define DAMPER 1
#define FRICTION 0.5

//sphere parameters that work
#define SSPRING 1E6
#define SDAMPER 0.2
#define SFRICTION 0.3
#define SFRICTIONROLLING 0.005

#define NNMUL(A, B, C)\
{\
 (C) [0] = (A)[0]*(B)[0]+(A)[3]*(B)[1]+(A)[6]*(B)[2];\
 (C) [1] = (A)[1]*(B)[0]+(A)[4]*(B)[1]+(A)[7]*(B)[2];\
 (C) [2] = (A)[2]*(B)[0]+(A)[5]*(B)[1]+(A)[8]*(B)[2];\
 (C) [3] = (A)[0]*(B)[3]+(A)[3]*(B)[4]+(A)[6]*(B)[5];\
 (C) [4] = (A)[1]*(B)[3]+(A)[4]*(B)[4]+(A)[7]*(B)[5];\
 (C) [5] = (A)[2]*(B)[3]+(A)[5]*(B)[4]+(A)[8]*(B)[5];\
 (C) [6] = (A)[0]*(B)[6]+(A)[3]*(B)[7]+(A)[6]*(B)[8];\
 (C) [7] = (A)[1]*(B)[6]+(A)[4]*(B)[7]+(A)[7]*(B)[8];\
 (C) [8] = (A)[2]*(B)[6]+(A)[5]*(B)[7]+(A)[8]*(B)[8];\
}

double delta::forces::spring(iREAL normal[3], iREAL conpnt[3], iREAL depth, iREAL relativeVelocity[3],
							iREAL positionASpatial[3], iREAL positionBSpatial[3], iREAL positionAReferential[3], iREAL positionBReferential[3], iREAL massA, iREAL massB,
							iREAL rotationA[9], iREAL rotationB[9], iREAL inverseA[9], iREAL inverseB[9], iREAL f[3])
{
	//RefConPoint = Rotation^T *(spatial contact point - spatial centre) + RefCentre;
	iREAL refconptA[3], refconptB[3], conptSubPosition[3];

	conptSubPosition[0] = conpnt[0] - positionASpatial[0];
	conptSubPosition[1] = conpnt[1] - positionASpatial[1];
	conptSubPosition[2] = conpnt[2] - positionASpatial[2];

	refconptA[0] = (conptSubPosition[0]*rotationA[0] + conptSubPosition[1]*rotationA[1] + conptSubPosition[2]*rotationA[2])+positionAReferential[0];
	refconptA[1] = (conptSubPosition[0]*rotationA[3] + conptSubPosition[1]*rotationA[4] + conptSubPosition[2]*rotationA[5])+positionAReferential[1];
	refconptA[2] = (conptSubPosition[0]*rotationA[6] + conptSubPosition[1]*rotationA[7] + conptSubPosition[2]*rotationA[8])+positionAReferential[2];

	conptSubPosition[0] = conpnt[0] - positionBSpatial[0];
	conptSubPosition[1] = conpnt[1] - positionBSpatial[1];
	conptSubPosition[2] = conpnt[2] - positionBSpatial[2];

	refconptB[0] = (conptSubPosition[0]*rotationB[0] + conptSubPosition[1]*rotationB[1] + conptSubPosition[2]*rotationB[2])+positionBReferential[0];
	refconptB[1] = (conptSubPosition[0]*rotationB[3] + conptSubPosition[1]*rotationB[4] + conptSubPosition[2]*rotationB[5])+positionBReferential[1];
	refconptB[2] = (conptSubPosition[0]*rotationB[6] + conptSubPosition[1]*rotationB[7] + conptSubPosition[2]*rotationB[8])+positionBReferential[2];

	//H_N is a 1x6 matrix; n^T is a 1x3 "matrix" (normal vector transposed);
	//[Rotation*(RefCentre-RefConPnt)   Identity] is a 3x6 matrix; 1x3 * 3x6 = 1x6;
	iREAL rPositionContactPnti[9];//3x3
	iREAL rPositionContactPntj[9];

	rPositionContactPnti[0] = 0;										rPositionContactPnti[3] = -positionAReferential[2]-refconptA[2];rPositionContactPnti[6] = positionAReferential[1]-refconptA[1];
	rPositionContactPnti[1] = positionAReferential[2]-refconptA[2];		rPositionContactPnti[4] = 0;									rPositionContactPnti[7] = -positionAReferential[0]-refconptA[0];
	rPositionContactPnti[2] = -positionAReferential[1]-refconptA[1];	rPositionContactPnti[5] = positionAReferential[0]-refconptA[0];	rPositionContactPnti[8] = 0;

	rPositionContactPntj[0] = 0;										rPositionContactPntj[3] = -positionBReferential[2]-refconptB[2];rPositionContactPntj[6] = positionBReferential[1]-refconptB[1];
	rPositionContactPntj[1] = positionBReferential[2]-refconptB[2];		rPositionContactPntj[4] = 0;									rPositionContactPntj[7] = -positionBReferential[0]-refconptB[0];
	rPositionContactPntj[2] = -positionBReferential[1]-refconptB[1];	rPositionContactPntj[5] = positionBReferential[0]-refconptB[0];	rPositionContactPntj[8] = 0;

	iREAL RIi[18];//[Rotation*(RefCentre-RefConPnt)   Identity]
	iREAL RIj[18];

	NNMUL(rotationA, rPositionContactPnti, RIi);

	RIi[9] = 1;		RIi[12] = 0;	RIi[15] = 0;
	RIi[10] = 0;	RIi[13] = 1;	RIi[16] = 0;
	RIi[11] = 0;	RIi[14] = 0;	RIi[17] = 1;

	NNMUL(rotationB, rPositionContactPntj, RIi);

	RIj[9] = 1;		RIj[12] = 0;	RIj[15] = 0;
	RIj[10] = 0;	RIj[13] = 1;	RIj[16] = 0;
	RIj[11] = 0;	RIj[14] = 0;	RIj[17] = 1;

	//H_N is a 1x6 matrix; n^T is a 1x3 "matrix" (normal vector transposed);
	//[Rotation*(RefCentre-RefConPnt)   Identity] is a 3x6 matrix; 1x3 * 3x6 = 1x6;
	iREAL Hi_n[6];
	iREAL Hj_n[6];

	//RIi[0];RIi[3];RIi[6];RIi[9];RIi[12];RIi[15];
	//RIi[1];RIi[4];RIi[7];RIi[10];RIi[13];RIi[16];
	//RIi[2];RIi[5];RIi[8];RIi[11];RIi[14];RIi[17];

	Hi_n[0] = normal[0]*RIi[0] + normal[1]*RIi[1] + normal[2]*RIi[2];
	Hi_n[1] = normal[0]*RIi[3] + normal[1]*RIi[4] + normal[2]*RIi[5];
	Hi_n[2] = normal[0]*RIi[6] + normal[1]*RIi[7] + normal[2]*RIi[8];
	Hi_n[3] = normal[0]*RIi[9] + normal[1]*RIi[10] + normal[2]*RIi[11];
	Hi_n[4] = normal[0]*RIi[12] + normal[1]*RIi[13] + normal[2]*RIi[14];
	Hi_n[5] = normal[0]*RIi[15] + normal[1]*RIi[16] + normal[2]*RIi[17];

	Hj_n[0] = normal[0]*RIj[0] + normal[1]*RIj[1] + normal[2]*RIj[2];
	Hj_n[1] = normal[0]*RIj[3] + normal[1]*RIj[4] + normal[2]*RIj[5];
	Hj_n[2] = normal[0]*RIj[6] + normal[1]*RIj[7] + normal[2]*RIj[8];
	Hj_n[3] = normal[0]*RIj[9] + normal[1]*RIj[10] + normal[2]*RIj[11];
	Hj_n[4] = normal[0]*RIj[12] + normal[1]*RIj[13] + normal[2]*RIj[14];
	Hj_n[5] = normal[0]*RIj[15] + normal[1]*RIj[16] + normal[2]*RIj[17];

	iREAL ui[36];
	ui[0] = inverseA[0]; ui[6] = inverseA[3];	ui[12] = inverseA[6];	ui[18] = 0;			ui[24] = 0; 		ui[30] = 0;
	ui[1] = inverseA[1]; ui[7] = inverseA[4];	ui[13] = inverseA[7];	ui[19] = 0;			ui[25] = 0; 		ui[31] = 0;
	ui[2] = inverseA[2]; ui[8] = inverseA[5];	ui[14] = inverseA[8];	ui[20] = 0;   		ui[26] = 0; 		ui[32] = 0;
	ui[3] = 0; 		   	 ui[9] = 0;				ui[15] = 0;				ui[21] = (1/massA); ui[27] = 0; 		ui[33] = 0;
	ui[4] = 0;		     ui[10] = 0;			ui[16] = 0;				ui[22] = 0;   		ui[28] = (1/massA); ui[34] = 0;
	ui[5] = 0;		     ui[11] = 0;			ui[17] = 0;				ui[23] = 0;   		ui[29] = 0; 		ui[35] = (1/massA);

	iREAL uj[36];
	uj[0] = inverseB[0]; uj[6] = inverseB[3];	uj[12] = inverseB[6];	uj[18] = 0;			uj[24] = 0; 		uj[30] = 0;
	uj[1] = inverseB[1]; uj[7] = inverseB[4];	uj[13] = inverseB[7];	uj[19] = 0;			uj[25] = 0; 		uj[31] = 0;
	uj[2] = inverseB[2]; uj[8] = inverseB[5];	uj[14] = inverseB[8];	uj[20] = 0;   		uj[26] = 0; 		uj[32] = 0;
	uj[3] = 0; 		   	 uj[9] = 0;				uj[15] = 0;				uj[21] = (1/massB); uj[27] = 0; 		uj[33] = 0;
	uj[4] = 0;		     uj[10] = 0;			uj[16] = 0;				uj[22] = 0;   		uj[28] = (1/massB); uj[34] = 0;
	uj[5] = 0;		     uj[11] = 0;			uj[17] = 0;				uj[23] = 0;   		uj[29] = 0; 		uj[35] = (1/massB);

	//H_N (1x6) * [] (6x6)
	iREAL Hi[6], Hj[6];

	Hi[0] = Hi_n[0]*ui[0] + Hi_n[1]*ui[1] + Hi_n[2]*ui[2] + Hi_n[3]*ui[3] + Hi_n[4]*ui[4] + Hi_n[5]*ui[5];
	Hi[1] = Hi_n[0]*ui[6] + Hi_n[1]*ui[7] + Hi_n[2]*ui[8] + Hi_n[3]*ui[9] + Hi_n[4]*ui[10] + Hi_n[5]*ui[11];
	Hi[2] = Hi_n[0]*ui[12] + Hi_n[1]*ui[13] + Hi_n[2]*ui[14] + Hi_n[3]*ui[15] + Hi_n[4]*ui[16] + Hi_n[5]*ui[17];
	Hi[3] = Hi_n[0]*ui[18] + Hi_n[1]*ui[19] + Hi_n[2]*ui[20] + Hi_n[3]*ui[21] + Hi_n[4]*ui[22] + Hi_n[5]*ui[23];
	Hi[4] = Hi_n[0]*ui[24] + Hi_n[1]*ui[25] + Hi_n[2]*ui[26] + Hi_n[3]*ui[27] + Hi_n[4]*ui[28] + Hi_n[5]*ui[29];
	Hi[5] = Hi_n[0]*ui[30] + Hi_n[1]*ui[31] + Hi_n[2]*ui[32] + Hi_n[3]*ui[33] + Hi_n[4]*ui[34] + Hi_n[5]*ui[35];

	Hj[0] = Hj_n[0]*uj[0] + Hj_n[1]*uj[1] + Hj_n[2]*uj[2] + Hj_n[3]*uj[3] + Hj_n[4]*uj[4] + Hj_n[5]*uj[5];
	Hj[1] = Hj_n[0]*uj[6] + Hj_n[1]*uj[7] + Hj_n[2]*uj[8] + Hj_n[3]*uj[9] + Hj_n[4]*uj[10] + Hj_n[5]*uj[11];
	Hj[2] = Hj_n[0]*uj[12] + Hj_n[1]*uj[13] + Hj_n[2]*uj[14] + Hj_n[3]*uj[15] + Hj_n[4]*uj[16] + Hj_n[5]*uj[17];
	Hj[3] = Hj_n[0]*uj[18] + Hj_n[1]*uj[19] + Hj_n[2]*uj[20] + Hj_n[3]*uj[21] + Hj_n[4]*uj[22] + Hj_n[5]*uj[23];
	Hj[4] = Hj_n[0]*uj[24] + Hj_n[1]*uj[25] + Hj_n[2]*uj[26] + Hj_n[3]*uj[27] + Hj_n[4]*uj[28] + Hj_n[5]*uj[29];
	Hj[5] = Hj_n[0]*uj[30] + Hj_n[1]*uj[31] + Hj_n[2]*uj[32] + Hj_n[3]*uj[33] + Hj_n[4]*uj[34] + Hj_n[5]*uj[35];

	iREAL W_NN = (Hi[0]*Hi_n[0] + Hi[1]*Hi_n[1] + Hi[2]*Hi_n[2] + Hi[3]*Hi_n[3] + Hi[4]*Hi_n[4] + Hi[5]*Hi_n[5])
			   + (Hj[0]*Hj_n[0] + Hj[1]*Hj_n[1] + Hj[2]*Hj_n[2] + Hj[3]*Hj_n[3] + Hj[4]*Hj_n[4] + Hj[5]*Hj_n[5]);

  iREAL mass = 1.0/W_NN;

  iREAL velocity = (relativeVelocity[0]*normal[0]) + (relativeVelocity[1]*normal[1]) + (relativeVelocity[2]*normal[2]);

  iREAL damp = DAMPER * 50 * sqrt(mass*SPRING)*velocity;

  iREAL force = SPRING*depth+damp;

  f[0] = force*normal[0];
  f[1] = force*normal[1];
  f[2] = force*normal[2];

  printf("Velocity:%f depth:%f spring:%f totalforce:%f damp:%f mass:%f\n", velocity, depth, SPRING*depth, force, damp, mass);
  return force;
}

void delta::forces::friction(iREAL normal[3], iREAL vi[3], iREAL force, iREAL friction[3])
{
  iREAL vt[3];

  vt[0] = vi[0] - normal[0]*((vi[0]*normal[0]) + (vi[1]*normal[1]) + (vi[2]*normal[2]));
  vt[1] = vi[1] - normal[1]*((vi[0]*normal[0]) + (vi[1]*normal[1]) + (vi[2]*normal[2]));
  vt[2] = vi[2] - normal[2]*((vi[0]*normal[0]) + (vi[1]*normal[1]) + (vi[2]*normal[2]));

  friction[0] =  -vt[0]*FRICTION*force;
  friction[1] =  -vt[1]*FRICTION*force;
  friction[2] =  -vt[2]*FRICTION*force;
}


double delta::forces::springSphere(iREAL normal[3], iREAL depth, iREAL relativeVelocity[3], iREAL massA, iREAL massB, iREAL f[3])
{
  iREAL ma = (1/massA) + (1/massB);

  iREAL velocity = (relativeVelocity[0]*normal[0]) + (relativeVelocity[1]*normal[1]) + (relativeVelocity[2]*normal[2]);

  iREAL damp = 2 * SDAMPER * sqrt(SSPRING*ma)*velocity;

  iREAL force = SSPRING*depth + damp;

  f[0] = force*normal[0];
  f[1] = force*normal[1];
  f[2] = force*normal[2];

  return force;
}

void delta::forces::frictionSphere(iREAL normal[3], iREAL vi[3], iREAL force, iREAL friction[3])
{
  iREAL vt[3];

  vt[0] = vi[0] - normal[0]*((vi[0]*normal[0]) + (vi[1]*normal[1]) + (vi[2]*normal[2]));
  vt[1] = vi[1] - normal[1]*((vi[0]*normal[0]) + (vi[1]*normal[1]) + (vi[2]*normal[2]));
  vt[2] = vi[2] - normal[2]*((vi[0]*normal[0]) + (vi[1]*normal[1]) + (vi[2]*normal[2]));

  friction[0] =  -vt[0]*SFRICTION*force;
  friction[1] =  -vt[1]*SFRICTION*force;
  friction[2] =  -vt[2]*SFRICTION*force;
}

void delta::forces::getContactForces(
    std::vector<delta::collision::contactpoint> &conpnt,
    iREAL positionASpatial[3],
	iREAL positionAReferential[3],
	iREAL angularA[3],
    iREAL refAngularA[3],
    iREAL linearA[3],
    iREAL massA,
	iREAL inverseA[9],
	iREAL rotationA[9],
    iREAL positionBSpatial[3],
	iREAL positionBReferential[3],
	iREAL angularB[3],
    iREAL refAngularB[3],
    iREAL linearB[3],
    iREAL massB,
	iREAL inverseB[9],
	iREAL rotationB[9],
	iREAL force[3],
    iREAL torque[3],
	bool  isSphere)
{

    for(unsigned int k = 0; k<conpnt.size(); k++)
    {
        iREAL z[3], vi[3], vj[3], vij[3];

        //contact point - position i
        z[0] = conpnt[k].x[0]-positionASpatial[0];
        z[1] = conpnt[k].x[1]-positionASpatial[1];
        z[2] = conpnt[k].x[2]-positionASpatial[2];
        
        //cross product - relative angular i to contact point plus linear i
        vi[0] = refAngularA[1]*z[2]-refAngularA[2]*z[1] + linearA[0];
        vi[1] = refAngularA[2]*z[0]-refAngularA[0]*z[2] + linearA[1];
        vi[2] = refAngularA[0]*z[1]-refAngularA[1]*z[0] + linearA[2];
        
        //contact point - position j
        z[0] = conpnt[k].x[0]-positionBSpatial[0];
        z[1] = conpnt[k].x[1]-positionBSpatial[1];
        z[2] = conpnt[k].x[2]-positionBSpatial[2];
        
        //cross product - relative angular j to contact point plus linear j
        vj[0] = refAngularB[1]*z[2]-refAngularB[2]*z[1] + linearB[0];
        vj[1] = refAngularB[2]*z[0]-refAngularB[0]*z[2] + linearB[1];
        vj[2] = refAngularB[0]*z[1]-refAngularB[1]*z[0] + linearB[2];
        
        //relative velocities
        vij[0] = vj[0] - vi[0];
        vij[1] = vj[1] - vi[1];
        vij[2] = vj[2] - vi[2];

        iREAL f[3], friction[3], forc;

        friction[0] = 0.0;
        friction[1] = 0.0;
        friction[2] = 0.0;

        if(isSphere)
        {
        	forc = delta::forces::springSphere(conpnt[k].normal, conpnt[k].depth, vij, massA, massB, f);

            if(conpnt[k].frictionType > 2)
            	delta::forces::frictionSphere(conpnt[k].normal, vi, forc, friction);
        }
        else{
            forc = delta::forces::spring(conpnt[k].normal,conpnt[k].x, conpnt[k].depth, vij,
            							positionASpatial, positionBSpatial,
										positionAReferential, positionBReferential, massA, massB,
            							rotationA, rotationB, inverseA, inverseB, f);

            if(conpnt[k].frictionType > 2)
            	delta::forces::friction(conpnt[k].normal, vi, forc, friction);
        }

        f[0] = f[0] + friction[0];
        f[1] = f[1] + friction[1];
        f[2] = f[2] + friction[2];

        //accumulate force
        force[0] += f[0];
        force[1] += f[1];
        force[2] += f[2];

        iREAL arm[3];
        //contact-position = arm
        arm[0] = conpnt[k].x[0]-positionASpatial[0];
        arm[1] = conpnt[k].x[1]-positionASpatial[1];
        arm[2] = conpnt[k].x[2]-positionASpatial[2];

        //cross product accumulate torque
        torque[0] += arm[1]*(f[2]) - arm[2]*(f[1]);
        torque[1] += arm[2]*(f[0]) - arm[0]*(f[2]);
        torque[2] += arm[0]*(f[1]) - arm[1]*(f[0]);

        if(isSphere)
        {
			//relative angular velocities
			vij[0] = angularA[0] - angularB[0];
			vij[1] = angularA[1] - angularB[1];
			vij[2] = angularA[2] - angularB[2];

			iREAL w = std::abs(sqrt(vij[0]*vij[0]+vij[1]*vij[1]+vij[2]*vij[2]));
			//printf("W:%f | wij: %f %f %f\n", w, vij[0], vij[1], vij[2]);

			if(w>0.0)
			{
				torque[0] += -(vij[0]/w)*SFRICTIONROLLING*forc;
				torque[1] += -(vij[1]/w)*SFRICTIONROLLING*forc;
				torque[2] += -(vij[2]/w)*SFRICTIONROLLING*forc;
			}
        }
    }
}
