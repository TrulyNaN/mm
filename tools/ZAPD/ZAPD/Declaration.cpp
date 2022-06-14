#include "Declaration.h"

Declaration::Declaration(DeclarationAlignment nAlignment, DeclarationPadding nPadding, size_t nSize,
                         std::string nText)
{
	alignment = nAlignment;
	padding = nPadding;
	size = nSize;
	text = nText;
}

Declaration::Declaration(DeclarationAlignment nAlignment, size_t nSize, std::string nVarType,
                         std::string nVarName, bool nIsArray, std::string nText)
	: Declaration(nAlignment, DeclarationPadding::None, nSize, nText)
{
	varType = nVarType;
	varName = nVarName;
	isArray = nIsArray;
}

Declaration::Declaration(DeclarationAlignment nAlignment, DeclarationPadding nPadding, size_t nSize,
                         std::string nVarType, std::string nVarName, bool nIsArray,
                         std::string nText)
	: Declaration(nAlignment, nPadding, nSize, nText)
{
	varType = nVarType;
	varName = nVarName;
	isArray = nIsArray;
}

Declaration::Declaration(DeclarationAlignment nAlignment, size_t nSize, std::string nVarType,
                         std::string nVarName, bool nIsArray, size_t nArrayItemCnt,
                         std::string nText)
	: Declaration(nAlignment, DeclarationPadding::None, nSize, nText)
{
	varType = nVarType;
	varName = nVarName;
	isArray = nIsArray;
	arrayItemCnt = nArrayItemCnt;
}

Declaration::Declaration(DeclarationAlignment nAlignment, size_t nSize, std::string nVarType,
                         std::string nVarName, bool nIsArray, std::string nArrayItemCntStr,
                         std::string nText)
	: Declaration(nAlignment, DeclarationPadding::None, nSize, nText)
{
	varType = nVarType;
	varName = nVarName;
	isArray = nIsArray;
	arrayItemCntStr = nArrayItemCntStr;
}

bool Declaration::IsStatic() const
{
	switch (staticConf)
	{
	case StaticConfig::Off:
		return false;

	case StaticConfig::Global:
		return Globals::Instance->forceStatic;

	case StaticConfig::On:
		return true;
	}

	return false;
}

std::string Declaration::GetNormalDeclarationStr() const
{
	std::string output;

	if (preText != "")
		output += preText + "\n";

	if (IsStatic())
	{
		output += "static ";
	}

	if (isArray)
	{
		if (arrayItemCntStr != "" && (IsStatic() || forceArrayCnt))
		{
			output += StringHelper::Sprintf("%s %s[%s];\n", varType.c_str(), varName.c_str(),
			                                arrayItemCntStr.c_str());
		}
		else if (arrayItemCnt != 0 && (IsStatic() || forceArrayCnt))
		{
			output += StringHelper::Sprintf("%s %s[%i] = {\n", varType.c_str(), varName.c_str(),
			                                arrayItemCnt);
		}
		else
		{
			output += StringHelper::Sprintf("%s %s[] = {\n", varType.c_str(), varName.c_str());
		}

		output += text + "\n";
	}
	else
	{
		output += StringHelper::Sprintf("%s %s = { ", varType.c_str(), varName.c_str());
		output += text;
	}

	if (output.back() == '\n')
		output += "};";
	else
		output += " };";

	if (rightText != "")
		output += " " + rightText + "";

	output += "\n";

	if (postText != "")
		output += postText + "\n";

	output += "\n";

	return output;
}

std::string Declaration::GetExternalDeclarationStr() const
{
	std::string output;

	if (preText != "")
		output += preText + "\n";

	if (IsStatic())
	{
		output += "static ";
	}

	if (arrayItemCntStr != "" && (IsStatic() || forceArrayCnt))
		output += StringHelper::Sprintf("%s %s[%s] = ", varType.c_str(), varName.c_str(),
		                                arrayItemCntStr.c_str());
	else if (arrayItemCnt != 0 && (IsStatic() || forceArrayCnt))
		output +=
			StringHelper::Sprintf("%s %s[%i] = ", varType.c_str(), varName.c_str(), arrayItemCnt);
	else
		output += StringHelper::Sprintf("%s %s[] = ", varType.c_str(), varName.c_str());

	output += StringHelper::Sprintf("{\n#include \"%s\"\n};", includePath.c_str());

	if (rightText != "")
		output += " " + rightText + "";

	output += "\n";

	if (postText != "")
		output += postText + "\n";

	output += "\n";

	return output;
}

Declaration::Declaration(DeclarationAlignment nAlignment, DeclarationPadding nPadding, size_t nSize,
                         std::string nVarType, std::string nVarName, bool nIsArray,
                         size_t nArrayItemCnt, std::string nText)
	: Declaration(nAlignment, nPadding, nSize, nText)
{
	if (IsStatic() || varType == "" || isUnaccounted)
	{
		return "";
	}

	if (isArray)
	{
		if (arrayItemCntStr != "" && (IsStatic() || forceArrayCnt))
		{
			return StringHelper::Sprintf("extern %s %s[%s];\n", varType.c_str(), varName.c_str(),
			                             arrayItemCntStr.c_str());
		}
		else if (arrayItemCnt != 0 && (IsStatic() || forceArrayCnt))
		{
			return StringHelper::Sprintf("extern %s %s[%i];\n", varType.c_str(), varName.c_str(),
			                             arrayItemCnt);
		}
		else
			return StringHelper::Sprintf("extern %s %s[];\n", varType.c_str(), varName.c_str());
	}

	return StringHelper::Sprintf("extern %s %s;\n", varType.c_str(), varName.c_str());
}

Declaration::Declaration(std::string nIncludePath, size_t nSize, std::string nVarType,
                         std::string nVarName)
	: Declaration(DeclarationAlignment::None, DeclarationPadding::None, nSize, "")
{
	includePath = nIncludePath;
	varType = nVarType;
	varName = nVarName;
}
