#ifndef MFUNC
#define MFUNC

#include "struct.h"

//print information of points, faces or edges...
void printp(p* mp)
{
	cout<<'('<<mp->x<<','<<mp->y<<','<<mp->z<<')';
}
void printf(f** mf)
{
	int i;
	for(i=0;mf[i]!=NULL;i++)
	{
		cout<<"f"<<i<<":";
		printp(mf[i]->p1);
		printp(mf[i]->p2);
		printp(mf[i]->p3);
		cout<<endl;
	}
	if(i==0)
		cout<<"fail"<<endl;
}
void printe(e** me)
{
	int i;
	for(i=0;me[i]!=NULL;i++)
	{
		cout<<"e"<<i<<":";
		printp(me[i]->p1);
		printp(me[i]->p2);
		cout<<endl;
	}
}
void tracepoint(p** mp)
{
	for(int i=0;mp[i]!=NULL;i++)
	{
		for(int j=0;j<4;j++)
		{
			for(int k=0;k<4;k++)
				cout<<mp[i]->m4.ele[j][k]<<' ';
			cout<<endl;
		}
		cout<<endl<<endl;
	}
}


int getfacecount(f** mf)
{
	int count;
	for(count=0;mf[count]!=NULL;count++)
		;
	return count;
}

bool check_edge_exist(e* temp,e** me,int count)
{
	int i;
	for(i=0;i<count;i++)
	{
		if(checke(temp,me[i]))
			return true;
	}
	return false;
}

//Load .obj file
void loadfile(p** mp,e** me,f** mf,const char* filename)
{
	int* a=new int[10];
	char* str=new char[100];
	for(int i=0;i<100;i++)	str[i]=0;

	int countp(0);
	int counte(0);
	int countf(0);
	int temp(0);
	fstream infile;
	infile.open(filename);
	if(infile.is_open()==false)
	{
		cout<<"fail to open file"<<endl;
		return;
	}

	while(infile>>str)
	{
		if(str[0]=='v'&&str[1]==0)
		{
			mp[countp]=new p;
			infile>>mp[countp]->x>>mp[countp]->y>>mp[countp]->z;
			countp++;
			mp[countp]=NULL;
		}
		if(str[0]=='f'&&str[1]==0)
		{
			mf[countf]=new f;
			infile>>str;
			temp=0;
			for(int j=0;str[j]!='/'&&str[j]!=0;j++)
			{
				temp*=10;
				temp+=str[j]-48;
			}
			mf[countf]->p1=mp[temp-1];

			infile>>str;
			temp=0;
			for(int j=0;str[j]!='/'&&str[j]!=0;j++)
			{
				temp*=10;
				temp+=str[j]-48;
			}
			mf[countf]->p2=mp[temp-1];

			infile>>str;
			temp=0;
			for(int j=0;str[j]!='/'&&str[j]!=0;j++)
			{
				temp*=10;
				temp+=str[j]-48;
			}
			mf[countf]->p3=mp[temp-1];
			mf[countf]->present=1;
			countf++;
			mf[countf]=NULL;
		}
	}
	e* tempe=new e;
	for(int i=0;mf[i]!=NULL;i++)
	{
		tempe->p1=mf[i]->p1;
		tempe->p2=mf[i]->p2;
		if(check_edge_exist(tempe,me,counte)==false)
		{
			me[counte]=new e;
			me[counte]->p1=tempe->p1;
			me[counte]->p2=tempe->p2;
			me[counte]->present=1;
			counte++;
			me[counte]=NULL;

		}

		tempe->p1=mf[i]->p2;
		tempe->p2=mf[i]->p3;
		if(check_edge_exist(tempe,me,counte)==false)
		{
			me[counte]=new e;
			me[counte]->p1=tempe->p1;
			me[counte]->p2=tempe->p2;
			me[counte]->present=1;
			counte++;
			me[counte]=NULL;
		}

		tempe->p1=mf[i]->p1;
		tempe->p2=mf[i]->p3;
		if(check_edge_exist(tempe,me,counte)==false)
		{
			me[counte]=new e;
			me[counte]->p1=tempe->p1;
			me[counte]->p2=tempe->p2;
			me[counte]->present=1;
			counte++;
			me[counte]=NULL;
		}
	}

	infile.close();
}
//Create new .obj file
void savefile(p** mp,e** me,f** mf,const char* filename)
{
	ofstream outfile;
	outfile.open(filename);
	if(outfile.is_open()==false)
	{
		cout<<"fail to write file"<<endl;
		return;
	}

	for(int i=0;mp[i]!=NULL;i++)
	{
		outfile<<"v "<<mp[i]->x<<' '<<mp[i]->y<<' '<<mp[i]->z<<endl;
	}
	outfile<<endl;

	for(int i=0;mf[i]!=NULL;i++)
	{
		int p1,p2,p3;
		for(int j=0;mp[j]!=NULL;j++)
		{
			if(checkp(mp[j],mf[i]->p1))
				p1=j+1;
			if(checkp(mp[j],mf[i]->p2))
				p2=j+1;
			if(checkp(mp[j],mf[i]->p3))
				p3=j+1;
		}
		outfile<<"f "<<p1<<' '<<p2<<' '<<p3<<endl;
	}
	outfile.close();
	cout<<"save finished"<<endl;
}


