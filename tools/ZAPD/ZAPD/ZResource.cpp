#include "ZResource.h"

#include <cassert>
#include <regex>

#include "Utils/StringHelper.h"
#include "WarningHandler.h"
#include "ZFile.h"

ZResource::ZResource(ZFile* nParent)
{
	// assert(nParent != nullptr);
	parent = nParent;
	name = "";
	outName = "";
	sourceOutput = "";
	rawDataIndex = 0;
	outputDeclaration = true;

	RegisterRequiredAttribute("Name");
	RegisterOptionalAttribute("OutName");
	RegisterOptionalAttribute("Offset");
	RegisterOptionalAttribute("Custom");
}

void ZResource::ExtractFromXML(tinyxml2::XMLElement* reader, const std::vector<uint8_t>& nRawData,
                               const uint32_t nRawDataIndex)
{
	rawData = nRawData;
	rawDataIndex = nRawDataIndex;

	if (reader != nullptr)
		ParseXML(reader);

	ParseRawData();
	CalcHash();
}

void ZResource::ExtractFromFile(const std::vector<uint8_t>& nRawData, uint32_t nRawDataIndex)
{
	rawData = nRawData;
	rawDataIndex = nRawDataIndex;

	ParseRawData();
	CalcHash();
}

void ZResource::ParseXML(tinyxml2::XMLElement* reader)
{
	if (reader != nullptr)
	{
		// If it is an inner node, then 'Name' isn't required
		if (isInner)
		{
			registeredAttributes.at("Name").isRequired = false;
		}

		auto attrs = reader->FirstAttribute();
		while (attrs != nullptr)
		{
			std::string attrName = attrs->Name();
			bool attrDeclared = false;

			if (registeredAttributes.find(attrName) != registeredAttributes.end())
			{
				registeredAttributes[attrName].value = attrs->Value();
				registeredAttributes[attrName].wasSet = true;
				attrDeclared = true;
			}

			if (!attrDeclared)
			{
				HANDLE_WARNING_RESOURCE(
					WarningType::UnknownAttribute, parent, this, rawDataIndex,
					StringHelper::Sprintf("unexpected '%s' attribute in resource <%s>",
				                          attrName.c_str(), reader->Name()),
					"");
			}
			attrs = attrs->Next();
		}

		if (!canHaveInner && !reader->NoChildren())
		{
			std::string errorHeader = StringHelper::Sprintf(
				"resource '%s' with inner element/child detected", reader->Name());
			HANDLE_ERROR_PROCESS(WarningType::InvalidXML, errorHeader, "");
		}

		for (const auto& attr : registeredAttributes)
		{
			if (attr.second.isRequired && attr.second.value == "")
			{
				std::string headerMsg =
					StringHelper::Sprintf("missing required attribute '%s' in resource <%s>",
				                          attr.first.c_str(), reader->Name());
				HANDLE_ERROR_RESOURCE(WarningType::MissingAttribute, parent, this, rawDataIndex,
				                      headerMsg, "");
			}
		}

		name = registeredAttributes.at("Name").value;

		static std::regex r("[a-zA-Z_]+[a-zA-Z0-9_]*", std::regex::icase | std::regex::optimize);

		if (!isInner || (isInner && name != ""))
		{
			if (!std::regex_match(name, r))
			{
				HANDLE_ERROR_RESOURCE(WarningType::InvalidAttributeValue, parent, this,
				                      rawDataIndex, "invalid value found for 'Name' attribute", "");
			}
		}

		outName = registeredAttributes.at("OutName").value;
		if (outName == "")
			outName = name;

		isCustomAsset = registeredAttributes["Custom"].wasSet;

		std::string& staticXml = registeredAttributes["Static"].value;
		if (staticXml == "Global")
		{
			staticConf = StaticConfig::Global;
		}
		else if (staticXml == "On")
		{
			staticConf = StaticConfig::On;
		}
		else if (staticXml == "Off")
		{
			staticConf = StaticConfig::Off;
		}
		else
		{
			HANDLE_ERROR_RESOURCE(
				WarningType::InvalidAttributeValue, parent, this, rawDataIndex,
				StringHelper::Sprintf("invalid value '%s' for 'Static' attribute", staticConf), "");
		}

		declaredInXml = true;
	}
}

