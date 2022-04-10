
#include "include/NBioAPI.h"
#include "include/NBioAPI_Export.h"
#include "include/NBioAPI_IndexSearch.h"

// Defines the global variables being used
NBioAPI_HANDLE bspHandle;
NBioAPI_DEVICE_ID idDevice;
NBioAPI_RETURN ret;

// Defines some shorten constants to make the code more readable
#define AUDIT NBioAPI_FIR_PURPOSE_AUDIT
#define ENROLL NBioAPI_FIR_PURPOSE_ENROLL
#define VERIFY NBioAPI_FIR_PURPOSE_VERIFY
#define IDENTIFY NBioAPI_FIR_PURPOSE_IDENTIFY

// Define the skins we currently have
#define SKIN_ENG (0x01)
#define SKIN_JPN (0x02)
#define SKIN_KOR (0x03)
#define SKIN_POR (0x04)
