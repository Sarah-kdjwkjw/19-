#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

#define Pi 3.14159
#define Length 200													//边界尺寸	"单位：毫米"
#define Breadth 200
#define Angle 180
#define a 5 														//粒子尺寸 
#define b 3 
#define N 2														//粒子数目 
#define fraction_line 100											//衡量离散程度  ：(srand(time(0))%100/fraction_line)	即（0-100）的随机数/fraction_**
#define fraction_angle 100
#define Time 2														//微扰次数 
//#define Max_ope	1000											//：i，j微调次数上限，以防出现死循环 
#define dtheta 1													//振幅调节？ 
#define dx 2
#define dy 2
#define k 4															//熵参数：Gk 

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
typedef int Status;
struct State{
	float x;
	float y;
	float angle_d;													//角度制 
	float angle_r;													//弧度制 
};
struct Point{														//变幻时作辅助 
	float x;
	float y;
}; 
struct Rela{														//图的链式结构的结点 ；无向无权图，存储重合关系 
	int vex;														//顶点下标 
	struct Rela *next;
};
float entropy[Time];												//表征熵的参数 
struct State state[N];												//录入系统的态 
struct Point points_i[4];											//i粒子的四个顶点，辅助用 
struct Point points_j[4];
/******************************************使满足定义域********************************************************/
Status EnGood_A(int s){												//good area:操作粒子k;直到定义域符合条件 
	int i,j;
	int tag;
	int flag[4][2];													//4个顶点，2个坐标分量(x,y)
	for(i=0;i<4;i++)//初始化 
		for(j=0;j<4;j++)
			flag[i][j]=0; 
	i=0;
	while(i++<4){													//第i个顶点 
		if(points_i[i].x>Length)
			flag[i][0]=1;
		else if(points_i[i].x<0)
				flag[i][0]=-1;

		if(points_i[i].y>Breadth)
			flag[i][1]=1;
		else if(points_i[i].y<0)
				flag[i][1]=-1; 
	}//end while 
	tag=0;															//以下：x分量。tag>0:有顶点偏右；<0:有顶点偏左 
	for(i=0;i<4;i++)
		tag+=flag[i][0];
	if(flag>0)														//变动1个粒子的线度 
		state[s].x-=(a>b?a:b); 
	else if(tag<0)
			state[s].x+=(a>b?a:b);
	tag=0;															//以下：上下边界判定，tag>0:偏上
	for(i=0;i<4;i++)
		tag+=flag[i][1]; 
	if(tag>0)
		state[s].y-=b;
	else if(tag<0)
			state[s].y+=(a>b?a:b); 
		else state[s].y-=(a>b?a:b);
	while(state[s].angle_d>180)
		state[s].angle_d-=dtheta;
	while(state[s].angle_d<0)
		state[s].angle_d+=dtheta;
	state[s].angle_r=state[s].angle_d/180*Pi;
//	更新points[]	//好像没有必要 
	return OK;
}
/***************************************************overlap******************************************************/
//点-线关系判断： 
int DL_Judge(float x,float y,float x1,float y1,float x2,float y2){									//dot-line;flag：相对位置标识 
	int flag;
	if(((x1-x2)*(y-y1)-(y1-y2)*(x-x1))>0)
		flag=1;
	else
		if(((x1-x2)*(y-y1)-(y1-y2)*(x-x1))<0)
			flag=-1;
		else
			flag=0;
//	printf("%d\t",flag);
	return flag;
} 
//点-框关系判断： 
int DR_Judge(float x,float y,struct Point points[4],int flag[4]){									//dot-rectangle,参数flag[]：框的质心与框的关系标识 
	int i;
	int j=0;
	int SS_flag[4];
	SS_flag[0]=DL_Judge(x,y,points[0].x,points[0].y,points[1].x,points[1].y);						//逆时针，第一个点-第二个点所连线 
	SS_flag[1]=DL_Judge(x,y,points[1].x,points[1].y,points[2].x,points[2].y);						//第二个点-第三个点所连线 
	SS_flag[2]=DL_Judge(x,y,points[2].x,points[2].y,points[3].x,points[3].y);						//第三个点-第四个点所连线
	SS_flag[3]=DL_Judge(x,y,points[0].x,points[0].y,points[3].x,points[3].y);						//第一个点-第四个点所连线
	for(i=0;i<4;i++){
//		printf("\n ssflag:%d	flag:%d \n",SS_flag[i],flag[i]);
		if(flag[i]==SS_flag[i])
			j++;
		}
	if(j<0||j>4)
		return -10;													//异常；-10：以防叠加判断出错 
	else{
		if(j==4)
			return 1;//1：在框内 
		else
			return 0;		
	}//end else
}
/*——————————————————————————————————————————————————————————————*/ 
int Overlap(int i,int j){											//判断粒子i,j是否重叠。1：重叠 
	int s,r=0;
	int flag[4];													//i的质心于边框相对位置标识：-1，0，1 
	float x,y; 
	while(r++<4)//初始化 :
		flag[r]=0;
	x=state[i].x;
	y=state[i].y;
//判断质心与边框的关系，存入flag[4]：
	flag[0]=DL_Judge(x,y,points_i[0].x,points_i[0].y,points_i[1].x,points_i[1].y);					//逆时针，第一个点-第二个点所连线 
	flag[1]=DL_Judge(x,y,points_i[1].x,points_i[1].y,points_i[2].x,points_i[2].y);					//第二个点-第三个点所连线  
	flag[2]=DL_Judge(x,y,points_i[2].x,points_i[2].y,points_i[3].x,points_i[3].y);					//第三个点-第四个点所连线  
	flag[3]=DL_Judge(x,y,points_i[0].x,points_i[0].y,points_i[3].x,points_i[3].y);					//第一个点-第四个点所连线  
//判断j的顶点与i的关系：
	s=r=0; 
//	printf("flag[0]: %d\nflag[1]: %d\nflag[2]: %d\nflag[3]: %d\n",flag[0],flag[1],flag[2],flag[3]);
	while(s++<4){
		r+=DR_Judge(points_j[s].x,points_j[s].y,points_i,flag);										//r:计数在框内的顶点数。提前计算flag：减少重复计算 
	}
	if(r==0){ 

		return 0;//所有点均在框外 
	}												
	else
		if(r>0)
			return 1;
		else
			return -1; 												//异常 
}
/************************************************反弹***************************************************/ 
Status Rebound(int i,int j){
	float Dx,Dy,Dtheta;
	Dx=state[j].x-state[i].x-a; 
	Dy=state[j].y-state[i].y-b;
	Dtheta=state[j].angle_d-state[i].angle_d;

	state[i].x-=(Dx>0?1:(-1))*dx;
	state[i].y-=(Dy>0?1:(-1))*dy; 
	state[j].x+=(Dx>0?1:(-1))*dx;
	state[j].y+=(Dy>0?1:(-1))*dy; //略微粗糙：0 
	if(Dtheta>90){
		state[i].angle_d-=dtheta;
		state[j].angle_d+=dtheta;
	}
	else if(Dtheta<-90){
			state[i].angle_d+=dtheta;
			state[j].angle_d-=dtheta;
		}
		else if(Dtheta>0){
			state[i].angle_d+=dtheta;
			state[j].angle_d-=dtheta;
		}
			else{
				state[i].angle_d-=dtheta;
				state[j].angle_d+=dtheta;
			}
	state[i].angle_r=state[i].angle_d/180*Pi;
	state[j].angle_r=state[j].angle_d/180*Pi;
	return TRUE;
} 
/****************************************************entropy*************************************************/ 
Status Entropy(int t){												//对第t时刻的系统求熵参数：Gk 
	int i,j;
	for(i=0;i<N-1;i++)
		for(j=i+1;j<N;j++){
			entropy[t]+=cos(k*(state[i].angle_r-state[j].angle_r));
		}
	return OK;	
}
/**********************************************main***********************************************************/ 
int main(){
//	int count=0;
	int i,j;														//小变量 
	int t;															//遍历time的小变量 
	FILE *fp;
	int flag=0;
	float Dx1,Dy1,Dx2,Dy2;
	float x,y;
	struct Rela rela[N];
	struct Rela *temp;//用来遍历图 
//初态:
	srand((unsigned)time(0));
	temp=(struct Rela*)malloc(sizeof(struct Rela));
	for(i=0;i<N;i++){
		state[i].x=rand()%Length;
		state[i].y=rand()%Breadth;
		state[i].angle_d=rand()%Angle;
		state[i].angle_r=state[i].angle_d/180*Pi;
		EnGood_A(i);
	}
	for(t=0;t<Time;t++)
		entropy[t]=0;	
//系统演化，随t： 
for(t=0;t<Time;t++){
	for(i=0;i<N-1;i++){												//两两判定是否重叠，并记录于图结构 
		x=state[i].x;												//以下若干行纯粹为了DL_Judge、DR_Judge服务 
		y=state[i].y;
		Dx1=a/2*sin(state[i].angle_r);//cos(弧度制) 
		Dx2=b/2*cos(state[i].angle_r);
		Dy1=a/2*cos(state[i].angle_r);//可以用Dx优化，如果cos比*运算量大太多
		Dy2=b/2*sin(state[i].angle_r);
		points_i[0].x=x+Dx1+Dx2;
		points_i[0].y=y-Dy1+Dy2;
		points_i[1].x=x-Dx1+Dx2;
		points_i[1].y=y+Dy1+Dy2;
		points_i[2].x=x-Dx1-Dx2;
		points_i[2].y=y+Dy1-Dy2;
		points_i[3].x=x+Dx1+Dx2;
		points_i[3].y=y-Dy1+Dy2;
		rela[i].vex=i;//链表的头部，后接与其有重叠关系的结点 
		rela[i].next=(struct Rela*)malloc(sizeof(struct Rela));
		temp=rela[i].next;
		for(j=i+1;j<N;j++){
			x=state[j].x;
			y=state[j].y;
			Dx1=a/2*sin(state[j].angle_r); 
			Dx2=b/2*cos(state[j].angle_r);
			Dy1=a/2*cos(state[j].angle_r);
			Dy2=b/2*sin(state[j].angle_r);
			points_j[0].x=x+Dx1+Dx2;
			points_j[0].y=y-Dy1+Dy2;
			points_j[1].x=x-Dx1+Dx2;
			points_j[1].y=y+Dy1+Dy2;
			points_j[2].x=x-Dx1-Dx2;
			points_j[2].y=y+Dy1-Dy2;
			points_j[3].x=x+Dx1+Dx2;
			points_j[3].y=y-Dy1+Dy2;
			if(Overlap(i,j)==0){//则在i后面添加结点
				temp->vex=j;
				printf("\n temp-next %d\n",temp->next==NULL?0:1);
				temp=temp->next;
				printf("\n temp-next %d\n",temp->next==NULL?0:1);
				printf("\n temp-next %d\n",temp->next==NULL?0:1);
			}//end if
		}//end for(j)
		temp=NULL; 
	}//end for(i)													//已完成：重合判定、记录了有重合关系的粒子 于rela(图） 
	for(i=0;i<N;i++){
		temp=rela[i].next;
		printf("temp:%d\n",temp==NULL?0:1);
		printf("\n ~~~~BEGIN~~~~~\n");
		while(temp!=NULL){
			j=temp->vex;
			printf("\n相关节点：%d\n",j);
			Rebound(i,j);
			temp=temp->next;
			printf("HERE temp:%d\n",temp==NULL?0:1);
		}//end while 												//此时完成：i粒子与周边粒子各作用一次 
		printf("\n end while\n");
		EnGood_A(i);
		printf("\n engood: %d\n",EnGood_A(i)==OK?1:0); 
	}
	Entropy(t);														//系统已完成一次微调，记下此时的“熵” 
//导出t时刻系统数据（态、熵）：
	fp=fopen("Entropy-T.txt","w");
//	printf("entropy open !\n");
	fprintf(fp,"%d\t%.3f\n",t,entropy[t]);
	fclose(fp);

	fp=fopen("State-T.txt","w");
//	printf("open state\n");
	for(i=0;i<N;i++)
		fprintf(fp,"%.3f\t%.3f\t%.3f\n",state[i].x,state[i].y,state[i].angle_d);
	fclose(fp);
}//end for(t)
return 1;
}
