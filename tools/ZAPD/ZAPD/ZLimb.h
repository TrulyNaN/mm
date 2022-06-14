#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "ZDisplayList.h"
#include "ZFile.h"

enum class ZLimbType
{
	Standard,
	LOD,
	Skin,
	Curve,
};

class ZLimbTable;

class ZLimb : public ZResource
{
protected:
	uint8_t unk_0;
	int16_t x;
	int16_t y;
	int16_t z;
	uint8_t unk_8;

public:
	std::string enumName;
	ZLimbTable* limbsTable = nullptr;  // borrowed pointer, do not delete!

	ZLimbType type = ZLimbType::Standard;

	ZLimbSkinType skinSegmentType = ZLimbSkinType::SkinType_0;  // Skin only
	segptr_t skinSegment = 0;                                   // Skin only
	Struct_800A5E28 segmentStruct;                              // Skin only
	segptr_t dList2Ptr;                                         // LOD and Curve Only

	std::string GetLimbDListSourceOutputCode(const std::string& prefix,
	                                         const std::string& limbPrefix, segptr_t dListPtr);

	std::string GetSourceOutputCodeSkin(const std::string& prefix);
	std::string GetSourceOutputCodeSkin_Type_4(const std::string& prefix);

public:
	segptr_t dListPtr = 0;
	segptr_t farDListPtr = 0;  // LOD only
	int16_t transX, transY, transZ;
	uint8_t childIndex, siblingIndex;

	uint8_t limbIndex = 0;

	ZLimb(ZFile* nParent);
	ZLimb(ZLimbType limbType, const std::string& prefix, const std::vector<uint8_t>& nRawData,
	      uint32_t nRawDataIndex, ZFile* nParent);

	void ExtractFromXML(tinyxml2::XMLElement* reader, const std::vector<uint8_t>& nRawData,
	                    const uint32_t nRawDataIndex) override;

	void ParseXML(tinyxml2::XMLElement* reader) override;
	void ParseRawData() override;

	size_t GetRawDataSize() const override;
	std::string GetSourceOutputCode(const std::string& prefix) override;
	std::string GetSourceTypeName() const override;
	ZResourceType GetResourceType() const override;

	ZLimbType GetLimbType();
	void SetLimbType(ZLimbType value);
	static const char* GetSourceTypeName(ZLimbType limbType);

	void SetLimbIndex(uint8_t nLimbIndex);

protected:
	void DeclareDList(segptr_t dListSegmentedPtr, const std::string& prefix,
	                  const std::string& limbSuffix);
};
