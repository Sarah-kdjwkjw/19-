#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

#define Length 200													//"��λ������"
#define Breadth 200
#define Angle 180
#define a 5 
#define b 3 
#define N 300
#define fraction_line 100											//(srand(time(0))%100/fraction_line)	����0-100���������/fraction_**
#define fraction_angle 100
#define Time 1000													//΢��1000�� 
#define Max_ope	1000												//��i��j΢���������ޣ��Է�������ѭ�� 
/************************************************************************************************************/
//#include<stdio.h>
//#include<string.h> 
#include<math.h>
#include<stdlib.h>
#include<time.h>
typedef int Status;
struct State{
	float x;
	float y;
	float angle;
};
struct Point{
	float x;
	float y;
};
struct State state[N]; 
struct Point points[4];												//�ĸ����� 
float entropy[Time];
/*����������������������������������������������������-�߹�ϵ�жϡ�����������������������������������������������*/ 
Status DL_Judge(float x,float y,float x1,float y1,float x2,float y2,int *flag){
	if(((x1-x2)*(y-y1)-(y1-y2)*(x-x1))>0)
		*flag=1;
	else
		if(((x1-x2)*(y-y1)-(y1-y2)*(x-x1))<0)
			*flag=-1;
		else
			*flag=0;
	return OK;
} 
/*����������������������������������������������������-���ϵ�жϡ�����������������������������������������������*/ 
int DR_Judge(float x,float y,struct Point points[4],int flag[4]){
	int i;
	int j=0;
	int SS_flag[4];													//same side: flag=1
	DL_Judge(x,y,points[0].x,points[0].y,points[1].x,points[1].y,SS_flag+0);
	DL_Judge(x,y,points[1].x,points[1].y,points[2].x,points[2].y,SS_flag+1);
	DL_Judge(x,y,points[2].x,points[2].y,points[3].x,points[3].y,SS_flag+2);
	DL_Judge(x,y,points[0].x,points[0].y,points[3].x,points[3].y,SS_flag+3);
	for(i=0;i<4;i++){
		if(flag[i]==SS_flag[i])
			j++;
	}
	if(j<0||j>=4)													//�쳣��� 
		return -1;
	else{
		if(j==4)
			return 1;												//�ڿ��� 
		else
			return 0;		
	}//end else
}
/**********************************************************entropy***********************************************************/ 
Status Entropy(int t){												//�Ե�tʱ�̵�̬���� 
	int i,j;
	for(i=0;i<N-1;i++){
		for(j=i+1;j<N;j++){
			entropy[t]+=cos(2*(state[i].angle-state[j].angle));
		}
	}
	return OK;	
}
/***********************************************************perturb***********************************************************/ 
Status Perturb(int j){
//	int i,j;
//	for(i=0;i<N;i++){
		state[j].x+=((srand(time(0))%200-100)/fraction_line);		//[-100,100]/100: [-1,1]������� 
		state[j].y+=((srand(time(0))%200-100)/fraction_line);
		state[j].angle+=((srand(time(0))%200-100)/fraction_angle);
//	}
	return OK;
}
/*һ����ʱû�õ�perturb 
Status Perturb(struct State state[N]){
	int i,j;
	for(i=0;i<N;i++){
		state[i].x+=((srand(time(0))%200-100)/fraction_line);		//[-100,100]/100: [-1,1]������� 
		state[i].y+=((srand(time(0))%200-100)/fraction_line);
		state[i].angle+=((srand(time(0))%200-100)/fraction_angle);
	}
	return OK;
}
*/
/*����������������������������������������������һ�������е�overlap��������������������������������������������������������
int Overlap(struct State state[N]){									//
	int i,j,k;
	float x,y,Dx1,Dx2,Dy1,Dy2;										//�������ꡢ�����������ƫ�ƣ������ظ����� 
	int flag[4]=0;													//i�������ڱ߿����λ�ñ�ʶ��-1��0��1 
	for(i=0;i<N;i++){
		x=state[i].x;
		y=state[i].y;
		Dx1=a/2*sin(state[i].angle);								//��i���ĸ����㣬��ʱ�����points 
		Dx2=b/2*cos(state[i].angle);
		Dy1=a/2*cos(state[i].angle);//������Dx�Ż������cos��*��������̫�� 
		Dy2=b/2*sin(state[i].angle);
		points[0].x=x+Dx1+Dx2;
		points[0].y=y-Dy1+Dy2;
		points[1].x=x-Dx1+Dx2;
		points[1].y=y+Dy1+Dy2;
		points[2].x=x-Dx1-Dx2;
		points[2].y=y+Dy1-Dy2;
		points[3].x=x+Dx1+Dx2;
		points[3].y=y-Dy1+Dy2;
		DL_Judge(x,y,points[0].x,points[0].y,points[1].x,points[1].y,&flag[0]);					//��ʱ�룬��һ����-�ڶ����������� 
		DL_Judge(x,y,points[1].x,points[1].y,points[2].x,points[2].y,&flag[1]);					//�ڶ�����-��������������  
		DL_Judge(x,y,points[2].x,points[2].y,points[3].x,points[3].y,&flag[2]);					//��������-���ĸ���������  
		DL_Judge(x,y,points[0].x,points[0].y,points[3].x,points[3].y,&flag[3]);					//��һ����-���ĸ���������  
		for(j=i+1;j<N;j++)
			k=DR_Judge(state[j].x,state[j].y,points,flag);//���ﻹδ��� 
	}
	return k;	
}
/*********************************************************overlap************************************************************/ 
int Overlap(int i,int j){
	int k;
	float x,y,Dx1,Dx2,Dy1,Dy2;										//�������ꡢ�����������ƫ�ƣ������ظ����� 
	int flag[4]=0;													//i�������ڱ߿����λ�ñ�ʶ��-1��0��1 
		x=state[i].x;
		y=state[i].y;
/*������������������������������������������������i���ĸ����㣬��ʱ�����points��������������������������������������������*/ 
		Dx1=a/2*sin(state[i].angle); 
		Dx2=b/2*cos(state[i].angle);
		Dy1=a/2*cos(state[i].angle);//������Dx�Ż������cos��*��������̫�� 
		Dy2=b/2*sin(state[i].angle);
		points[0].x=x+Dx1+Dx2;
		points[0].y=y-Dy1+Dy2;
		points[1].x=x-Dx1+Dx2;
		points[1].y=y+Dy1+Dy2;
		points[2].x=x-Dx1-Dx2;
		points[2].y=y+Dy1-Dy2;
		points[3].x=x+Dx1+Dx2;
		points[3].y=y-Dy1+Dy2;
/*�������������������������������������������������ж�������߿�Ĺ�ϵ����������������������������������������������������*/ 
		DL_Judge(x,y,points[0].x,points[0].y,points[1].x,points[1].y,&flag[0]);					//��ʱ�룬��һ����-�ڶ����������� 
		DL_Judge(x,y,points[1].x,points[1].y,points[2].x,points[2].y,&flag[1]);					//�ڶ�����-��������������  
		DL_Judge(x,y,points[2].x,points[2].y,points[3].x,points[3].y,&flag[2]);					//��������-���ĸ���������  
		DL_Judge(x,y,points[0].x,points[0].y,points[3].x,points[3].y,&flag[3]);					//��һ����-���ĸ���������  
		
		k=DR_Judge(state[j].x,state[j].y,points,flag);
		return k; 
}
/***********************************************************̬��ʼ��***********************************************************/
int main{
	int t;															//����Time��С���� 
	int i,j,k;														//����С�������������N 
	FILE *fp;
	for(t=0;t<Time;t++)												//�ز�����ʼ�� 
		entropy[t]=0;
	for(i=0;i<N;i++){												//�����ʼ�� 
		state[i].x=srand(time(0))%Length;
		state[i].y=srand(time(0))%Breadth;
		state.[i]angle=srand(time(0))%Angle;
	}
/*������������������������������������������������ѭ��������������������������������������������������������������*/ 
	for(t=0;t<Time;t++){
		Entropy(t);
//		Perturb(state);
		for(i=0;i<N-1;i++){
			for(j=i+1;j<N;j++){
				k=0;
				while(Overlap(i,j)&&k<Max_ope){
					Perturb(state[t]);
					k++;
				}//end while
			}
		}	
	}//end for
/*����������������������������������������������������Time��entropy���ݡ���������������������������������������������*/ 
	fp=fopen("Entropy-T.txt","w");
	for(t=1;i<Time;t++)											//t=0,��̬����ʱ��Ҫ 
		fprintf(fp, "%d\t%.3f\n",t,entropy[t]);
	fclose(fp);

	fp=fopen("State-T.txt","w");
	for(i=0;i<N;i++)
		fprintf(fp,"%.3f\t%.3f\t%.3f\n",state[i].x,state[i].y,state[i].angle);
	fclose(fp);
	return 1;
}
