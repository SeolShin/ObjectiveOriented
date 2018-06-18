#ifndef MATHEX_H
#define MATHEX_H

#include <windows.h>
#include <math.h>
#include "../Cartesian.h"

// 회전행렬 구하기
void GetRotMatrix( double (*pRot_Matrix)[3], double rad_alpha, double rad_beta, double rad_gamma);
/* r_i[i] 세팅 */
void Set_ri( CCartesian *pr_i, double rad_alpha, double rad_beta, double rad_gamma, CCartesian *T_L);	
#endif