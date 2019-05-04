#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

#define Pi 3.14159
#define Length 200													//�߽�ߴ�	"��λ������"
#define Breadth 200
#define Angle 180
#define a 5 														//���ӳߴ� 
#define b 3 
#define N 2														//������Ŀ 
#define fraction_line 100											//������ɢ�̶�  ��(srand(time(0))%100/fraction_line)	����0-100���������/fraction_**
#define fraction_angle 100
#define Time 2														//΢�Ŵ��� 
//#define Max_ope	1000											//��i��j΢���������ޣ��Է�������ѭ�� 
#define dtheta 1													//������ڣ� 
#define dx 2
#define dy 2
#define k 4															//�ز�����Gk 

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
typedef int Status;
struct State{
	float x;
	float y;
	float angle_d;													//�Ƕ��� 
	float angle_r;													//������ 
};
struct Point{														//���ʱ������ 
	float x;
	float y;
}; 
struct Rela{														//ͼ����ʽ�ṹ�Ľ�� ��������Ȩͼ���洢�غϹ�ϵ 
	int vex;														//�����±� 
	struct Rela *next;
};
float entropy[Time];												//�����صĲ��� 
struct State state[N];												//¼��ϵͳ��̬ 
struct Point points_i[4];											//i���ӵ��ĸ����㣬������ 
struct Point points_j[4];
/******************************************ʹ���㶨����********************************************************/
Status EnGood_A(int s){												//good area:��������k;ֱ��������������� 
	int i,j;
	int tag;
	int flag[4][2];													//4�����㣬2���������(x,y)
	for(i=0;i<4;i++)//��ʼ�� 
		for(j=0;j<4;j++)
			flag[i][j]=0; 
	i=0;
	while(i++<4){													//��i������ 
		if(points_i[i].x>Length)
			flag[i][0]=1;
		else if(points_i[i].x<0)
				flag[i][0]=-1;

		if(points_i[i].y>Breadth)
			flag[i][1]=1;
		else if(points_i[i].y<0)
				flag[i][1]=-1; 
	}//end while 
	tag=0;															//���£�x������tag>0:�ж���ƫ�ң�<0:�ж���ƫ�� 
	for(i=0;i<4;i++)
		tag+=flag[i][0];
	if(flag>0)														//�䶯1�����ӵ��߶� 
		state[s].x-=(a>b?a:b); 
	else if(tag<0)
			state[s].x+=(a>b?a:b);
	tag=0;															//���£����±߽��ж���tag>0:ƫ��
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
//	����points[]	//����û�б�Ҫ 
	return OK;
}
/***************************************************overlap******************************************************/
//��-�߹�ϵ�жϣ� 
int DL_Judge(float x,float y,float x1,float y1,float x2,float y2){									//dot-line;flag�����λ�ñ�ʶ 
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
//��-���ϵ�жϣ� 
int DR_Judge(float x,float y,struct Point points[4],int flag[4]){									//dot-rectangle,����flag[]������������Ĺ�ϵ��ʶ 
	int i;
	int j=0;
	int SS_flag[4];
	SS_flag[0]=DL_Judge(x,y,points[0].x,points[0].y,points[1].x,points[1].y);						//��ʱ�룬��һ����-�ڶ����������� 
	SS_flag[1]=DL_Judge(x,y,points[1].x,points[1].y,points[2].x,points[2].y);						//�ڶ�����-�������������� 
	SS_flag[2]=DL_Judge(x,y,points[2].x,points[2].y,points[3].x,points[3].y);						//��������-���ĸ���������
	SS_flag[3]=DL_Judge(x,y,points[0].x,points[0].y,points[3].x,points[3].y);						//��һ����-���ĸ���������
	for(i=0;i<4;i++){
//		printf("\n ssflag:%d	flag:%d \n",SS_flag[i],flag[i]);
		if(flag[i]==SS_flag[i])
			j++;
		}
	if(j<0||j>4)
		return -10;													//�쳣��-10���Է������жϳ��� 
	else{
		if(j==4)
			return 1;//1���ڿ��� 
		else
			return 0;		
	}//end else
}
/*����������������������������������������������������������������������������������������������������������������������������*/ 
int Overlap(int i,int j){											//�ж�����i,j�Ƿ��ص���1���ص� 
	int s,r=0;
	int flag[4];													//i�������ڱ߿����λ�ñ�ʶ��-1��0��1 
	float x,y; 
	while(r++<4)//��ʼ�� :
		flag[r]=0;
	x=state[i].x;
	y=state[i].y;
//�ж�������߿�Ĺ�ϵ������flag[4]��
	flag[0]=DL_Judge(x,y,points_i[0].x,points_i[0].y,points_i[1].x,points_i[1].y);					//��ʱ�룬��һ����-�ڶ����������� 
	flag[1]=DL_Judge(x,y,points_i[1].x,points_i[1].y,points_i[2].x,points_i[2].y);					//�ڶ�����-��������������  
	flag[2]=DL_Judge(x,y,points_i[2].x,points_i[2].y,points_i[3].x,points_i[3].y);					//��������-���ĸ���������  
	flag[3]=DL_Judge(x,y,points_i[0].x,points_i[0].y,points_i[3].x,points_i[3].y);					//��һ����-���ĸ���������  
//�ж�j�Ķ�����i�Ĺ�ϵ��
	s=r=0; 
//	printf("flag[0]: %d\nflag[1]: %d\nflag[2]: %d\nflag[3]: %d\n",flag[0],flag[1],flag[2],flag[3]);
	while(s++<4){
		r+=DR_Judge(points_j[s].x,points_j[s].y,points_i,flag);										//r:�����ڿ��ڵĶ���������ǰ����flag�������ظ����� 
	}
	if(r==0){ 

		return 0;//���е���ڿ��� 
	}												
	else
		if(r>0)
			return 1;
		else
			return -1; 												//�쳣 
}
/************************************************����***************************************************/ 
Status Rebound(int i,int j){
	float Dx,Dy,Dtheta;
	Dx=state[j].x-state[i].x-a; 
	Dy=state[j].y-state[i].y-b;
	Dtheta=state[j].angle_d-state[i].angle_d;

	state[i].x-=(Dx>0?1:(-1))*dx;
	state[i].y-=(Dy>0?1:(-1))*dy; 
	state[j].x+=(Dx>0?1:(-1))*dx;
	state[j].y+=(Dy>0?1:(-1))*dy; //��΢�ֲڣ�0 
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
Status Entropy(int t){												//�Ե�tʱ�̵�ϵͳ���ز�����Gk 
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
	int i,j;														//С���� 
	int t;															//����time��С���� 
	FILE *fp;
	int flag=0;
	float Dx1,Dy1,Dx2,Dy2;
	float x,y;
	struct Rela rela[N];
	struct Rela *temp;//��������ͼ 
//��̬:
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
//ϵͳ�ݻ�����t�� 
for(t=0;t<Time;t++){
	for(i=0;i<N-1;i++){												//�����ж��Ƿ��ص�������¼��ͼ�ṹ 
		x=state[i].x;												//���������д���Ϊ��DL_Judge��DR_Judge���� 
		y=state[i].y;
		Dx1=a/2*sin(state[i].angle_r);//cos(������) 
		Dx2=b/2*cos(state[i].angle_r);
		Dy1=a/2*cos(state[i].angle_r);//������Dx�Ż������cos��*��������̫��
		Dy2=b/2*sin(state[i].angle_r);
		points_i[0].x=x+Dx1+Dx2;
		points_i[0].y=y-Dy1+Dy2;
		points_i[1].x=x-Dx1+Dx2;
		points_i[1].y=y+Dy1+Dy2;
		points_i[2].x=x-Dx1-Dx2;
		points_i[2].y=y+Dy1-Dy2;
		points_i[3].x=x+Dx1+Dx2;
		points_i[3].y=y-Dy1+Dy2;
		rela[i].vex=i;//�����ͷ��������������ص���ϵ�Ľ�� 
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
			if(Overlap(i,j)==0){//����i������ӽ��
				temp->vex=j;
				printf("\n temp-next %d\n",temp->next==NULL?0:1);
				temp=temp->next;
				printf("\n temp-next %d\n",temp->next==NULL?0:1);
				printf("\n temp-next %d\n",temp->next==NULL?0:1);
			}//end if
		}//end for(j)
		temp=NULL; 
	}//end for(i)													//����ɣ��غ��ж�����¼�����غϹ�ϵ������ ��rela(ͼ�� 
	for(i=0;i<N;i++){
		temp=rela[i].next;
		printf("temp:%d\n",temp==NULL?0:1);
		printf("\n ~~~~BEGIN~~~~~\n");
		while(temp!=NULL){
			j=temp->vex;
			printf("\n��ؽڵ㣺%d\n",j);
			Rebound(i,j);
			temp=temp->next;
			printf("HERE temp:%d\n",temp==NULL?0:1);
		}//end while 												//��ʱ��ɣ�i�������ܱ����Ӹ�����һ�� 
		printf("\n end while\n");
		EnGood_A(i);
		printf("\n engood: %d\n",EnGood_A(i)==OK?1:0); 
	}
	Entropy(t);														//ϵͳ�����һ��΢�������´�ʱ�ġ��ء� 
//����tʱ��ϵͳ���ݣ�̬���أ���
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
