#!/usr/bin/env python
# -*- coding: utf-8 -*-
from __future__ import unicode_literals

import CppHeaderParser
import re
from enum import Enum
from string import Template
import os
import datetime

"""
解析头文件，生成结构体的rtti（run time type info）
"""


class FiledType(Enum):
    """
    字段类型
    """
    FMT_INT = 0
    FMT_STR = 1
    FMT_DOUBLE = 2
    FMT_CHAR = 3
    FMT_NIL = 4


class Typedef:
    """
    描述头文件中的typedef
    """
    NameRegex = re.compile('([\w]+)(\[\d+\])?')
    IntTypeRegex = re.compile('u?int[0-9]*[_t]?')  # int类型正则表达式

    def __init__(self, name, type_info):
        m = self.NameRegex.match(name)
        self.type_info_str = type_info.strip()
        self.name = m.groups()[0]
        if self.type_info_str == 'char':
            if m.groups()[1]:
                self.type_info = FiledType.FMT_STR
            else:
                self.type_info = FiledType.FMT_CHAR
        elif self.type_info_str == 'double':
            self.type_info = FiledType.FMT_DOUBLE
        elif self.IntTypeRegex.match(self.type_info_str):
            self.type_info = FiledType.FMT_INT
        else:
            raise Exception("无法识别的类型:{}".format(self.type_info_str))


class HSDataTypeParser:
    """
    解析HSDataType.h文件
    """
    FileName = "HSDataType.h"

    def __init__(self, data_type_file_name=FileName):
        self.data_type_file_name = data_type_file_name
        self.parser = CppHeaderParser.CppHeader(self.data_type_file_name)
        self.typedefs = dict()
        self.__init_typedef__()

    def __init_typedef__(self):
        for name, type_info in self.parser.typedefs.items():
            t = Typedef(name, type_info)
            self.typedefs[t.name] = t
        self.typedefs['int32_t'] = Typedef('int32_t', 'int32_t')  # int32_t类型在头文件中没有typedef，所以在这里手工添加
        self.typedefs['char'] = Typedef('char', 'char')  # int32_t类型在头文件中没有typedef，所以在这里手工添加

    def __getitem__(self, key):
        return self.typedefs[key].type_info