//Calculate q matrix for every face in the model
void getqmatrix(f** mf,p** mp)
{
	for(int i=0;mf[i]!=NULL;i++)
	{
		if(mf[i]->p1!=NULL)
		{
			for(int j=0;j<4;j++)
				for(int k=0;k<4;k++)
				{
					mf[i]->p1->m4.ele[j][k]=0;
					mf[i]->p2->m4.ele[j][k]=0;
					mf[i]->p3->m4.ele[j][k]=0;
				}
		}
	}
	for(int i=0;mf[i]!=NULL;i++)
	{
		double a=mf[i]->p1->y*(mf[i]->p2->z-mf[i]->p3->z)+mf[i]->p2->y*(mf[i]->p3->z-mf[i]->p1->z)+mf[i]->p3->y*(mf[i]->p1->z-mf[i]->p2->z);
		double b=mf[i]->p1->z*(mf[i]->p2->x-mf[i]->p3->x)+mf[i]->p2->z*(mf[i]->p3->x-mf[i]->p1->x)+mf[i]->p3->z*(mf[i]->p1->x-mf[i]->p2->x);
		double c=mf[i]->p1->x*(mf[i]->p2->y-mf[i]->p3->y)+mf[i]->p2->x*(mf[i]->p3->y-mf[i]->p1->y)+mf[i]->p3->x*(mf[i]->p1->y-mf[i]->p2->y);
		mf[i]->v4.ele[0]=a/pow(a*a+b*b+c*c,0.5);
		mf[i]->v4.ele[1]=b/pow(a*a+b*b+c*c,0.5);
		mf[i]->v4.ele[2]=c/pow(a*a+b*b+c*c,0.5);
		mf[i]->v4.ele[3]=-(mf[i]->v4.ele[0]*mf[i]->p1->x)-(mf[i]->v4.ele[1]*mf[i]->p1->y)-(mf[i]->v4.ele[2]*mf[i]->p1->z);
		for(int j=0;j<4;j++)
			for(int k=0;k<4;k++)
			{
				mf[i]->p1->m4.ele[j][k]+=mf[i]->v4.ele[j]*mf[i]->v4.ele[k];
				mf[i]->p2->m4.ele[j][k]+=mf[i]->v4.ele[j]*mf[i]->v4.ele[k];
				mf[i]->p3->m4.ele[j][k]+=mf[i]->v4.ele[j]*mf[i]->v4.ele[k];
			}
	}
}
//Delete redundant points, faces and edges. It is one part of function del_edge() and del_vertex()
void clear(p* p1,p* p2,p** mp,e** me,f** mf)
{
	if(checkp(p1,p2)==false)
		return;
	int counte(0);
	int countf(0);
	int countp(0);
	for(int i=0;me[i]!=NULL;i++)
	{
		if((me[i]->p1==p1&&me[i]->p2==p2)||(me[i]->p1==p2&&me[i]->p2==p1))
		{
			me[i]->present=0;
		}
		else if(me[i]->p1==p2)
		{
			for(int j=0;me[j]!=NULL;j++)
			{
				if(i==j)
					continue;
				if((me[j]->p1==p1&&me[j]->p2==me[i]->p2)||(me[j]->p2==p1&&me[j]->p1==me[i]->p2))
				{
					me[i]->present=0;
					break;
				}
			}
		}
		else if(me[i]->p2==p2)
		{
			for(int j=0;me[j]!=NULL;j++)
			{
				if(i==j)
					continue;
				if((me[j]->p1==p1&&me[j]->p2==me[i]->p1)||(me[j]->p2==p1&&me[j]->p1==me[i]->p1))
				{
					me[i]->present=0;
					break;
				}
			}
		}

		if(me[i]->p1==p2&&me[i]->present==1)
		{
			me[i]->p1=p1;
		}
		if(me[i]->p2==p2&&me[i]->present==1)
		{
			me[i]->p2=p1;
		}
		counte++;
	}


	for(int i=0;mf[i]!=NULL;i++)
	{
		if(checkp(mf[i]->p1,mf[i]->p2)||checkp(mf[i]->p1,mf[i]->p3)||checkp(mf[i]->p2,mf[i]->p3))
		{
			mf[i]->present=0;
		}
		else if(mf[i]->p1==p2)
		{
			for(int j=0;mf[j]!=NULL;j++)
			{
				if(i==j)
					continue;
				if(		(mf[j]->p1==p1&&mf[j]->p2==mf[i]->p2&&mf[j]->p3==mf[i]->p3)||
						(mf[j]->p1==p1&&mf[j]->p2==mf[i]->p3&&mf[j]->p3==mf[i]->p2)||
						(mf[j]->p2==p1&&mf[j]->p1==mf[i]->p2&&mf[j]->p3==mf[i]->p3)||
						(mf[j]->p2==p1&&mf[j]->p1==mf[i]->p3&&mf[j]->p3==mf[i]->p2)||
						(mf[j]->p3==p1&&mf[j]->p1==mf[i]->p2&&mf[j]->p2==mf[i]->p3)||
						(mf[j]->p3==p1&&mf[j]->p1==mf[i]->p3&&mf[j]->p2==mf[i]->p2)		)
				{
					mf[i]->present=0;
				}
			}
		}
		else if(mf[i]->p2==p2)
		{
			for(int j=0;mf[j]!=NULL;j++)
			{
				if(i==j)
					continue;
				if(		(mf[j]->p1==p1&&mf[j]->p2==mf[i]->p1&&mf[j]->p3==mf[i]->p3)||
						(mf[j]->p1==p1&&mf[j]->p2==mf[i]->p3&&mf[j]->p3==mf[i]->p1)||
						(mf[j]->p2==p1&&mf[j]->p1==mf[i]->p1&&mf[j]->p3==mf[i]->p3)||
						(mf[j]->p2==p1&&mf[j]->p1==mf[i]->p3&&mf[j]->p3==mf[i]->p1)||
						(mf[j]->p3==p1&&mf[j]->p1==mf[i]->p1&&mf[j]->p2==mf[i]->p3)||
						(mf[j]->p3==p1&&mf[j]->p1==mf[i]->p3&&mf[j]->p2==mf[i]->p1)		)
				{
					mf[i]->present=0;
				}
			}
		}
		else if(mf[i]->p3==p2)
		{
			for(int j=0;mf[j]!=NULL;j++)
			{
				if(i==j)
					continue;
				if(		(mf[j]->p1==p1&&mf[j]->p2==mf[i]->p2&&mf[j]->p3==mf[i]->p1)||
						(mf[j]->p1==p1&&mf[j]->p2==mf[i]->p1&&mf[j]->p3==mf[i]->p2)||
						(mf[j]->p2==p1&&mf[j]->p1==mf[i]->p2&&mf[j]->p3==mf[i]->p1)||
						(mf[j]->p2==p1&&mf[j]->p1==mf[i]->p1&&mf[j]->p3==mf[i]->p2)||
						(mf[j]->p3==p1&&mf[j]->p1==mf[i]->p2&&mf[j]->p2==mf[i]->p1)||
						(mf[j]->p3==p1&&mf[j]->p1==mf[i]->p1&&mf[j]->p2==mf[i]->p2)		)
				{
					mf[i]->present=0;
				}

			}
		}
		else if((0==mf[i]->p1->y*(mf[i]->p2->z-mf[i]->p3->z)+mf[i]->p2->y*(mf[i]->p3->z-mf[i]->p1->z)+mf[i]->p3->y*(mf[i]->p1->z-mf[i]->p2->z))&&
				(0==mf[i]->p1->z*(mf[i]->p2->x-mf[i]->p3->x)+mf[i]->p2->z*(mf[i]->p3->x-mf[i]->p1->x)+mf[i]->p3->z*(mf[i]->p1->x-mf[i]->p2->x))&&
				(0==mf[i]->p1->x*(mf[i]->p2->y-mf[i]->p3->y)+mf[i]->p2->x*(mf[i]->p3->y-mf[i]->p1->y)+mf[i]->p3->x*(mf[i]->p1->y-mf[i]->p2->y)) )
		{
			mf[i]->present=0;
		}

		if(mf[i]->p1==p2&&mf[i]->present==1)
		{
			mf[i]->p1=p1;
		}
		if(mf[i]->p2==p2&&mf[i]->present==1)
		{
			mf[i]->p2=p1;
		}
		if(mf[i]->p3==p2&&mf[i]->present==1)
		{
			mf[i]->p3=p1;
		}
		countf++;
	}

	for(int i=0;me[i]!=NULL;i++)
	{
		if(me[i]->present==0)
		{
			for(int j=counte-1;j>=i;j--)
			{
				if(me[j]!=NULL&&me[j]->present==1)
				{
					me[i]=me[j];
					me[j]=NULL;
					break;
				}
				if(i==j&&me[j]->present==0)
				{
					me[i]=NULL;
					break;
				}
			}
		}
	}

	for(int i=0;mf[i]!=NULL;i++)
	{
		if(mf[i]->present==0)
		{
			for(int j=countf-1;j>=i;j--)
			{
				if(mf[j]!=NULL&&mf[j]->present==1)
				{
					mf[i]=mf[j];
					mf[j]=NULL;
					break;
				}
				if(i==j&&mf[j]->present==0)
				{
					mf[i]=NULL;
					break;
				}
			}
		}
	}

	for(int i=0;mp[i]!=NULL;i++)
	{
		countp++;
	}
	for(int i=0;mp[i]!=NULL;i++)
	{
		if(mp[i]==p2&&i!=countp-1)
		{
			mp[i]=mp[countp-1];
			mp[countp-1]=NULL;
			countp--;
		}
		if(mp[i]==p2&&i==countp-1)
		{
			mp[i]=NULL;
		}
	}


}
//Edge collapse algorithm
void del_edge(p** mp,e** me,f** mf)
{
	p* temp1=new p;
	p* temp2=new p;
	double minx,miny,minz;
	double tempx,tempy,tempz;
	matrix4 sum;
	matrix4* tempm=new matrix4;
	double mincost(MAX*MAX);
	for(int i=0;me[i]!=NULL;i++)
	{
		for(int u=0;u<4;u++)
			for(int v=0;v<4;v++)
			{
				if(u!=3)
					sum.ele[u][v]=me[i]->p1->m4.ele[u][v]+me[i]->p2->m4.ele[u][v];//Q=Q1+Q2
				else
					sum.ele[u][v]=(int)(v/3);//0 0 0 1
			}
		//Check and store the inverse matrix in 'tempm'
		if(inverse(sum,tempm)==false)
		{
			for(int u=0;u<4;u++)
			for(int v=0;v<4;v++)
			{
				//Q=Q1+Q2
				sum.ele[u][v]=me[i]->p1->m4.ele[u][v]+me[i]->p2->m4.ele[u][v];
			}
			//Use 0.5(v1+v2) if there is not an inverse matrix
			tempx=0.5*(me[i]->p1->x+me[i]->p2->x);
			tempy=0.5*(me[i]->p1->y+me[i]->p2->y);
			tempz=0.5*(me[i]->p1->z+me[i]->p2->z);
			//Calculate the cost
			me[i]->cost=tempx*(tempx*sum.ele[0][0]+tempy*sum.ele[1][0]+tempz*sum.ele[2][0]+sum.ele[3][0])
					   +tempy*(tempx*sum.ele[0][1]+tempy*sum.ele[1][1]+tempz*sum.ele[2][1]+sum.ele[3][1])
					   +tempz*(tempx*sum.ele[0][2]+tempy*sum.ele[1][2]+tempz*sum.ele[2][2]+sum.ele[3][2])
							 +(tempx*sum.ele[0][3]+tempy*sum.ele[1][3]+tempz*sum.ele[2][3]+sum.ele[3][3]);
		}
		else
		{
			for(int u=0;u<4;u++)
			for(int v=0;v<4;v++)
			{
				sum.ele[u][v]=me[i]->p1->m4.ele[u][v]+me[i]->p2->m4.ele[u][v];//Q=Q1+Q2
			}
			//With inverse matrix
			tempx=tempm->ele[0][3];
			tempy=tempm->ele[1][3];
			tempz=tempm->ele[2][3];
			me[i]->cost=tempx*(tempx*sum.ele[0][0]+tempy*sum.ele[1][0]+tempz*sum.ele[2][0]+sum.ele[3][0])
					   +tempy*(tempx*sum.ele[0][1]+tempy*sum.ele[1][1]+tempz*sum.ele[2][1]+sum.ele[3][1])
					   +tempz*(tempx*sum.ele[0][2]+tempy*sum.ele[1][2]+tempz*sum.ele[2][2]+sum.ele[3][2])
							 +(tempx*sum.ele[0][3]+tempy*sum.ele[1][3]+tempz*sum.ele[2][3]+sum.ele[3][3]);
		}

		if(me[i]->cost<mincost)
		{
			temp1=me[i]->p1;
			temp2=me[i]->p2;
			minx=tempx;
			miny=tempy;
			minz=tempz;
			mincost=me[i]->cost;
		}
	}
	for(int i=0;me[i]!=NULL;i++)
	{
		if(me[i]->cost==mincost)
		{
			cout<<"delete edge:"<<i<<endl;
			break;
		}
	}
	temp1->x=minx;temp2->x=minx;
	temp1->y=miny;temp2->y=miny;
	temp1->z=minz;temp2->z=minz;
	cout<<endl;
	clear(temp1,temp2,mp,me,mf);
}
//Smooth algorithm
void smooth(p** mp,e** me,double ratio)
{
	p** tem=new p*[MAX];
	int* count=new int[MAX];
	for(int i=0;mp[i]!=NULL;i++)
	{
		tem[i]=new p;
		tem[i]->x=tem[i]->y=tem[i]->z=0;
		count[i]=0;
	}
		
	for(int i=0;me[i]!=NULL;i++)
	{
		for(int j=0;mp[j]!=NULL;j++)
		{
			if(me[i]->p1==mp[j])
			{
				tem[j]->x+=me[i]->p2->x;
				tem[j]->y+=me[i]->p2->y;
				tem[j]->z+=me[i]->p2->z;
				count[j]++;
			}
			if(me[i]->p2==mp[j])
			{
				tem[j]->x+=me[i]->p1->x;
				tem[j]->y+=me[i]->p1->y;
				tem[j]->z+=me[i]->p1->z;
				count[j]++;
			}
		}
	}
	for(int i=0;mp[i]!=NULL;i++)
	{
		mp[i]->x=(tem[i]->x/count[i])*ratio+(mp[i]->x)*(1-ratio);
		mp[i]->y=(tem[i]->y/count[i])*ratio+(mp[i]->y)*(1-ratio);
		mp[i]->z=(tem[i]->z/count[i])*ratio+(mp[i]->z)*(1-ratio);
	}
}
//delete the suitable vertex
void del_vertex(p** mp,e** me,f** mf)
{
	int p_amount(0);
	double temp;
	for(int i=0;mp[i]!=NULL;i++)
		p_amount++;
	if(p_amount==3)
	{
		cout<<"Can not simplify anymore!"<<endl;
		return;
	}
	double* dis=new double[p_amount];
	double dmax(0);
	int maxcount(-1);

	//Calculate the distance for vertex
	for(int i=0;i<p_amount;i++)
	{	
		dis[i]=0;
		for(int j=0;j<p_amount;j++)
		{
			temp=pow(mp[i]->x-mp[j]->x,2)+pow(mp[i]->y-mp[j]->y,2)+pow(mp[i]->z-mp[j]->z,2);
			dis[i]+=pow(temp,0.5);
		}
		if(dis[i]>dmax)
		{
			dmax=dis[i];
			maxcount=i;
		}
	}
	
	//Move this vertex
	for(int i=0;me[i]!=NULL;i++)
	{
		if(checkp(mp[maxcount],me[i]->p1))
		{
			mp[maxcount]->x=me[i]->p2->x;
			mp[maxcount]->y=me[i]->p2->y;
			mp[maxcount]->z=me[i]->p2->z;
			clear(mp[maxcount],me[i]->p2,mp,me,mf);
			return;
		}
		if(checkp(mp[maxcount],me[i]->p2))
		{
			mp[maxcount]->x=me[i]->p1->x;
			mp[maxcount]->y=me[i]->p1->y;
			mp[maxcount]->z=me[i]->p1->z;
			clear(mp[maxcount],me[i]->p1,mp,me,mf);
			return;
		}
	}
	cout<<"can't find the point"<<endl;
	return;
}

#endif