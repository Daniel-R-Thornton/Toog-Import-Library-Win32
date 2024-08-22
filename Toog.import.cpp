#include "pch.h"
#include "Toog.Import.h"
#include <cstring>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <functional>

namespace {
    std::unordered_map<AsyncHandle, std::function<void()>> asyncOperations;
    AsyncHandle nextHandle = 1;

    void* AllocateString(const std::string& str) {
        size_t len = str.size() + 1;
        char* buffer = new char[len];
        std::memcpy(buffer, str.c_str(), len);
        return buffer;
    }

    void FreeString(void* str) {
        delete[] static_cast<char*>(str);
    }

    void* AllocateStringFromCStr(const char* str) {
        if (!str) return nullptr;
        return AllocateString(std::string(str));
    }

    std::string GetStringFromCStr(const char* str) {
        return std::string(str ? str : "");
    }
}
// connect to the named pipe, if alls good it should return the current tokens status , we will then disconnect to leave the pipe free
TOOGIMPORT_API int Initialise(const char* apiKey, TokenStatus* status) {
    // Initialize the module with the provided API key
    // Simulate initialization
    //setup the named pipe
    if (apiKey) {
        *status = TOKEN_STATUS_OK;  // Simulate successful initialization
        return 0;  // Success
    }
    return -1;  // Error
}

TOOGIMPORT_API int Connect(ConnectionResult* result) {
    // Connect to the service
    // Simulate connection
    *result = CONNECTION_OK;  // Simulate successful connection
    return 0;  // Success
}

TOOGIMPORT_API AsyncHandle StartAsyncGetJobs(JobStatusCallback jobStatusCallback, CollectionCallback collectionCallback, void* context) {
    AsyncHandle handle = nextHandle++;
    asyncOperations[handle] = [handle, jobStatusCallback, collectionCallback, context]() {
        // Simulate async operation and invoke callbacks
        if (jobStatusCallback) {
            jobStatusCallback(context, "jobId123", "Job Name Example");
        }
        if (collectionCallback) {
            collectionCallback(context, "collectionId123", "Collection Name Example");
        }
        };
    return handle;
}

TOOGIMPORT_API AsyncHandle StartAsyncGetSites(JobStatusCallback jobStatusCallback, CollectionCallback collectionCallback, void* context) {
    AsyncHandle handle = nextHandle++;
    asyncOperations[handle] = [handle, jobStatusCallback, collectionCallback, context]() {
        // Simulate async operation and invoke callbacks
        if (jobStatusCallback) {
            jobStatusCallback(context, "jobId123", "Job Name Example");
        }
        if (collectionCallback) {
            collectionCallback(context, "collectionId123", "Collection Name Example");
        }
        };
    return handle;
}

TOOGIMPORT_API void CancelAsyncOperation(AsyncHandle handle) {
    asyncOperations.erase(handle);
}

TOOGIMPORT_API int GetAsyncResult(AsyncHandle handle, Jobs** jobsResult, Sites** sitesResult) {
    if (asyncOperations.find(handle) != asyncOperations.end()) {
        // Call the async operation
        asyncOperations[handle]();
        asyncOperations.erase(handle);
        return 0;  // Success
    }
    return -1;  // Error
}

TOOGIMPORT_API void ReleaseJobs(Jobs* jobs) {
    if (jobs) {
        for (size_t i = 0; i < jobs->jobsSize; ++i) {
            FreeString(jobs->jobs[i].id);
            FreeString(jobs->jobs[i].name);
            FreeString(jobs->jobs[i].jobStatusId);
            FreeString(jobs->jobs[i].jobNumber);
            FreeString(jobs->jobs[i].siteId);
            for (size_t j = 0; j < jobs->jobs[i].collectionsSize; ++j) {
                FreeString(jobs->jobs[i].collections[j].id);
                FreeString(jobs->jobs[i].collections[j].name);
            }
            delete[] jobs->jobs[i].collections;
        }
        delete[] jobs->jobs;
        delete[] jobs->jobStatuses;
        delete jobs;
    }
}

TOOGIMPORT_API void ReleaseSites(Sites* sites) {
    if (sites) {
        for (size_t i = 0; i < sites->sitesSize; ++i) {
            FreeString(sites->sites[i].id);
            FreeString(sites->sites[i].name);
            FreeString(sites->sites[i].accountId);
            FreeString(sites->sites[i].details.address1);
            FreeString(sites->sites[i].details.address2);
            FreeString(sites->sites[i].details.address3);
            FreeString(sites->sites[i].details.city);
            FreeString(sites->sites[i].details.state);
            FreeString(sites->sites[i].details.zip);
            FreeString(sites->sites[i].details.country);
        }
        delete[] sites->sites;
        delete[] sites->jobStatuses;
        delete sites;
    }
}

TOOGIMPORT_API void GetLastError(char* buffer, size_t bufferSize) {
    // Simulate getting an error message
    std::string errorMessage = "Sample error message";
    std::strncpy(buffer, errorMessage.c_str(), bufferSize);
}

TOOGIMPORT_API int GetStatusUpdate(char* buffer, size_t bufferSize) {
    // Simulate getting a status update
    std::string statusUpdate = "Sample status update";
    std::strncpy(buffer, statusUpdate.c_str(), bufferSize);
    return 0;  // Success
}
