#include "../include/Resources.h"

#if defined(__ANDROID__)
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "../include/OS_ANDROID.h"
#endif

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

namespace GGE {

    Resources* Resources::instance = 0;
    Resources::Resources()
    {
        zipFile = NULL;
    }

    char* Resources::sgets( char * str, int num, char **input )
    {
        char *next = *input;
        int  numread = 0;

        while ( numread + 1 < num && *next ) {
            int isnewline = ( *next == '\n' );
            *str++ = *next++;
            numread++;

            if ( isnewline )
                break;
        }

        if ( numread == 0 )
            return NULL;

        *str = '\0';

        *input = next;
        return str;
    }

    void Resources::destroy()
    {
        if (zipFile)
            zip_close(zipFile);
	    delete instance;
        instance = NULL;
    }

    void Resources::loadZip(const char* zipFileName, const char* key)
    {
#if defined(__ANDROID__)
        AAssetManager* mgr = OS::getInstance()->getAndroidApp()->activity->assetManager;
        AAsset *asset = AAssetManager_open(mgr, zipFileName, AASSET_MODE_BUFFER);
        size_t fileLength = AAsset_getLength(asset);

        fileContent = new char[fileLength+1];

        AAsset_read(asset, fileContent, fileLength);

        fileContent[fileLength] = '\0';
        zip_error zerr;
        zip_source_t *src = zip_source_buffer_create(fileContent, fileLength, 0, &zerr);
        if (!src)
        {
            OS::getInstance()->alert("ZipFile", "Error reading resources file!");
        }
        else
        {
            zipFile = zip_open_from_source(src, 0, &zerr);
        }

        AAsset_close(asset);
#else
        int err = 0;
        zipFile = ::zip_open(zipFileName, 0, &err);
#endif
        zip_set_default_password(zipFile, key);
    }

    resourceFile* Resources::loadFile(const char* cfileName)
    {
#if defined(__ANDROID__)
        AAssetManager* mgr = OS::getInstance()->getAndroidApp()->activity->assetManager;
        AAsset *asset = AAssetManager_open(mgr,("conf/" + to_string(cfileName)).c_str(), AASSET_MODE_BUFFER);
        if (asset) {
            uint64_t fileLength = AAsset_getLength(asset);


            resourceFile *rf = new resourceFile();
            rf->fileName = cfileName;
            rf->size = fileLength;

            rf->file = new char[fileLength + 1];

            AAsset_read(asset, rf->file, fileLength);

            rf->file[fileLength] = '\0';

            AAsset_close(asset);

            return rf;
        } else
        {
            return NULL;
        }
#else

        char path[1024] = ".";
 #if defined (__APPLE__)
        CFBundleRef mainBundle = CFBundleGetMainBundle();
        CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
        if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
        {
            exit(1);
        }
        CFRelease(resourcesURL);

        chdir(path);
 #endif

        long size;
//        std::cout << "Current Path: " << (to_string(path) + "/conf/" + to_string(cfileName)).c_str() << std::endl;
        std::ifstream File((to_string(path) + "/conf/" + to_string(cfileName)).c_str(), std::ifstream::binary);
        if(File.is_open()){

           File.seekg(0, std::ifstream::end);
           size=File.tellg();
           File.seekg(0);

           resourceFile* rf = new resourceFile();
            rf->fileName = cfileName;
            rf->size = size ;

           rf->file = (char*) calloc(size + 1, sizeof(char));

           File.read (rf->file, size);


           File.close();

            return rf;

        }else{
            return NULL;
        }
#endif
    }

    resourceFile* Resources::loadCompressedFile(const char* cFileName)
    {
#if defined(DEBUG)
        return loadFile(cFileName);
#endif

        const char *name = cFileName;
        struct zip_stat st;
        zip_stat_init(&st);
        zip_stat(zipFile, name, 0, &st);

        resourceFile* rf = new resourceFile();
        rf->fileName = cFileName;
        rf->size = st.size;

        rf->file = (char*) calloc(st.size + 1, sizeof(char));

        zip_file *f = zip_fopen(zipFile, cFileName, 0);
        zip_fread(f, rf->file, st.size);
        zip_fclose(f);


        return rf;
    }
}
