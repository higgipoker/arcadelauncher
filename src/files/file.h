#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class File {

  public:
    /**
      @brief constructor
      @param f file name
    */
    File(const std::string &f);
    File();

    virtual void Open(const std::string &f);

    /**
     * @brief destructor
     */
    virtual ~File();

    /**
     * @brief write a line to the file
     * @param line string line to write
     */
    void WriteLine(const std::string &line);

    /**
     * @brief write line to the file
     * @param line a number to write
     */
    void WriteLine(int line);

    /**
     * @brief close the file safely
     */
    void Close();

    /**
     * @brief get the data from the file in the form of a vector of lines
     */
    std::vector<std::string> GetLines();

  protected:
    std::string file_name;
    std::ofstream out_file;
    std::ifstream in_file;

    bool opened_for_read;
    bool opened_for_write;

    void open_for_write();
    void open_for_read();

    std::vector<std::string> lines;
};
