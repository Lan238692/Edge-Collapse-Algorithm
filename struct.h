#ifndef MSTRUCT
#define MSTRUCT

#include <math.h>
#include <windows.h>
#include <gl/glut.h> 
#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;
#define MAX 10000

struct matrix4
{
	double ele[4][4];
};
struct matrix3
{
	double ele[3][3];
};
struct vector4
{
	double ele[4];
};

struct p
{
	double x;
	double y;
	double z;
	matrix4 m4;
};
struct e
{
	p* p1;
	p* p2;
	double cost;
	bool present;
};
struct f
{
	p* p1;
	p* p2;
	p* p3;
	vector4 v4;
	bool present;
};



//Check if two points or two edges are the same
bool checkp(p* p1,p* p2)
{
	if(p1->x==p2->x&&p1->y==p2->y&&p1->z==p2->z)
		return true;
	else
		return false;
}
bool checke(e* e1,e* e2)
{
	if(checkp(e1->p1,e2->p1)&&checkp(e1->p2,e2->p2))
		return true;
	if(checkp(e1->p1,e2->p2)&&checkp(e1->p2,e2->p1))
		return true;
	return false;
}

//Check if it is an odd alignment or an even alignment
bool checkperm(int a,int b,int c,int d)
{
	int sum(0);
	if(a>b)	sum++;
	if(a>c)	sum++;
	if(a>d)	sum++;
	if(b>c)	sum++;
	if(b>d)	sum++;
	if(c>d)	sum++;
	if(sum%2==0)
		return 0;
	else
		return 1;
}

//Calculating the determinant(3*3 matrix)
double m3det(matrix3 m)
{
	return  (m.ele[0][0]*m.ele[1][1]*m.ele[2][2])
			+(m.ele[0][1]*m.ele[1][2]*m.ele[2][0])
			+(m.ele[0][2]*m.ele[1][0]*m.ele[2][1])
			-(m.ele[0][2]*m.ele[1][1]*m.ele[2][0])
			-(m.ele[0][1]*m.ele[1][0]*m.ele[2][2])
			-(m.ele[0][0]*m.ele[1][2]*m.ele[2][1])
			;
}

//Inverse matrix
bool inverse(matrix4 m,matrix4* t)
{
	double det(0);
	for(int a=0;a<4;a++)
		for(int b=0;b<4;b++)
			for(int c=0;c<4;c++)
				for(int d=0;d<4;d++)
				{
					if(a!=b&&a!=c&&a!=d&&b!=c&&b!=d&&c!=d)
					{
						if(checkperm(a,b,c,d)==0)
						{
							det+=m.ele[0][a]*m.ele[1][b]*m.ele[2][c]*m.ele[3][d];
						}
						else
						{
							det-=m.ele[0][a]*m.ele[1][b]*m.ele[2][c]*m.ele[3][d];
						}
					}
				}
	if(det==0)
		return false;
	

	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			matrix3 tempm;
			for(int x=0;x<3;x++)
			{
				for(int y=0;y<3;y++)
				{
					if(x<i&&y<j)
					{
						tempm.ele[x][y]=m.ele[y][x];
					}
					if(x>=i&&y<j)
					{
						tempm.ele[x][y]=m.ele[y][x+1];
					}
					if(x<i&&y>=j)
					{
						tempm.ele[x][y]=m.ele[y+1][x];
					}
					if(x>=i&&y>=j)
					{
						tempm.ele[x][y]=m.ele[y+1][x+1];
					}
				}
			}
			t->ele[i][j]=m3det(tempm)*pow((double)-1,i+j)/det;
		}
	}
	return true;
}

#endif