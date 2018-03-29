/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Ray Garner $
   ======================================================================== */

#if !defined(API_META_H)

char* WhiteSpaceText = " ";
char* StructText = "struct";
char* OpenBraceText = "{";
char* CloseBraceText = "}";
char* SemiColonText = ";";
char* NewLineText = "\n";
char* CTypeCharPtrText = "char*";
char* EqualsText = "=";

APIDEF var_type GetTypeForString(string* String)
{
    return var_type_string;
}

APIDEF string* GetStringForType(var_type VarType,memory_partition* StringPartition)
{

    string* Result;
    switch(VarType)
    {
        case var_type_string:
        {
            Result = CreateStringFromLiteral("string",StringPartition);
        }break;
    }
    return Result;
    
}

APIDEF void StartBlock(string* StringToUse,memory_partition* StringPartition)
{
    AppendCharToStringAndAdvace(StringToUse, NewLineText , StringPartition);
    AppendCharToStringAndAdvace(StringToUse, OpenBraceText , StringPartition);
    AppendCharToStringAndAdvace(StringToUse, NewLineText , StringPartition);
}

APIDEF void EndBlock(string* StringToUse,memory_partition* StringPartition)
{
    AppendCharToStringAndAdvace(StringToUse, CloseBraceText , StringPartition);
    AppendCharToStringAndAdvace(StringToUse, NewLineText, StringPartition);
}
// 
APIDEF void DeclareAndAssignVariable(string* StringToUse,string* VarName, string* Value,memory_partition* StringPartition)
{

    var_type VarType = GetTypeForString(Value);
//    string* 
    AppendCharToStringAndAdvace(StringToUse, CTypeCharPtrText , StringPartition);
    AppendCharToStringAndAdvace(StringToUse, WhiteSpaceText, StringPartition);
    AppendCharToStringAndAdvace(StringToUse, VarName->String , StringPartition);
    AppendCharToStringAndAdvace(StringToUse, WhiteSpaceText , StringPartition);
    AppendCharToStringAndAdvace(StringToUse, EqualsText , StringPartition);
    AppendCharToStringAndAdvace(StringToUse, Value->String , StringPartition);
    AppendCharToStringAndAdvace(StringToUse, SemiColonText , StringPartition);
    AppendCharToStringAndAdvace(StringToUse, NewLineText , StringPartition);

}

APIDEF void StartStructBlock(string* StringToUse,string StructName,memory_partition *StringPartition)
{
    
    AppendCharToStringAndAdvace(StringToUse, StructText , StringPartition);
    AppendCharToStringAndAdvace(StringToUse, WhiteSpaceText , StringPartition);

    AppendStringAndAdvance(StringToUse, StructName , StringPartition);

    StartBlock(StringToUse,StringPartition);
}

#define API_META_H
#endif