class StructInfo:
    """
    生成一个struct的GetStructRtti的声明、定义
    """
    # GetStructRtti 函数声明
    GetRttiFunctionDeclarationTemplateStr = """struct ${struct_name}; 
TRADE_API_EXPORT CStructRtti& GetStructRtti(${struct_name}* S); // 获得结构体${struct_name}运行时类型信息
"""
    #
    GetRttiFunctionDeclarationTemplate = Template(GetRttiFunctionDeclarationTemplateStr)
    # GetStructRtti 函数定义
    GetRttiFunctionDefinitionTemplate = Template("""CStructRtti& GetStructRtti(${struct_name}* S)
{
    static CStructRttiImp ${rtti_name} (
    CStructRtti::FieldMetaListT({$definition_content}), 
    "${struct_name}",
    sizeof(${struct_name})
    );
    return ${rtti_name};
}
""")
    # operator<< 函数声明
    StructOutputOperatorDeclarationTemplateStr = """struct ${struct_name}; 
TRADE_API_EXPORT std::ostream& operator<<(std::ostream& Stream, const ${struct_name}* S); // 结构体${struct_name}的operator<<
"""
    StructOutputOperatorDeclarationTemplate = Template(StructOutputOperatorDeclarationTemplateStr)

    StructOutputOperatorDefinitionTemplate = Template("""std::ostream& operator<<(std::ostream& Stream, const ${struct_name}* S)
{    
    Stream << "${struct_name}" << ":";
    if(S)
    {
        ${struct_field_formatters};
    }
    return Stream;
}    
""")
    # 没有任何字段的结构体    
    EmptyStructOutputOperatorDefinitionTemplate = Template("""std::ostream& operator<<(std::ostream& Stream, const ${struct_name}* S)
{    
    Stream << "${struct_name}";
    return Stream;
}    
""")

    # #
    # RttiDeclarationTemplateStr = """extern CStructRtti& ${rtti_name}; // 结构体${struct_name}运行时类型信息\n"""

    # GetStructRtti 函数定义
    #     GetRttiFunctionDefinitionTemplateStr = """CStructRtti& GetStructRtti(${struct_name}* S)
    # {
    #     return ${rtti_name};
    # }
    # """

    class StructField:
        """
        结构体字段
        """

        @staticmethod
        def IsPassword(hs_field_type, field_name):
            """

            :param field_type: 字段类型
            :param field_name: 字段名称
            :return:
            """
            if "HSPassword" in hs_field_type:
                return True

        def __init__(self, cpp_struct_field, data_type_parser):
            """


            """
            self.struct_name = cpp_struct_field['property_of_class']
            self.field_name = cpp_struct_field['name']
            self.data_type_parser = data_type_parser
            self.hs_type_name = cpp_struct_field['type']
            self.cpp_field_type = self.data_type_parser[cpp_struct_field['type']]

        def field_rtti(self):
            return Template(
                """{"${field_name}", offsetof(struct ${struct_name}, ${field_name}), ${field_type}, sizeof(${struct_name}::${field_name})}""").substitute(
                struct_name=self.struct_name, field_name=self.field_name, field_type=self.cpp_field_type.name)

        def field_formatter(self):
            if self.IsPassword(self.hs_type_name, self.field_name):
                """密码是比较敏感的，不写入到日志"""
                return ""
            else:
                return Template("""StructMemberFormatter(Stream, "${field_name}", S->${field_name});""").substitute(
                    struct_name=self.struct_name, field_name=self.field_name)

    def __init__(self, cpp_class, data_type_parser):
        """
        :param data_type_parser HSDataTypeParser，用于转换类型
        :param cpp_class:  CppHeaderParser.CppHeaderParser.CppClass
        """
        self.data_type_parser = data_type_parser
        self.cpp_class = cpp_class  # c++的结构体或类
        self.struct_name = self.cpp_class['name']  # 结构体名称
        self.cpp_rtti_name = self.struct_name + "Rtti"  # 运行时类型信息名称
        self.get_rtti_declaration = None  # 声明
        self.get_rtti_definition = None  # 定义
        self.output_operator_declaration = None  # 声明
        self.output_operator_definition = None  # 定义
        self.struct_fields = [self.StructField(field, self.data_type_parser) for field in
                              self.cpp_class['properties']['public']]

    def generate_get_rtti_function(self):
        """
        生成声明和定义
        :return:
        """
        self.get_rtti_declaration = self.GetRttiFunctionDeclarationTemplate.substitute(rtti_name=self.cpp_rtti_name,
                                                                                       struct_name=self.struct_name)

        definition_content = ", \n".join(f.field_rtti() for f in self.struct_fields)
        self.get_rtti_definition = self.GetRttiFunctionDefinitionTemplate.substitute(rtti_name=self.cpp_rtti_name,
                                                                                     struct_name=self.struct_name,
                                                                                     definition_content=definition_content)

        return self.get_rtti_declaration, self.get_rtti_definition

    def generate_output_operator_function(self):
        """

        :return:
        """
        self.output_operator_declaration = self.StructOutputOperatorDeclarationTemplate.substitute(
            struct_name=self.struct_name)
        struct_field_formatters = "\n".join(f.field_formatter() for f in self.struct_fields)
        if len(self.struct_fields) > 0:
            self.output_operator_definition = self.StructOutputOperatorDefinitionTemplate.substitute(
                struct_name=self.struct_name, struct_field_formatters=struct_field_formatters)
        else:
            self.output_operator_definition = self.EmptyStructOutputOperatorDefinitionTemplate.substitute(
                struct_name=self.struct_name)
        return self.output_operator_declaration, self.output_operator_definition


