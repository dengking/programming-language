/*
 * ust_api.h
 *
 *  Created on: 2020Äê4ÔÂ17ÈÕ
 *      Author: dengkai17334
 */

#ifndef IMPLEMENTATION_CORE_UST_API_H_
#define IMPLEMENTATION_CORE_UST_API_H_

#include <string>

template<int ApiFunctionNo, typename ApiReqT, typename ApiRspT>
class UstApi
{
public:
//	static std::string Name;
	static int FunctionNo;
	using ReqT = ApiReqT;
	using RspT = ApiRspT;
};
template<int ApiFunctionNo, typename ApiReqT, typename ApiRspT>
int UstApi<ApiFunctionNo, ApiReqT, ApiRspT>::FunctionNo = ApiFunctionNo;

//template<int ApiFunctionNo, typename ApiReqT, typename ApiRspT, std::string ApiName>
//int UstApi<ApiFunctionNo, ApiReqT, ApiRspT>::Name = ApiName;
#endif /* IMPLEMENTATION_CORE_UST_API_H_ */
