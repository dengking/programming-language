#pragma once

#include <sstream>
#include <string>

template<typename OStreamType>
static inline OStreamType& StructMemberFormatter(OStreamType& OStream, const char* MemberName, int MemberValue)
{
	OStream << "[I]" << MemberName << "=" << MemberValue;
	return OStream;
}

/**
 * 格式化结构体的成员变量
 */
static inline std::string StructMemberFormatter(const char* MemberName, int MemberValue)
{
	std::stringstream Stream;
	StructMemberFormatter(Stream, MemberName, MemberValue);
	return Stream.str();
}

template<typename OStreamType>
static inline OStreamType& StructMemberFormatter(OStreamType& OStream, const char* MemberName, char MemberValue)
{
	OStream << "[C]" << MemberName << "=" << MemberValue;
	return OStream;
}

static inline std::string StructMemberFormatter(const char* MemberName, char MemberValue)
{
	std::stringstream Stream;
	StructMemberFormatter(Stream, MemberName, MemberValue);
	return Stream.str();
}

template<typename OStreamType>
static inline OStreamType& StructMemberFormatter(OStreamType& OStream, const char* MemberName, const char* MemberValue)
{
	OStream << "[S]" << MemberName << "=" << MemberValue;
	return OStream;
}

static inline std::string StructMemberFormatter(const char* MemberName, char* MemberValue)
{
	std::stringstream Stream;
	StructMemberFormatter(Stream, MemberName, MemberValue);
	return Stream.str();
}

template<typename OStreamType>
static inline OStreamType& StructMemberFormatter(OStreamType& OStream, const char* MemberName, double MemberValue)
{
	OStream << "[D]" << MemberName << "=" << MemberValue;
	return OStream;
}

static inline std::string StructMemberFormatter(const char* MemberName, double MemberValue)
{
	std::stringstream Stream;
	StructMemberFormatter(Stream, MemberName, MemberValue);
	return Stream.str();
}

