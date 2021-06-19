#pragma once

#include "struct_rtti.h"


template<typename StructT>
CStructRtti& GetStructRttiByType()
{
	return GetStructRtti((StructT*)nullptr);
}
