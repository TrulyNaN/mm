#include "ZSkeleton.h"

#include <cassert>

#include "Globals.h"
#include "Utils/BitConverter.h"
#include "Utils/StringHelper.h"
#include "WarningHandler.h"

REGISTER_ZFILENODE(Skeleton, ZSkeleton);

ZSkeleton::ZSkeleton(ZFile* nParent) : ZResource(nParent)
{
	type = ZSkeletonType::Normal;
	limbType = ZLimbType::Standard;
	dListCount = 0;

	RegisterRequiredAttribute("Type");
	RegisterRequiredAttribute("LimbType");
	RegisterOptionalAttribute("EnumName");
	RegisterOptionalAttribute("LimbNone");
	RegisterOptionalAttribute("LimbMax");
}

ZSkeleton::ZSkeleton(ZSkeletonType nType, ZLimbType nLimbType, const std::string& prefix,
                     const std::vector<uint8_t>& nRawData, uint32_t nRawDataIndex, ZFile* nParent)
	: ZSkeleton(nParent)
{
	rawData.assign(nRawData.begin(), nRawData.end());
	rawDataIndex = nRawDataIndex;
	parent = nParent;

	name = StringHelper::Sprintf("%sSkel_%06X", prefix.c_str(), rawDataIndex);
	type = nType;
	limbType = nLimbType;

	ParseRawData();

	std::string defaultPrefix = name;
	defaultPrefix.replace(0, 1, "s");  // replace g prefix with s for local variables
	uint32_t ptr = Seg2Filespace(limbsArrayAddress, parent->baseAddress);

	for (size_t i = 0; i < limbCount; i++)
	{
		uint32_t ptr2 = Seg2Filespace(BitConverter::ToUInt32BE(rawData, ptr), parent->baseAddress);

		ZLimb* limb = new ZLimb(limbType, prefix, rawData, ptr2, parent);
		limbs.push_back(limb);

		ptr += 4;
	}
}

ZSkeleton::~ZSkeleton()
{
	for (auto& limb : limbs)
		delete limb;
}

void ZSkeleton::ParseXML(tinyxml2::XMLElement* reader)
{
	ZResource::ParseXML(reader);

	std::string skelTypeXml = registeredAttributes.at("Type").value;

	if (skelTypeXml == "Flex")
		type = ZSkeletonType::Flex;
	else if (skelTypeXml == "Curve")
		type = ZSkeletonType::Curve;
	else if (skelTypeXml != "Normal")
	{
		HANDLE_ERROR_RESOURCE(WarningType::InvalidAttributeValue, parent, this, rawDataIndex,
		                      "invalid value found for 'Type' attribute", "");
	}

	std::string limbTypeXml = registeredAttributes.at("LimbType").value;

	if (limbTypeXml == "Standard")
		limbType = ZLimbType::Standard;
	else if (limbTypeXml == "LOD")
		limbType = ZLimbType::LOD;
	else if (limbTypeXml == "Skin")
		limbType = ZLimbType::Skin;
	else if (limbTypeXml == "Curve")
		limbType = ZLimbType::Curve;
	else
	{
		HANDLE_ERROR_RESOURCE(
			WarningType::InvalidAttributeValue, parent, this, rawDataIndex,
			StringHelper::Sprintf("invalid value '%s' found for 'LimbType' attribute",
		                          limbTypeXml.c_str()),
			"Defaulting to 'Standard'.");
	}

	enumName = registeredAttributes.at("EnumName").value;
	limbNoneName = registeredAttributes.at("LimbNone").value;
	limbMaxName = registeredAttributes.at("LimbMax").value;

	if (enumName != "")
	{
		if (limbNoneName == "" || limbMaxName == "")
		{
			HANDLE_ERROR_RESOURCE(WarningType::MissingAttribute, parent, this, rawDataIndex,
			                      "'EnumName' attribute was used but either 'LimbNone' or "
			                      "'LimbMax' attribute is missing",
			                      "");
		}
	}

	if (limbNoneName != "")
	{
		if (limbMaxName == "")
		{
			HANDLE_ERROR_RESOURCE(
				WarningType::MissingAttribute, parent, this, rawDataIndex,
				"'LimbNone' attribute was used but 'LimbMax' attribute is missing", "");
		}
	}

	if (limbMaxName != "")
	{
		if (limbNoneName == "")
		{
			HANDLE_ERROR_RESOURCE(
				WarningType::MissingAttribute, parent, this, rawDataIndex,
				"'LimbMax' attribute was used but 'LimbNone' attribute is missing", "");
		}
	}
}

