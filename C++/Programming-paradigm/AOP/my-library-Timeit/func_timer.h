#pragma once

#include <type_traits> // std::result_of
#include <list> // std::list
#include <string>
#include <utility>      // std::pair
#include <fstream>

#include "common.h"

//class CScopedTimer;
/**
 * 打点计时器
 */
namespace TickerTapTimer
{
/**
 * @brief 对函数的执行耗时进行统计
 *
 */
template<typename ClockType = SyscallClock>
class CFuncTimer
{
	//friend class CScopedTimer;
public:
	using TickerType = CTicker<ClockType>;
	/**
	 * @brief
	 *
	 * @param Enable 是否开启即使
	 */
	CFuncTimer(bool Enable) :
			m_Enable { Enable }
	{

	}
	/**
	 * @brief 统计函数f的执行耗时
	 *
	 * @tparam F
	 * @tparam Args
	 * @param f
	 * @param args
	 * @return
	 */
	template<typename F, typename ... Args>
	inline auto Stat(F &&f, Args &&... args) -> typename std::result_of<F(Args...)>::type
	{
		using return_type = typename std::result_of<F(Args...)>::type;
		if (m_Enable)
		{
			TickBegin();
			return_type ret = f(args...);
			TickEnd();
			return ret;
		}
		else
		{
			return f(args...);
		}

	}
	/**
	 * 写入到CSV文件中
	 * @param TickerTapTimer
	 * @return
	 */
	int WriteToCSV(std::string TickFileName)
	{
		std::ofstream TickFileStream;
		Open(TickFileStream, TickFileName);
		TickFileStream << "Begin,End" << "\n";
		int RowCount = 0;
		for (auto &&Row : m_Rows)
		{
			TickFileStream << Row.first << "," << Row.second << "\n";
			RowCount++;
		}
		TickFileStream.close();
		return RowCount;
	}
private:
	/**
	 * @brief 函数开始执行打点
	 *
	 */
	void TickBegin()
	{
		m_Begin = TickerType().Str();
	}
	/**
	 * @brief 函数结束执行打点
	 *
	 */
	void TickEnd()
	{
		m_End = TickerType().Str();
		m_Rows.emplace_back(std::move(m_Begin), std::move(m_End));
	}
	bool Open(std::ofstream &Stream, std::string &FileName, bool Append = false)
	{
		if (Append)
			Stream.open(FileName.c_str(), std::ios::out | std::ios::app);
		else
			Stream.open(FileName.c_str(), std::ios::out | std::ios::trunc);
		if (!Stream.is_open())
		{
			return false;
		}
		else
		{
			if (Append)
				Stream << std::endl;
			return true;
		}
	}
private:
	bool m_Enable { false };
	std::string m_Begin;
	std::string m_End;
	using BeginEndPair = std::pair<std::string, std::string>;
	std::list<BeginEndPair> m_Rows;
};

///**
// * @brief 基于scope的函数执行耗时统计
// *
// */
//class CScopedTimer
//{
//public:
//	CScopedTimer(CFuncTimer *FuncTimer) :
//			m_FuncTimer(FuncTimer)
//	{
//		if (m_FuncTimer)
//		{
//			m_FuncTimer->TickBegin();
//		}
//	}
//	~CScopedTimer()
//	{
//		if (m_FuncTimer)
//		{
//			m_FuncTimer->TickEnd();
//		}
//	}
//private:
//	CFuncTimer *m_FuncTimer;
//};

}

