#pragma once
#include <cstddef>

#ifdef TOOGIMPORT_EXPORTS
#define TOOGIMPORT_API __declspec(dllexport)
#else
#define TOOGIMPORT_API __declspec(dllimport)
#endif

#ifdef __cplusplus

extern "C" {
#endif

    // Define buffer size for error messages
#define ERROR_BUFFER_SIZE 256

// Handle type for async operations
    typedef unsigned long AsyncHandle;

    // Function pointer type for callbacks
    typedef void (*JobStatusCallback)(void* context, const char* jobId, const char* jobName);
    typedef void (*CollectionCallback)(void* context, const char* collectionId, const char* collectionName);
    typedef void (*CompletionCallback)(void* context, AsyncHandle handle, int result);

    // Represents the status of a job
    typedef struct {
        char* id;   // string for the job status ID
        char* name; // string for the job status name
    } JobStatus;

    // Represents a collection of items associated with a job
    typedef struct {
        char* id;   // string for the collection ID
        char* name; // string for the collection name
    } Collection;

    // Represents data related to a job
    typedef struct {
        char* id;                // string for the job ID
        char* name;              // string for the job name
        char* jobStatusId;       // string for the job status ID
        char* jobNumber;         // string for the job number
        char* siteId;            // string for the site ID
        Collection* collections; // Pointer to an array of collections
        size_t collectionsSize;  // Number of collections
    } JobData;

    // Contains multiple jobs and their statuses
    typedef struct {
        JobData* jobs;           // Pointer to an array of job data
        size_t jobsSize;         // Number of jobs
        JobStatus* jobStatuses;  // Pointer to an array of job statuses
        size_t jobStatusesSize;  // Number of job statuses
    } Jobs;

    // Represents a type of import with associated metadata
    typedef struct {
        char* id;                // string for the import type ID
        char* name;              // string for the import type name
        char* columns;           // string of column names
        size_t columnsSize;      // Number of columns
        char* commonItemPrefix;  // string for the common item prefix
    } ImportType;

    // Represents detailed address information for a site
    typedef struct {
        char* address1;  // string for the first line of the address
        char* address2;  // string for the second line of the address (optional)
        char* address3;  // string for the third line of the address (optional)
        char* city;      // string for the city
        char* state;     // string for the state
        char* zip;       // string for the postal code
        char* country;   // string for the country
    } SiteDetails;

    // Represents a site with its details
    typedef struct {
        char* id;         // string for the site ID
        char* name;       // string for the site name
        char* accountId;  // string for the account ID
        SiteDetails details; // Detailed address information for the site
    } Site;

    // Contains multiple sites and their statuses
    typedef struct {
        Site* sites;        // Pointer to an array of sites
        size_t sitesSize;   // Number of sites
        JobStatus* jobStatuses; // Pointer to an array of job statuses related to the sites
        size_t jobStatusesSize; // Number of job statuses
    } Sites;

    // Enumeration for connection results
    typedef enum {
        CONNECTION_OK,         // Connection succeeded
        CONNECTION_TIMED_OUT,   // Connection timed out
        CONNECTION_ERROR       // Connection encountered an error
    } ConnectionResult;

    typedef enum {
        TOKEN_STATUS_OK,
        TOKEN_STATUS_UNKNOWN,
        TOKEN_STATUS_INVALID,
    } TokenStatus;

    // Functions exposed by the DLL
    TOOGIMPORT_API int Initialise(const char* apiKey, TokenStatus* status);
    TOOGIMPORT_API int Connect(ConnectionResult* result);
    TOOGIMPORT_API AsyncHandle StartAsyncGetJobs(JobStatusCallback jobStatusCallback, CollectionCallback collectionCallback, void* context);
    TOOGIMPORT_API AsyncHandle StartAsyncGetSites(JobStatusCallback jobStatusCallback, CollectionCallback collectionCallback, void* context);
    TOOGIMPORT_API void CancelAsyncOperation(AsyncHandle handle);
    TOOGIMPORT_API int GetAsyncResult(AsyncHandle handle, Jobs** jobsResult, Sites** sitesResult);
    TOOGIMPORT_API void ReleaseJobs(Jobs* jobs);
    TOOGIMPORT_API void ReleaseSites(Sites* sites);
    TOOGIMPORT_API int GetStatusUpdate(char* buffer, size_t bufferSize);
#ifdef __cplusplus
}
#endif
