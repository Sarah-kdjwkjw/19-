/*算法思想：
	#define{
		平面边界Length,Broad 
		“长方形”边长a,b 
		粒子数N 1000 
		time 10000 
	
	结构体数组State 						//浮点型 
		结构体存储体系状态 
	熵数组[time] 
	
	随机初态
	“求熵” 									//求熵的表征参数 
	循环 
		微扰函数								//n*fraction 	 
		重合判定
		求熵存至熵数组[time]
	
	输出终态坐标.txt 
	输出time-熵数据
*/
