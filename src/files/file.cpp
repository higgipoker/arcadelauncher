#include "file.h"
#include <iostream>
#include <sstream>

// ------------------------------------------------------------
// File
// ------------------------------------------------------------
File::File() {
    opened_for_read = opened_for_write = false;
}

// ------------------------------------------------------------
// File
// ------------------------------------------------------------
File::File(const std::string &f) : file_name(f) {
    opened_for_read = opened_for_write = false;
}

// ------------------------------------------------------------
// read_data
// ------------------------------------------------------------
File::~File() {
    Close();
}

// ------------------------------------------------------------
// open
// ------------------------------------------------------------
void File::Open(const std::string &f) {
    file_name = f;
}

// ------------------------------------------------------------
// close
// ------------------------------------------------------------
void File::Close() {
    if (opened_for_read) {
        in_file.close();
    }
    if (opened_for_write) {
        out_file.close();
    }
}

// ------------------------------------------------------------
// writeLine
// ------------------------------------------------------------
void File::WriteLine(const std::string &line) {
    open_for_write();

    if (out_file.is_open()) {
        out_file << line.c_str() << "\n";
    } else {
        std::cout << "Unable to opne file: " << file_name.c_str();
    }
}

// ------------------------------------------------------------
// writeLine
// ------------------------------------------------------------
void File::WriteLine(int line) {
    std::ostringstream str;
    str << line;

    WriteLine(str.str());
}

// ------------------------------------------------------------
// openForWrite
// ------------------------------------------------------------
void File::open_for_write() {
    if (opened_for_write) {
        return;
    }

    if (opened_for_read) {
        in_file.close();
    }

    out_file.open(file_name.c_str(), std::ios::out);
    opened_for_write = true;
}

// ------------------------------------------------------------
// openForRead
// ------------------------------------------------------------
void File::open_for_read() {
    if (opened_for_read) {
        return;
    }

    if (opened_for_write) {
        out_file.close();
    }

    in_file.open(file_name.c_str(), std::ios::in);
    opened_for_read = true;
}

// ------------------------------------------------------------
// getLines
// ------------------------------------------------------------
std::vector<std::string> File::GetLines() {
    open_for_read();

    // temp
    std::string line;
    while (getline(in_file, line)) {
        lines.push_back(line);
    }

    return lines;
}
