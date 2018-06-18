#include "MathEx.h"

void GetRotMatrix( double (*pRot_Matrix)[3], double rad_alpha, double rad_beta, double rad_gamma)
{
	pRot_Matrix[0][0] = cos(rad_gamma)*cos(rad_beta);
	pRot_Matrix[0][1] = cos(rad_gamma)*sin(rad_beta)*sin(rad_alpha) - sin(rad_gamma)*cos(rad_alpha);
	pRot_Matrix[0][2] = cos(rad_gamma)*sin(rad_beta)*cos(rad_alpha) + sin(rad_gamma)*sin(rad_alpha);
	pRot_Matrix[1][0] = sin(rad_gamma)*cos(rad_beta);
	pRot_Matrix[1][1] = sin(rad_gamma)*sin(rad_beta)*sin(rad_alpha) + cos(rad_gamma)*cos(rad_alpha);
	pRot_Matrix[1][2] = sin(rad_gamma)*sin(rad_beta)*cos(rad_alpha) - cos(rad_gamma)*sin(rad_alpha);
	pRot_Matrix[2][0] = -sin(rad_beta);
	pRot_Matrix[2][1] = cos(rad_beta)*sin(rad_alpha);
	pRot_Matrix[2][2] = cos(rad_beta)*cos(rad_alpha);
}

void Set_ri( CCartesian *pr_i, double rad_alpha, double rad_beta, double rad_gamma, CCartesian *T_L) // r_i[i] 세팅
{
	double Rot_Matrix[3][3];

	/*-- 회전행렬 구하기 --*/ 
	GetRotMatrix( Rot_Matrix, rad_alpha, rad_beta, rad_gamma);	

	/*------- r_i[i] = Rotation 행렬 * m_T_L[i] -------*/ 
	for( int i = 0; i < 3; i++)
	{
		pr_i[i].x = (Rot_Matrix[0][0]*T_L[i].x) +
			       (Rot_Matrix[0][1]*T_L[i].y) +
				   (Rot_Matrix[0][2]*T_L[i].z);
		pr_i[i].y = (Rot_Matrix[1][0]*T_L[i].x) +
				   (Rot_Matrix[1][1]*T_L[i].y) +
				   (Rot_Matrix[1][2]*T_L[i].z);
		pr_i[i].z = (Rot_Matrix[2][0]*T_L[i].x) +
			       (Rot_Matrix[2][1]*T_L[i].y) +
				   (Rot_Matrix[2][2]*T_L[i].z);
	}
}

