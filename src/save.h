#ifndef save_h
#define save_h

#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>

void listDir(fs::FS &fs, const char * dirname, uint8_t levels);

String readFile(fs::FS &fs, const char * path);

void writeFile(fs::FS &fs, const char * path, const char * message);

void appendFile(fs::FS &fs, const char * path, const char * message);

void renameFile(fs::FS &fs, const char * path1, const char * path2);

void deleteFile(fs::FS &fs, const char * path);

void testFileIO(fs::FS &fs, const char * path);

#endif