class StructParser:
    """
    解析结构体
    """
    HeaderFileRegex = re.compile("(\w+)\.h")
    RTTI_HeaderFileTemplate = Template("${struct_name}_rtti.h")
    RTTI_SourceFileTemplate = Template("${struct_name}_rtti.cpp")

    IOOperator_HeaderFileTemplate = Template("${struct_name}_io.h")
    IOOperator_SourceFileTemplate = Template("${struct_name}_io.cpp")

    CaptionTemplateStr = """/**
* 本文件是由程序生成，请不要修改它
* Created on: ${date_time}
**/
"""
    # 头文件的头
    RttiHeaderFileHeaderTemplateStr = """
#ifndef IMPLEMENTATION_${business_name}_RTTI_H_
#define IMPLEMENTATION_${business_name}_RTTI_H_
#include "${ins_known_file_path}" // 声明TRADE_API_EXPORT的头文件
#include "${struct_rtti_file_path}"  // 声明CStructRtti的头文件
"""
    RttiHeaderFileTail = """
#endif
"""
    RttiHeaderFileTemplate = Template(
        CaptionTemplateStr + RttiHeaderFileHeaderTemplateStr + "${declaration}" + RttiHeaderFileTail)

    # 源文件的头
    RttiSourceFileHeaderTemplateStr = """
#include "${struct_rtti_imp_h}" // 声明CStructRttiImp的头文件
#include <cstddef>
#include "${struct_file_path}" // 结构体文件
#include "${struct_rtti_header_file_name}" // GetStructRtti头文件
"""

    RttiSourceFileTemplate = Template(CaptionTemplateStr + RttiSourceFileHeaderTemplateStr + "${definition}")

    NamespaceTemplate = Template("""namespace $namespace
{
$content
}
""")
    GetAllRttiDeclarationTemplate = Template("""
std::vector<CStructRtti> Get${business_name}AllStructRttiList();
""")
    GetAllRttiDefinitionTemplate = Template("""
    std::vector<CStructRtti> Get${business_name}AllStructRttiList();
    """)

    # 头文件的头
    IOHeaderFileHeaderTemplateStr = """
#ifndef IMPLEMENTATION_${business_name}_IO_H_
#define IMPLEMENTATION_${business_name}_IO_H_
#include "${ins_known_file_path}" // 声明TRADE_API_EXPORT的头文件
#include <ostream>  // 声明ostream的头文件
"""
    IOHeaderFileTail = """
#endif
"""
    IOHeaderFileTemplate = Template(
        CaptionTemplateStr + IOHeaderFileHeaderTemplateStr + "${declaration}" + IOHeaderFileTail)

    # 源文件的头
    IOSourceFileHeaderTemplateStr = """
#include "${struct_format_h}" // 声明StructMemberFormatter的头文件
#include "${struct_file_path}" // 结构体文件
#include "${struct_io_header_file_name}" // struct io头文件
"""

    IOSourceFileTemplate = Template(CaptionTemplateStr + IOSourceFileHeaderTemplateStr + "${definition}")

    def __init__(self, struct_file_name, data_type_parser, struct_rtti_source_path="./", struct_io_source_path="./",
                 struct_header_path="../Interface/api/",
                 struct_rtti_header_file_path="struct_rtti.h",
                 ins_known_file_path="ins_known.h",
                 struct_format_h="common/struct_helper/struct_rtti_imp.h",
                 struct_rtti_imp_h="common/struct_helper/struct_rtti_imp.h"):
        """

        :param struct_file_name: 结构体文件的名称
        :param data_type_parser:
        :param struct_header_path: 结构体头文件的路径，比如"include/stock_opt"表示的是股票期权的struct文件的路径、
        "include/derivative"表示的是股指期权的struct文件的路径
        :param struct_rtti_header_file_path: rtti 头文件的路径，一般该文件放在"include/common/struct_rtti.h"
        :param ins_known_file_path: ins_known.h文件的路径，一般该文件放在"include/common/ins_known.h"
        :param struct_rtti_source_path: 表示将生成的rtti source文件置于哪个路径
        """
        self.struct_file_name = struct_file_name  # 文件名

        self.struct_header_path = struct_header_path  # 路径
        self.struct_file_path = os.path.join(self.struct_header_path, self.struct_file_name).replace(os.path.sep,
                                                                                                     '/')  # 路径 + 文件名

        self.ins_known_file_path = ins_known_file_path  # 路径

        self.struct_rtti_source_path = struct_rtti_source_path  # 路径
        self.struct_io_source_path = struct_io_source_path  # 路径

        self.struct_format_h = struct_format_h
        self.struct_rtti_imp_h = struct_rtti_imp_h
        self.struct_rtti_file_path = struct_rtti_header_file_path  # 路径 + 文件名
        m = self.HeaderFileRegex.match(struct_file_name)
        if m.groups()[0]:
            self.business_name = m.groups()[0]  # 业务名称
            self.struct_rtti_header_file_name = os.path.join(self.struct_header_path,
                                                             self.RTTI_HeaderFileTemplate.substitute(
                                                                 struct_name=self.business_name)).replace(os.path.sep,
                                                                                                          '/')  # 运行时类型信息头文件
            self.struct_rtti_source_file_name = os.path.join(self.struct_rtti_source_path,
                                                             self.RTTI_SourceFileTemplate.substitute(
                                                                 struct_name=self.business_name)).replace(os.path.sep,
                                                                                                          '/')  # 运行时类型信息源代码文件
            self.struct_io_header_file_name = os.path.join(self.struct_header_path,
                                                           self.IOOperator_HeaderFileTemplate.substitute(
                                                               struct_name=self.business_name)).replace(os.path.sep,
                                                                                                        '/')

            self.struct_io_source_file_name = os.path.join(self.struct_io_source_path,
                                                           self.IOOperator_SourceFileTemplate.substitute(
                                                               struct_name=self.business_name)).replace(os.path.sep,
                                                                                                        '/')
        else:
            raise Exception("无法识别的头文件：{}".format(self.struct_file_path))
        self.parser = CppHeaderParser.CppHeader(self.struct_file_path)
        self.data_type_parser = data_type_parser

    def generate_cpp_rtti_file(self):
        """
        生成c++ rtti源代码文件
        :return:
        """
        with open(self.struct_rtti_header_file_name, mode='w') as struct_rtti_header_file, \
                open(self.struct_rtti_source_file_name, mode='w') as struct_rtti_source_file, \
                open(self.struct_io_header_file_name, mode='w') as struct_io_header_file, \
                open(self.struct_io_source_file_name, mode='w') as struct_io_source_file:
            struct_rtti_declarations = ""
            struct_rtti_definitions = ""
            struct_io_declarations = ""
            struct_io_definitions = ""

            namespace = ""
            for count, (struct_name, cpp_class) in enumerate(sorted(self.parser.classes.items())):
                s = StructInfo(cpp_class, self.data_type_parser)
                namespace = cpp_class['namespace']
                print(s.struct_name)
                struct_rtti_declaration, struct_rtti_definition = s.generate_get_rtti_function()
                struct_io_declaration, struct_io_definition = s.generate_output_operator_function()
                count_comment = """// 第{}个结构体\n""".format(count)

                struct_rtti_declarations += count_comment
                struct_rtti_declarations += struct_rtti_declaration

                struct_rtti_definitions += count_comment
                struct_rtti_definitions += struct_rtti_definition

                struct_io_declarations += count_comment
                struct_io_declarations += struct_io_declaration

                struct_io_definitions += count_comment
                struct_io_definitions += struct_io_definition

            if namespace:
                struct_rtti_declarations = self.NamespaceTemplate.substitute(namespace=namespace,
                                                                             content=struct_rtti_declarations)
                struct_rtti_definitions = self.NamespaceTemplate.substitute(namespace=namespace,
                                                                            content=struct_rtti_definitions)

                struct_io_declarations = self.NamespaceTemplate.substitute(namespace=namespace,
                                                                           content=struct_io_declarations)
                struct_io_definitions = self.NamespaceTemplate.substitute(namespace=namespace,
                                                                          content=struct_io_definitions)

            struct_rtti_header_file_content = self.RttiHeaderFileTemplate.substitute(
                struct_rtti_file_path=self.struct_rtti_file_path,
                ins_known_file_path=self.ins_known_file_path,
                date_time=datetime.datetime.now(),
                business_name=self.business_name,
                declaration=struct_rtti_declarations)

            struct_rtti_source_file_content = self.RttiSourceFileTemplate.substitute(date_time=datetime.datetime.now(),
                                                                                     struct_file_path=self.struct_file_path,
                                                                                     definition=struct_rtti_definitions,
                                                                                     struct_rtti_header_file_name=self.struct_rtti_header_file_name,
                                                                                     struct_rtti_imp_h=self.struct_rtti_imp_h)
            struct_rtti_header_file.write(struct_rtti_header_file_content)
            struct_rtti_source_file.write(struct_rtti_source_file_content)

            struct_io_header_file_content = self.IOHeaderFileTemplate.substitute(
                ins_known_file_path=self.ins_known_file_path,
                date_time=datetime.datetime.now(),
                business_name=self.business_name,
                declaration=struct_io_declarations)
            struct_io_source_file_content = self.IOSourceFileTemplate.substitute(date_time=datetime.datetime.now(),
                                                                                 struct_file_path=self.struct_file_path,
                                                                                 definition=struct_io_definitions,
                                                                                 struct_io_header_file_name=self.struct_io_header_file_name,
                                                                                 struct_format_h=self.struct_format_h)
            struct_io_header_file.write(struct_io_header_file_content)
            struct_io_source_file.write(struct_io_source_file_content)