void ZResource::Save(const fs::path& outFolder)
{
}

void ZResource::PreGenSourceFiles()
{
}

const std::string& ZResource::GetName() const
{
	return name;
}

const std::string& ZResource::GetOutName() const
{
	return outName;
}

void ZResource::SetOutName(const std::string& nName)
{
	outName = nName;
}

void ZResource::SetName(const std::string& nName)
{
	name = nName;
}

bool ZResource::IsExternalResource() const
{
	return false;
}

bool ZResource::DoesSupportArray() const
{
	return false;
}

std::string ZResource::GetExternalExtension() const
{
	return "";
}

const std::vector<uint8_t>& ZResource::GetRawData() const
{
	return rawData;
}

void ZResource::SetRawData(const std::vector<uint8_t>& nData)
{
	rawData = nData;
}

bool ZResource::WasDeclaredInXml() const
{
	return declaredInXml;
}

uint32_t ZResource::GetRawDataIndex() const
{
	return rawDataIndex;
}

void ZResource::SetRawDataIndex(uint32_t value)
{
	rawDataIndex = value;
}

void ZResource::SetRawDataIndex(offset_t nRawDataIndex)
{
	rawDataIndex = nRawDataIndex;
}

std::string ZResource::GetBodySourceCode() const
{
	return "ERROR";
}

std::string ZResource::GetSourceOutputCode(const std::string& prefix)
{
	return "";
}

std::string ZResource::GetSourceOutputHeader(const std::string& prefix)
{
	return "";
}

void ZResource::GetSourceOutputCode([[maybe_unused]] const std::string& prefix)
{
}

	if (bodyStr != "ERROR")
	{
		Declaration* decl = parent->GetDeclaration(rawDataIndex);

		if (decl == nullptr || decl->isPlaceholder)
			decl = DeclareVar(prefix, bodyStr);
		else
			decl->text = bodyStr;

		decl->staticConf = staticConf;
	}
}

std::string ZResource::GetSourceTypeName() const
{
	return "u8";
}

ZResourceType ZResource::GetResourceType() const
{
	return ZResourceType::Error;
}

void ZResource::CalcHash()
{
	hash = 0;
}

void ZResource::SetInnerNode(bool inner)
{
	isInner = inner;
}

void ZResource::RegisterRequiredAttribute(const std::string& attr)
{
	ResourceAttribute resAtrr;
	resAtrr.key = attr;
	resAtrr.isRequired = true;
	registeredAttributes[attr] = resAtrr;
}

void ZResource::RegisterOptionalAttribute(const std::string& attr, const std::string& defaultValue)
{
	ResourceAttribute resAtrr;
	resAtrr.key = attr;
	resAtrr.value = defaultValue;
	registeredAttributes[attr] = resAtrr;
}

uint32_t Seg2Filespace(segptr_t segmentedAddress, uint32_t parentBaseAddress)
{
	uint32_t currentPtr = GETSEGOFFSET(segmentedAddress);

	if (GETSEGNUM(segmentedAddress) == 0x80)  // Is defined in code?
	{
		uint32_t parentBaseOffset = GETSEGOFFSET(parentBaseAddress);
		if (parentBaseOffset > currentPtr)
		{
			HANDLE_ERROR(WarningType::Always,
			             StringHelper::Sprintf(
							 "resource address 0x%08X is smaller than 'BaseAddress' 0x%08X",
							 segmentedAddress, parentBaseAddress),
			             "Maybe your 'BaseAddress' is wrong?");
		}

		currentPtr -= parentBaseOffset;
	}

	return currentPtr;
}
