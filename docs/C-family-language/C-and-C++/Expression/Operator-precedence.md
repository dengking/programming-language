# Operator Precedence

记住一些常见的operator precedence是理解一些expression的前提。

## cppreference [C++ Operator Precedence](https://en.cppreference.com/w/cpp/language/operator_precedence)



> NOTE: 下面总结的一些规律
>
> ### [Scope resolution](https://en.cppreference.com/w/cpp/language/identifiers#Qualified_identifiers) **Precedence** 最高
>
> 静态的
>
> ### 创建 temporary 和 member access的优先级次之
>
> 创建temporary的: 
>
> | operator          | 解释                                                         |                     |
> | ----------------- | ------------------------------------------------------------ | ------------------- |
> | `a++   a--`       | Suffix/postfix [increment and decrement](https://en.cppreference.com/w/cpp/language/operator_incdec) | 会创建一个temporary |
> | `type()   type{}` | [Functional cast](https://en.cppreference.com/w/cpp/language/explicit_cast) | 会创建一个temporary |
>
> member access:
>
> | operator |                                                              |
> | -------- | ------------------------------------------------------------ |
> | `a[]`    | [Subscript](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_subscript_operator) |
> | `.   ->` | [Member access](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_member_access_operators) |
>
> 



## Example



### sqlite CSV virtual table

https://sqlite.org/src/file/ext/misc/csv.c

取成员变量的地址: `&obj->member`，`->`的优先级高于`&`

```C
static int csvtabOpen(sqlite3_vtab *p, sqlite3_vtab_cursor **ppCursor){
  CsvTable *pTab = (CsvTable*)p;
  CsvCursor *pCur;
  size_t nByte;
  nByte = sizeof(*pCur) + (sizeof(char*)+sizeof(int))*pTab->nCol;
  pCur = sqlite3_malloc64( nByte );
  if( pCur==0 ) return SQLITE_NOMEM;
  memset(pCur, 0, nByte);
  pCur->azVal = (char**)&pCur[1];
  pCur->aLen = (int*)&pCur->azVal[pTab->nCol];
  *ppCursor = &pCur->base;
  if( csv_reader_open(&pCur->rdr, pTab->zFilename, pTab->zData) ){
    csv_xfer_error(pTab, &pCur->rdr);
    return SQLITE_ERROR;
  }
  return SQLITE_OK;
}
```