def generate_trade_struct(api_header_file_path, struct_rtti_source_path, struct_io_source_path, struct_format_h,
                          struct_rtti_imp_h):
    """
    生成交易核心的rtti文件
    :param api_header_file_path:
    :return:
    """

    HSDataType_file_path = os.path.join(api_header_file_path, "HSDataType.h")
    data_type_p = HSDataTypeParser(HSDataType_file_path)

    common_struct_header_file_path = os.path.join(api_header_file_path, "common")
    common = StructParser("common_struct.h", data_type_p, struct_rtti_source_path=struct_rtti_source_path,
                          struct_io_source_path=struct_io_source_path,
                          struct_header_path=common_struct_header_file_path,
                          struct_format_h=struct_format_h,
                          struct_rtti_imp_h=struct_rtti_imp_h)
    common.generate_cpp_rtti_file()

    struct_rtti_file_path = "../common/struct_rtti.h"
    ins_known_file_path = "../common/ins_known.h"

    derivative_struct_header_path = os.path.join(api_header_file_path, "derivative")
    derivative = StructParser("derivative_struct.h", data_type_p, struct_rtti_source_path=struct_rtti_source_path,
                              struct_io_source_path=struct_io_source_path,
                              struct_header_path=derivative_struct_header_path,
                              struct_rtti_header_file_path=struct_rtti_file_path,
                              ins_known_file_path=ins_known_file_path,
                              struct_format_h=struct_format_h,
                              struct_rtti_imp_h=struct_rtti_imp_h)
    derivative.generate_cpp_rtti_file()

    stock_opt_struct_header_path = os.path.join(api_header_file_path, "stock_opt")
    stock_opt = StructParser("stock_opt_struct.h", data_type_p, struct_rtti_source_path=struct_rtti_source_path,
                             struct_io_source_path=struct_io_source_path,
                             struct_header_path=stock_opt_struct_header_path,
                             struct_rtti_header_file_path=struct_rtti_file_path,
                             ins_known_file_path=ins_known_file_path,
                             struct_format_h=struct_format_h,
                             struct_rtti_imp_h=struct_rtti_imp_h)
    stock_opt.generate_cpp_rtti_file()
    
    
    stock_struct_header_path = os.path.join(api_header_file_path, "stock")
    stock = StructParser("stock_struct.h", data_type_p, struct_rtti_source_path=struct_rtti_source_path,
                             struct_io_source_path=struct_io_source_path,
                             struct_header_path=stock_struct_header_path,
                             struct_rtti_header_file_path=struct_rtti_file_path,
                             ins_known_file_path=ins_known_file_path,
                             struct_format_h=struct_format_h,
                             struct_rtti_imp_h=struct_rtti_imp_h)
    stock.generate_cpp_rtti_file()
    
    gold_struct_header_path = os.path.join(api_header_file_path, "gold")
    gold = StructParser("gold_struct.h", data_type_p, struct_rtti_source_path=struct_rtti_source_path,
                             struct_io_source_path=struct_io_source_path,
                             struct_header_path=gold_struct_header_path,
                             struct_rtti_header_file_path=struct_rtti_file_path,
                             ins_known_file_path=ins_known_file_path,
                             struct_format_h=struct_format_h,
                             struct_rtti_imp_h=struct_rtti_imp_h)
    gold.generate_cpp_rtti_file()

    # DataType_file_path = os.path.join(api_header_file_path, "union_risk_control/data_type.h")
    # data_type_p = HSDataTypeParser(DataType_file_path)

    # union_risk_control_struct_header_path = os.path.join(api_header_file_path, "union_risk_control")
    # union_risk_control = StructParser("union_risk_control_struct.h", data_type_p,
    #                                   struct_rtti_source_path=struct_rtti_source_path,
    #                                   struct_io_source_path=struct_io_source_path,
    #                                   struct_header_path=union_risk_control_struct_header_path,
    #                                   struct_rtti_header_file_path=struct_rtti_file_path,
    #                                   ins_known_file_path=ins_known_file_path,
    #                                   struct_format_h=struct_format_h,
    #                                   struct_rtti_imp_h=struct_rtti_imp_h)
    # union_risk_control.generate_cpp_rtti_file()


def generate_am_ust_trade_struct():
    api_header_file_path = "../../../include/"
    struct_rtti_source_path = '../../trade_api/struct_rtti'
    struct_io_source_path = '../../trade_api/struct_io'
    struct_format_h = "api/api_impl/common/struct_helper/struct_format.h"
    struct_rtti_imp_h = "api/api_impl/common/struct_helper/struct_rtti_imp.h"
    generate_trade_struct(api_header_file_path, struct_rtti_source_path, struct_io_source_path, struct_format_h, struct_rtti_imp_h)


def generate_stock_opt_ust_trade_struct():
    api_header_file_path = "../../../../../include/api"
    struct_rtti_source_path = '../../HSTradeApi/struct_rtti'
    struct_io_source_path = '../../HSTradeApi/struct_io'
    struct_format_h = "common/struct_helper/struct_format.h"
    struct_rtti_imp_h = "common/struct_helper/struct_rtti_imp.h"
    generate_trade_struct(api_header_file_path, struct_rtti_source_path, struct_io_source_path, struct_format_h, struct_rtti_imp_h)


if __name__ == "__main__":
    generate_am_ust_trade_struct()
    generate_stock_opt_ust_trade_struct()