void ZSkeleton::ParseRawData()
{
	ZResource::ParseRawData();

	limbsArrayAddress = BitConverter::ToUInt32BE(rawData, rawDataIndex);
	limbCount = BitConverter::ToUInt8BE(rawData, rawDataIndex + 4);

	if (type == ZSkeletonType::Flex)
	{
		dListCount = BitConverter::ToUInt8BE(rawData, rawDataIndex + 8);
	}
}

void ZSkeleton::ExtractFromXML(tinyxml2::XMLElement* reader, const std::vector<uint8_t>& nRawData,
                               const uint32_t nRawDataIndex)
{
	ZResource::ExtractFromXML(reader, nRawData, nRawDataIndex);

	parent->AddDeclaration(rawDataIndex, DeclarationAlignment::Align16, GetRawDataSize(),
	                       GetSourceTypeName(), name, "");

	std::string defaultPrefix = name;
	defaultPrefix.replace(0, 1, "s");  // replace g prefix with s for local variables
	uint32_t ptr = Seg2Filespace(limbsArrayAddress, parent->baseAddress);

	if (limbsArrayAddress != SEGMENTED_NULL && GETSEGNUM(limbsArrayAddress) == parent->segment)
	{
		offset_t ptr = Seg2Filespace(limbsArrayAddress, parent->baseAddress);

		if (!parent->HasDeclaration(ptr))
		{
			limbsTable = new ZLimbTable(parent);
			limbsTable->ExtractFromBinary(ptr, limbType, limbCount);
			limbsTable->SetName(StringHelper::Sprintf("%sLimbs", defaultPrefix.c_str()));
			parent->AddResource(limbsTable);
		}
		else
		{
			limbsTable = static_cast<ZLimbTable*>(parent->FindResource(ptr));
		}

		if (limbsTable->enumName == "")
		{
			limbsTable->enumName = enumName;
		}
		if (limbsTable->limbNoneName == "")
		{
			limbsTable->limbNoneName = limbNoneName;
		}
		if (limbsTable->limbMaxName == "")
		{
			limbsTable->limbMaxName = limbMaxName;
		}
	}
}

		std::string limbName = StringHelper::Sprintf("%sLimb_%06X", defaultPrefix.c_str(), ptr2);
		Declaration* decl = parent->GetDeclaration(ptr2);
		if (decl != nullptr)
			limbName = decl->varName;

	std::string countStr;
	assert(limbsTable != nullptr);
	// There are some Skeletons with the wrong limb count on them, so this check is necessary.
	if (limbsTable->count == limbCount)
	{
		countStr = StringHelper::Sprintf("ARRAY_COUNT(%s)", limbArrayName.c_str());
	}
	else
	{
		countStr = StringHelper::Sprintf("%i", limbCount);
	}

	switch (type)
	{
	case ZSkeletonType::Normal:
	case ZSkeletonType::Curve:
		return StringHelper::Sprintf("\n\t%s, %s\n", limbArrayName.c_str(), countStr.c_str());

	case ZSkeletonType::Flex:
		return StringHelper::Sprintf("\n\t{ %s, %s }, %i\n", limbArrayName.c_str(),
		                             countStr.c_str(), dListCount);
	}
}

void ZSkeleton::GenerateHLIntermediette(HLFileIntermediette& hlFile)
{
	HLModelIntermediette* mdl = (HLModelIntermediette*)&hlFile;
	HLModelIntermediette::FromZSkeleton(mdl, this);
	mdl->blocks.push_back(new HLTerminator());
}

