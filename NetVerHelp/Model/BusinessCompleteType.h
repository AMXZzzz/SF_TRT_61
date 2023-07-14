#pragma once
#ifndef BusinessCompleteType_H_
#define BusinessCompleteType_H_
namespace Model
{
	//订单业务完成状态
	enum BusinessCompleteType
	{
		// 等待完成
		WaitComplete = 0,
		//成功
		Success = 1,
		//失败
		Fail = 2,
	};
}
#endif