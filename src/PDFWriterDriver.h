/*
 Source File : PDFPageDriver.h
 
 
 Copyright 2013 Gal Kahana HummusJS
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 
 */
#pragma once

#include "nodes.h"
#include <utility>
#include <string>
#include <map>

#include "PDFWriter.h"
#include "PDFEmbedParameterTypes.h"
#include "ObjectByteWriterWithPosition.h"
#include "ObjectByteReaderWithPosition.h"


typedef std::pair<unsigned long,unsigned long> ULongAndULongPair;
typedef std::pair<std::string,unsigned long> StringAndULongPair;
typedef std::pair<ObjectIDType,bool> ObjectIDTypeAndBool;


struct CachedHummusImageInformation
{
    enum EHummusImageType
    {
        eUndefined,
        ePDF,
        eJPG,
        eTIFF
    };
    
    CachedHummusImageInformation(){writtenObjectID = 0;imageType=eUndefined;imageWidth=-1;imageHeight=-1;}
    
    ObjectIDType writtenObjectID;
    EHummusImageType imageType;
    double imageWidth;
    double imageHeight;
};

typedef std::map<StringAndULongPair,CachedHummusImageInformation> StringAndULongPairToCachedHummusImageInformationMap;

class IByteWriterWithPosition;

class PDFWriterDriver : public node::ObjectWrap
{
public:
    virtual ~PDFWriterDriver();
    
    static void Init();
	static METHOD_RETURN_TYPE NewInstance(const ARGS_TYPE& args);
	static v8::Handle<v8::Value> GetNewInstance(const ARGS_TYPE& args);

    PDFHummus::EStatusCode StartPDF(const std::string& inOutputFilePath,
                                    EPDFVersion inPDFVersion,
                                    const LogConfiguration& inLogConfiguration,
                                    const PDFCreationSettings& inCreationSettings);
    PDFHummus::EStatusCode StartPDF(v8::Handle<v8::Object> inStreamObject,
                                    EPDFVersion inPDFVersion,
                                    const LogConfiguration& inLogConfiguration,
                                    const PDFCreationSettings& inCreationSettings);
    
    PDFHummus::EStatusCode ContinuePDF(const std::string& inOutputFilePath,
                                       const std::string& inStateFilePath,
                                       const std::string& inOptionalOtherOutputFile,
                                       const LogConfiguration& inLogConfiguration);

    PDFHummus::EStatusCode ContinuePDF(v8::Handle<v8::Object>  inOutputStream,
                                       const std::string& inStateFilePath,
                                       v8::Handle<v8::Object>  inModifiedSourceStream,
                                       const LogConfiguration& inLogConfiguration);
    
    
    PDFHummus::EStatusCode ModifyPDF(const std::string& inSourceFile,
                                     EPDFVersion inPDFVersion,
                                     const std::string& inOptionalOtherOutputFile,
                                     const LogConfiguration& inLogConfiguration,
                                     const PDFCreationSettings& inCreationSettings);

    PDFHummus::EStatusCode ModifyPDF(v8::Handle<v8::Object>  inSourceStream,
                                     v8::Handle<v8::Object>  inDestinationStream,
                                     EPDFVersion inPDFVersion,
                                     const LogConfiguration& inLogConfiguration,
                                     const PDFCreationSettings& inCreationSettings);
    
    
    
    // image registry services, optimization for r/w
    DoubleAndDoublePair GetImageDimensions(const std::string& inImageFile,unsigned long inImageIndex);
    ObjectIDTypeAndBool RegisterImageForDrawing(const std::string& inImageFile,unsigned long inImageIndex);
    PDFHummus::EStatusCode WriteFormForImage(const std::string& inImagePath,unsigned long inImageIndex,ObjectIDType inObjectID);

    PDFWriter* GetWriter();
    
private:
    PDFWriterDriver();
    
    
    static v8::Persistent<v8::Function> constructor;
    static METHOD_RETURN_TYPE New(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE End(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE CreatePage(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE WritePage(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE WritePageAndReturnID(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE StartPageContentContext(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE PausePageContentContext(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE CreateFormXObject(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE EndFormXObject(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE CreateformXObjectFromJPG(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE CreateImageXObjectFromJPG(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE RetrieveJPGImageInformation(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE GetFontForFile(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE AttachURLLinktoCurrentPage(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE Shutdown(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE CreateFormXObjectFromTIFF(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE GetObjectsContext(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE AppendPDFPagesFromPDF(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE MergePDFPagesToPage(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE CreatePDFCopyingContext(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE CreateFormXObjectsFromPDF(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE CreatePDFCopyingContextForModifiedFile(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE CreatePDFTextString(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE CreatePDFDate(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE SGetImageDimensions(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE GetModifiedFileParser(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE GetModifiedInputFile(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE GetOutputFile(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE GetDocumentContext(const ARGS_TYPE& args);
    static METHOD_RETURN_TYPE RegisterAnnotationReferenceForNextPageWrite(const ARGS_TYPE& args);
    
    static CMYKRGBColor colorFromArray(v8::Handle<v8::Value> inArray);
    static PDFPageRange ObjectToPageRange(v8::Handle<v8::Object> inObject);

    CachedHummusImageInformation& GetImageInformationStructFor(const std::string& inImageFile,unsigned long inImageIndex);
    CachedHummusImageInformation::EHummusImageType GetImageType(const std::string& inImageFile,unsigned long inImageIndex);
    
    bool mStartedWithStream;
    PDFWriter mPDFWriter;
    StringAndULongPairToCachedHummusImageInformationMap mImagesInformation;
    ObjectByteWriterWithPosition* mWriteStreamProxy;
    ObjectByteReaderWithPosition* mReadStreamProxy;
};