size_t ZSkeleton::GetRawDataSize() const
{
	switch (type)
	{
	case ZSkeletonType::Flex:
		return 0xC;
	case ZSkeletonType::Normal:
	case ZSkeletonType::Curve:
	default:
		return 0x8;
	}
}

std::string ZSkeleton::GetSourceOutputCode(const std::string& prefix)
{
	if (parent == nullptr)
		return "";

	std::string defaultPrefix = name.c_str();
	defaultPrefix.replace(0, 1, "s");  // replace g prefix with s for local variables

DeclarationAlignment ZSkeleton::GetDeclarationAlignment() const
{
	return DeclarationAlignment::Align4;
}

	uint32_t ptr = Seg2Filespace(limbsArrayAddress, parent->baseAddress);
	if (!parent->HasDeclaration(ptr))
	{
		// Table
		std::string tblStr = "";
		std::string limbArrTypeStr = "static void*";
		if (limbType == ZLimbType::Curve)
		{
			limbArrTypeStr =
				StringHelper::Sprintf("static %s*", ZLimb::GetSourceTypeName(limbType));
		}

ZLimbTable::ZLimbTable(ZFile* nParent) : ZResource(nParent)
{
	RegisterRequiredAttribute("LimbType");
	RegisterRequiredAttribute("Count");
	RegisterOptionalAttribute("EnumName");
	RegisterOptionalAttribute("LimbNone");
	RegisterOptionalAttribute("LimbMax");
}

			std::string decl = StringHelper::Sprintf(
				"    &%s,", parent->GetDeclarationName(limb->GetFileAddress()).c_str());
			if (i != (limbs.size() - 1))
			{
				decl += "\n";
			}

			tblStr += decl;
		}

		parent->AddDeclarationArray(ptr, DeclarationAlignment::None, 4 * limbCount, limbArrTypeStr,
		                            StringHelper::Sprintf("%sLimbs", defaultPrefix.c_str()),
		                            limbCount, tblStr);
	}

	std::string headerStr;
	switch (type)
	{
		HANDLE_WARNING_RESOURCE(WarningType::InvalidAttributeValue, parent, this, rawDataIndex,
		                        "invalid value found for 'LimbType' attribute.",
		                        "Defaulting to 'Standard'.");
		limbType = ZLimbType::Standard;
	}

	count = StringHelper::StrToL(registeredAttributes.at("Count").value);

	enumName = registeredAttributes.at("EnumName").value;
	limbNoneName = registeredAttributes.at("LimbNone").value;
	limbMaxName = registeredAttributes.at("LimbMax").value;

	if (enumName != "")
	{
		if (limbNoneName == "" || limbMaxName == "")
		{
			HANDLE_ERROR_RESOURCE(
				WarningType::MissingAttribute, parent, this, rawDataIndex,
				"'EnumName' attribute was used but 'LimbNone'/'LimbMax' attributes is missing", "");
		}
	}

	if (limbNoneName != "")
	{
		if (limbMaxName == "")
		{
			HANDLE_ERROR_RESOURCE(
				WarningType::MissingAttribute, parent, this, rawDataIndex,
				"'LimbNone' attribute was used but 'LimbMax' attribute is missing", "");
		}
	}

	if (limbMaxName != "")
	{
		if (limbNoneName == "")
		{
			HANDLE_ERROR_RESOURCE(
				WarningType::MissingAttribute, parent, this, rawDataIndex,
				"'LimbMax' attribute was used but 'LimbNone' attribute is missing", "");
		}
	}
}

