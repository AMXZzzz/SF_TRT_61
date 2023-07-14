#pragma once
#ifndef OrderState_H_
#define OrderState_H_
namespace Model
{
	//订单支付状态
	enum OrderState
	{
		// 支付中
		WaitPay = 0,
		//支付成功
		PaySuccess = 1,
		//支付未完成
		PayNoComplete = 2,
		//支付失败
		PayError = 3,
	};
}
#endif