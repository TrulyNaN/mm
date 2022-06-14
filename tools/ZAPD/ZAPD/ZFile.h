#pragma once

#include <string>
#include <vector>
#include "Directory.h"
#include "ZResource.h"
#include "ZTexture.h"
#include "tinyxml2.h"

enum class ZFileMode
{
	BuildTexture,
	BuildOverlay,
	BuildModelIntermediette,
	BuildAnimationIntermediette,
	BuildBlob,
	BuildSourceFile,
	BuildBackground,
	Extract,
	Invalid
};

enum class ZGame
{
	OOT_RETAIL,
	OOT_SW97,
	MM_RETAIL
};

class ZFile
{
public:
	std::map<uint32_t, Declaration*> declarations;
	std::string defines;
	std::vector<ZResource*> resources;
	int32_t segment;
	uint32_t baseAddress, rangeStart, rangeEnd;

	ZFile(const fs::path& nOutPath, std::string nName);
	ZFile(ZFileMode mode, tinyxml2::XMLElement* reader, const fs::path& nBasePath,
	      const fs::path& nOutPath, std::string filename, const fs::path& nXmlFilePath,
	      bool placeholderMode);
	~ZFile();

	std::string GetVarName(uint32_t address);
	std::string GetName() const;
	std::string GetOutName() const;
	ZFileMode GetMode() const;
	const fs::path& GetXmlFilePath() const;
	const std::vector<uint8_t>& GetRawData() const;
	void ExtractResources(fs::path outputDir);
	void BuildSourceFile(fs::path outputDir);
	void AddResource(ZResource* res);
	ZResource* FindResource(offset_t rawDataIndex);
	std::vector<ZResource*> GetResourcesOfType(ZResourceType resType);

	Declaration* AddDeclaration(uint32_t address, DeclarationAlignment alignment, size_t size,
	                            std::string varType, std::string varName, std::string body);
	Declaration* AddDeclaration(uint32_t address, DeclarationAlignment alignment,
	                            DeclarationPadding padding, size_t size, std::string varType,
	                            std::string varName, std::string body);
	Declaration* AddDeclarationArray(uint32_t address, DeclarationAlignment alignment, size_t size,
	                                 std::string varType, std::string varName, size_t arrayItemCnt,
	                                 std::string body);
	Declaration* AddDeclarationArray(uint32_t address, DeclarationAlignment alignment, size_t size,
	                                 std::string varType, std::string varName, size_t arrayItemCnt,
	                                 std::string body, bool isExternal);
	Declaration* AddDeclarationArray(uint32_t address, DeclarationAlignment alignment, size_t size,
	                                 std::string varType, std::string varName,
	                                 std::string arrayItemCntStr, std::string body);
	Declaration* AddDeclarationArray(uint32_t address, DeclarationAlignment alignment,
	                                 DeclarationPadding padding, size_t size, std::string varType,
	                                 std::string varName, size_t arrayItemCnt, std::string body);
	Declaration* AddDeclarationPlaceholder(uint32_t address);
	Declaration* AddDeclarationPlaceholder(uint32_t address, std::string varName);
	Declaration* AddDeclarationInclude(uint32_t address, std::string includePath, size_t size,
	                                   std::string varType, std::string varName);
	Declaration* AddDeclarationIncludeArray(uint32_t address, std::string includePath, size_t size,
	                                        std::string varType, std::string varName,
	                                        size_t arrayItemCnt);

	bool GetDeclarationPtrName(segptr_t segAddress, const std::string& expectedType,
	                           std::string& declName) const;
	bool GetDeclarationArrayIndexedName(segptr_t segAddress, size_t elementSize,
	                                    const std::string& expectedType,
	                                    std::string& declName) const;

	Declaration* GetDeclaration(offset_t address) const;
	Declaration* GetDeclarationRanged(offset_t address) const;
	bool HasDeclaration(offset_t address);

	std::string GetHeaderInclude() const;
	std::string GetZRoomHeaderInclude() const;
	std::string GetExternalFileHeaderInclude() const;

	void GeneratePlaceholderDeclarations();

	void AddTextureResource(uint32_t offset, ZTexture* tex);
	ZTexture* GetTextureResource(uint32_t offset) const;

	static std::map<std::string, ZResourceFactoryFunc*>* GetNodeMap();
	static void RegisterNode(std::string nodeName, ZResourceFactoryFunc* nodeFunc);

protected:
	std::vector<uint8_t> rawData;
	std::string name;
	fs::path basePath;
	fs::path outputPath;
	fs::path xmlFilePath;
	// Keep track of every texture of this ZFile.
	// The pointers declared here are "borrowed" (somebody else is the owner),
	// so ZFile shouldn't delete/free those textures.
	std::map<uint32_t, ZTexture*> texturesResources;

	ZFile();
	void ParseXML(ZFileMode mode, tinyxml2::XMLElement* reader, std::string filename,
	              bool placeholderMode);
	void DeclareResourceSubReferences();
	void GenerateSourceFiles(fs::path outputDir);
	void GenerateSourceHeaderFiles();
	void GenerateHLIntermediette();
	void AddDeclarationDebugChecks(uint32_t address);
	std::string ProcessDeclarations();
	void ProcessDeclarationText(Declaration* decl);
	std::string ProcessExterns();

	std::string ProcessTextureIntersections(const std::string& prefix);
	void HandleUnaccountedData();
	bool HandleUnaccountedAddress(offset_t currentAddress, offset_t lastAddr, uint32_t& lastSize);
};
