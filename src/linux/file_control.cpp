#include "linux/file_control.h"

FileControl::FileControl(int min_buffer, int max_buffer) {
    min_char_buffer = min_buffer;
    max_stored_buffer = max_buffer;
};

FileControl::~FileControl() {

}