void ZLimbTable::ParseRawData()
{
	ZResource::ParseRawData();

	if (decl == nullptr)
	{
		parent->AddDeclaration(GetAddress(), DeclarationAlignment::Align16, GetRawDataSize(),
		                       GetSourceTypeName(), name, headerStr);
	}
	else
	{
		segptr_t limbAddress = limbsAddresses[i];

		if (limbAddress != 0 && GETSEGNUM(limbAddress) == parent->segment)
		{
			uint32_t limbOffset = Seg2Filespace(limbAddress, parent->baseAddress);
			ZLimb* limb;

			if (!parent->HasDeclaration(limbOffset))
			{
				limb = new ZLimb(parent);
				limb->ExtractFromBinary(limbOffset, limbType);
				limb->SetName(limb->GetDefaultName(varPrefix));
				limb->DeclareVar(varPrefix, "");
				limb->DeclareReferences(varPrefix);
				parent->AddResource(limb);
			}
			else
			{
				limb = static_cast<ZLimb*>(parent->FindResource(limbOffset));
				assert(limb != nullptr);
				assert(limb->GetResourceType() == ZResourceType::Limb);
			}

			limb->limbsTable = this;
			limb->SetLimbIndex(i + 1);

			limbsReferences.push_back(limb);
		}
	}

	return "";
}

std::string ZSkeleton::GetSourceTypeName() const
{
	switch (type)
	{
		std::string limbName;
		Globals::Instance->GetSegmentedPtrName(limbsAddresses[i], parent, "", limbName);
		body += StringHelper::Sprintf("\t%s,", limbName.c_str());

		auto& limb = limbsReferences.at(i);
		std::string limbEnumName = limb->enumName;
		if (limbEnumName != "")
		{
			body += StringHelper::Sprintf(" /* %s */", limbEnumName.c_str());
		}

		if (i + 1 < count)
			body += "\n";
	}

	return "SkeletonHeader";
}

std::string ZLimbTable::GetSourceOutputHeader([[maybe_unused]] const std::string& prefix)
{
	if (limbNoneName == "" || limbMaxName == "" || enumName == "")
	{
		// Don't produce a enum of any of those attributes is missing
		return "";
	}

	std::string limbEnum = StringHelper::Sprintf("typedef enum %s {\n", enumName.c_str());

	// This assumes there isn't any skeleton with more than 0x100 limbs

	limbEnum += StringHelper::Sprintf("    /* 0x00 */ %s,\n", limbNoneName.c_str());

	size_t i = 0;
	for (; i < count; i++)
	{
		auto& limb = limbsReferences.at(i);
		std::string limbEnumName = limb->enumName;

		if (limbEnumName == "")
		{
			HANDLE_ERROR_RESOURCE(
				WarningType::MissingAttribute, parent, this, rawDataIndex,
				"Skeleton's enum attributes were used but at least one limb is missing its "
				"'LimbName' attribute",
				StringHelper::Sprintf("When processing limb %02i, named '%s' at offset '0x%X'",
			                          i + 1, limb->GetName().c_str(), limb->GetRawDataIndex()));
		}

		limbEnum += StringHelper::Sprintf("    /* 0x%02X */ %s,\n", i + 1, limbEnumName.c_str());
	}

	limbEnum += StringHelper::Sprintf("    /* 0x%02X */ %s\n", i + 1, limbMaxName.c_str());

	limbEnum += StringHelper::Sprintf("} %s;\n", enumName.c_str());

	return limbEnum;
}

std::string ZLimbTable::GetSourceTypeName() const
{
	return ZResourceType::Skeleton;
}

segptr_t ZSkeleton::GetAddress()
{
	return rawDataIndex;
}

uint8_t ZSkeleton::GetLimbCount()
{
	return limbCount;
}

std::string ZLimbTable::GetLimbEnumName(uint8_t limbIndex) const
{
	if (limbIndex == 0xFF)
	{
		return "LIMB_DONE";
	}

	if (limbIndex < count)
	{
		std::string limbEnumName = limbsReferences.at(limbIndex)->enumName;
		if (limbEnumName != "")
		{
			return StringHelper::Sprintf("%s - 1", limbEnumName.c_str());
		}
	}
	else
	{
		HANDLE_WARNING_RESOURCE(WarningType::InvalidExtractedData, parent, this, rawDataIndex,
		                        StringHelper::Sprintf("Limb index '%02i' out of range", limbIndex),
		                        "");
	}

	return StringHelper::Sprintf("0x%02X", limbIndex);
}
