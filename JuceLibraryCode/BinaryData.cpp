/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== TextEx.txt ==================
static const unsigned char temp_binary_data_0[] =
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris eget gravida ligula. Vestibulum eu commodo eros, eget vestibulum mauris. Mauris convallis eget sapien quis elementum. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam feli"
"s purus, vehicula non vestibulum fringilla, ultricies faucibus purus. Donec pretium suscipit tellus, eget tempor risus ornare a. Cras ut lacus ut ligula egestas facilisis.";

const char* TextEx_txt = (const char*) temp_binary_data_0;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0xcd7f2491:  numBytes = 421; return TextEx_txt;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "TextEx_txt"
};

const char* originalFilenames[] =
{
    "TextEx.txt"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
    {
        if (namedResourceList[i] == resourceNameUTF8)
            return originalFilenames[i];
    }

    return nullptr;
}

}
