#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

#define Length 200													//"单位：毫米"
#define Breadth 200
#define Angle 180
#define a 5 
#define b 3 
#define N 300
#define fraction_line 100											//(srand(time(0))%100/fraction_line)	即（0-100）的随机数/fraction_**
#define fraction_angle 100
#define Time 1000													//微扰1000次 
#define Max_ope	1000												//：i，j微调次数上限，以防出现死循环 
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
struct Point points[4];												//四个顶点 
float entropy[Time];
/*—————————————————————————点-线关系判断————————————————————————*/ 
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
/*—————————————————————————点-框关系判断————————————————————————*/ 
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
	if(j<0||j>=4)													//异常检查 
		return -1;
	else{
		if(j==4)
			return 1;												//在框内 
		else
			return 0;		
	}//end else
}
/**********************************************************entropy***********************************************************/ 
Status Entropy(int t){												//对第t时刻的态求熵 
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
		state[j].x+=((srand(time(0))%200-100)/fraction_line);		//[-100,100]/100: [-1,1]的随机数 
		state[j].y+=((srand(time(0))%200-100)/fraction_line);
		state[j].angle+=((srand(time(0))%200-100)/fraction_angle);
//	}
	return OK;
}
/*一个暂时没用的perturb 
Status Perturb(struct State state[N]){
	int i,j;
	for(i=0;i<N;i++){
		state[i].x+=((srand(time(0))%200-100)/fraction_line);		//[-100,100]/100: [-1,1]的随机数 
		state[i].y+=((srand(time(0))%200-100)/fraction_line);
		state[i].angle+=((srand(time(0))%200-100)/fraction_angle);
	}
	return OK;
}
*/
/*———————————————————————一个不可行的overlap————————————————————————————
int Overlap(struct State state[N]){									//
	int i,j,k;
	float x,y,Dx1,Dx2,Dy1,Dy2;										//质心坐标、顶点坐标相对偏移（减少重复计算 
	int flag[4]=0;													//i的质心于边框相对位置标识：-1，0，1 
	for(i=0;i<N;i++){
		x=state[i].x;
		y=state[i].y;
		Dx1=a/2*sin(state[i].angle);								//求i的四个顶点，逆时针存入points 
		Dx2=b/2*cos(state[i].angle);
		Dy1=a/2*cos(state[i].angle);//可以用Dx优化，如果cos比*运算量大太多 
		Dy2=b/2*sin(state[i].angle);
		points[0].x=x+Dx1+Dx2;
		points[0].y=y-Dy1+Dy2;
		points[1].x=x-Dx1+Dx2;
		points[1].y=y+Dy1+Dy2;
		points[2].x=x-Dx1-Dx2;
		points[2].y=y+Dy1-Dy2;
		points[3].x=x+Dx1+Dx2;
		points[3].y=y-Dy1+Dy2;
		DL_Judge(x,y,points[0].x,points[0].y,points[1].x,points[1].y,&flag[0]);					//逆时针，第一个点-第二个点所连线 
		DL_Judge(x,y,points[1].x,points[1].y,points[2].x,points[2].y,&flag[1]);					//第二个点-第三个点所连线  
		DL_Judge(x,y,points[2].x,points[2].y,points[3].x,points[3].y,&flag[2]);					//第三个点-第四个点所连线  
		DL_Judge(x,y,points[0].x,points[0].y,points[3].x,points[3].y,&flag[3]);					//第一个点-第四个点所连线  
		for(j=i+1;j<N;j++)
			k=DR_Judge(state[j].x,state[j].y,points,flag);//这里还未完成 
	}
	return k;	
}
/*********************************************************overlap************************************************************/ 
int Overlap(int i,int j){
	int k;
	float x,y,Dx1,Dx2,Dy1,Dy2;										//质心坐标、顶点坐标相对偏移（减少重复计算 
	int flag[4]=0;													//i的质心于边框相对位置标识：-1，0，1 
		x=state[i].x;
		y=state[i].y;
/*———————————————————————求i的四个顶点，逆时针存入points——————————————————————*/ 
		Dx1=a/2*sin(state[i].angle); 
		Dx2=b/2*cos(state[i].angle);
		Dy1=a/2*cos(state[i].angle);//可以用Dx优化，如果cos比*运算量大太多 
		Dy2=b/2*sin(state[i].angle);
		points[0].x=x+Dx1+Dx2;
		points[0].y=y-Dy1+Dy2;
		points[1].x=x-Dx1+Dx2;
		points[1].y=y+Dy1+Dy2;
		points[2].x=x-Dx1-Dx2;
		points[2].y=y+Dy1-Dy2;
		points[3].x=x+Dx1+Dx2;
		points[3].y=y-Dy1+Dy2;
/*————————————————————————判断质心与边框的关系：—————————————————————————*/ 
		DL_Judge(x,y,points[0].x,points[0].y,points[1].x,points[1].y,&flag[0]);					//逆时针，第一个点-第二个点所连线 
		DL_Judge(x,y,points[1].x,points[1].y,points[2].x,points[2].y,&flag[1]);					//第二个点-第三个点所连线  
		DL_Judge(x,y,points[2].x,points[2].y,points[3].x,points[3].y,&flag[2]);					//第三个点-第四个点所连线  
		DL_Judge(x,y,points[0].x,points[0].y,points[3].x,points[3].y,&flag[3]);					//第一个点-第四个点所连线  
		
		k=DR_Judge(state[j].x,state[j].y,points,flag);
		return k; 
}
/***********************************************************态初始化***********************************************************/
int main{
	int t;															//遍历Time的小参数 
	int i,j,k;														//普适小参数，比如遍历N 
	FILE *fp;
	for(t=0;t<Time;t++)												//熵参数初始化 
		entropy[t]=0;
	for(i=0;i<N;i++){												//坐标初始化 
		state[i].x=srand(time(0))%Length;
		state[i].y=srand(time(0))%Breadth;
		state.[i]angle=srand(time(0))%Angle;
	}
/*————————————————————————循环——————————————————————————————*/ 
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
/*————————————————————————导出Time—entropy数据———————————————————————*/ 
	fp=fopen("Entropy-T.txt","w");
	for(t=1;i<Time;t++)											//t=0,初态，暂时不要 
		fprintf(fp, "%d\t%.3f\n",t,entropy[t]);
	fclose(fp);

	fp=fopen("State-T.txt","w");
	for(i=0;i<N;i++)
		fprintf(fp,"%.3f\t%.3f\t%.3f\n",state[i].x,state[i].y,state[i].angle);
	fclose(fp);
	return 1;
}
