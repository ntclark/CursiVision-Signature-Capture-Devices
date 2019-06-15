/// @file      versionString.h
/// @copyright Copyright(c) 2018 Wacom Co., Ltd.
/// @author    KParkes
/// @date      2018-07-24
/// @brief     

#ifndef versionString_h
#define versionString_h


// File Version = Major.Minor.Release.Build
// Major, Minor, Release defined individually for each component
#define FILE_VER_BUILD JENKINS_BUILD_NUMBER

#define WacomGSS_STRINGIZER2(S) #S
#define WacomGSS_STRINGIZER(S) WacomGSS_STRINGIZER2(S)

#define WacomGSS_FILEVER_S(MAJ, MIN, REL, BLD) WacomGSS_STRINGIZER(MAJ) "." WacomGSS_STRINGIZER(MIN) "." WacomGSS_STRINGIZER(REL) "." WacomGSS_STRINGIZER(BLD)



#endif    // versionString_h
