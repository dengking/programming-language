#include<iostream>
template<int FunctionNo, typename ReqT, typename RspT, int T=10>
class UstApi
{
public:
	static int FuncNo;
	using Req = ReqT;
	using Rsp = RspT;
};
template<int FunctionNo, typename ReqT, typename RspT, int T>
int UstApi<FunctionNo, ReqT, RspT, T>::FuncNo = FunctionNo;

class Req{};
class Rsp{};

typedef UstApi<100, Req, Rsp> MyUstApi; 
int main(){
std::cout<<MyUstApi::FuncNo<<std::endl;

}
