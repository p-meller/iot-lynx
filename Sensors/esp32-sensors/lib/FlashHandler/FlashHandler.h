#ifndef FLASHSTORAGEMETA_H
#define FLASHSTORAGEMETA_H

#include <EEPROM.h>
#include "FlashIndex.h"

struct FlashStorageMeta
{
    uint32_t idx;
    uint32_t size;
    uint32_t offset;
};

struct FlashStorageData
{
    uint32_t idx;
    uint32_t size;
    char *data;

    FlashStorageData() = default;
    ~FlashStorageData()
    {
        if (data)
        {
            delete[] data;
        }
    }
};

class FlashHandler
{
    FlashStorageMeta *storageMeta;
    uint32_t storageSize = 0;
    uint32_t currentSize = 0;

public:
    FlashHandler(uint32_t size)
    {
        storageSize = size;
        storageMeta = new FlashStorageMeta[storageSize];
        for (uint32_t i = 0; i < storageSize; i++)
        {
            storageMeta[i].idx = i;
            storageMeta[i].size = 0;
            storageMeta[i].offset = 0;
        }
    }

    uint32_t registerStorage(uint32_t storageMaxSize)
    {
        if (currentSize < storageSize)
        {
            storageMeta[currentSize].idx = currentSize;
            storageMeta[currentSize].size = storageMaxSize;
            storageMeta[currentSize].offset = currentSize;
            return currentSize++;
        }
        else
        {
            return UINT32_MAX;
        }
    }

    FlashStorageData readFromStorage(uint32_t storageIdx)
    {
        FlashStorageData data;
        if (storageIdx < currentSize)
        {
            data.idx = storageIdx;
            data.size = storageMeta[storageIdx].size;
            data.data = new char[data.size];
            EEPROM.readBytes(storageMeta[storageIdx].offset, data.data, data.size);
        }
        return data;
    }

    bool writeToStorage(uint32_t storageIdx, void *data, uint32_t size)
    {
        if (storageIdx < currentSize)
        {
            EEPROM.writeBytes(storageMeta[storageIdx].offset, data, size);
            EEPROM.commit();
            return true;
        }
        else
        {
            return false;
        }
    }
};

#endif // FLASHSTORAGEMETA_H