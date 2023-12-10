/** 
 * This is the implementation of symbol table class.
 * @author hujinbo0931
 * 
 * The symbol table class provides functions as follows:
 * 1. Create a new frame;
 * 2. Delete current frame;
 * 3. Insert a variable;
 * 4. Modify a variable by passing in its name and value (if legal);
 * 5. Modify a variable by passing in its address and value;
 * 6. Return the value of given variable (if legal);
 * 7. Return the address of given variable (if legal);
 * 8. Return the type of given variable (if legal).
 */

#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include "basictype.hpp"
#include "value.hpp"
#include <unordered_map>
#include <vector>
#include <stack>
#include <string>
#include <exception>

typedef unsigned int Time;
typedef unsigned long long Address;
typedef std::string Name;
typedef bool FrameType;

class SymbolTable {
private:
    struct Item {
        Name name;
        ValuePtr value_ptr;
        Address address;
        bool initialized;
        Time time;
    };

    Time time = 0;
    Time global_time_limit = 0;
    Time current_frame_time = 0;
    Time current_funcframe_time = 0;
    bool global_definition_completed = false;
    std::stack<Time> frame_time_list;
    std::stack<Time> funcframe_time_list;
    std::unordered_map<Address, int> address_table;
    std::unordered_map<Name, std::vector<Time>> name_table;
    std::unordered_map<Time, int> time_index_table;
    std::vector<Item> info_table;

    void TimeElapse() {++time; return;} // increment time.
    int IsLegalVariable(Name name);

public:
    SymbolTable();
    void NewFrame(FrameType frametype);
    void DeleteFrame();
    void InsertItem(Name name, ValuePtr type, bool initialized);
    void ModifyByName(Name name, ValuePtr value);
    void ModifyByAddress(Address address, ValuePtr value);
    ValuePtr GetValue(Name name);
    Address GetAddress(Name name);
    BasicTypePtr GetType(Name name);
};

#endif