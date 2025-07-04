#ifndef FILECONTROL_H
#define FILECONTROL_H

#include <X11/Xlib.h>
#include <string>

class FileControl {
private:
    //const std::string& currentFolder = "";

    int min_char_buffer;
    int max_stored_buffer;

public:
    FileControl(int min_buffer, int max_buffer);
    ~FileControl();

    void SetCurrentFolder(const std::string& folderpath);
    int CheckProgress();
};

#endif