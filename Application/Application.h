#pragma once
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif
EXTERNC void Application_RunApplication();
#undef EXTERNC
