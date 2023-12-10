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

#include <basictype.hpp>
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
        BasicTypePtr type;
        Address address;
        bool initialized;
        long long value;
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
    void NewFrame(FrameType frametype);
    void DeleteFrame();
    void InsertItem(Name name, BasicTypePtr type, bool initialized, long long value = 0);
    void ModifyByName(Name name, long long value);
    void ModifyByAddress(Address address, long long value);
    long long GetValue(Name name);
    Address GetAddress(Name name);
    BasicTypePtr GetType(Name name);
};

/**
 * Initialization
 */
SymbolTable::SymbolTable() {
    current_frame_time = time;
    frame_time_list.push(time);
    current_funcframe_time = time;
    funcframe_time_list.push(time);
}

/**
 * Determine whether the variable is legal.
 */
int SymbolTable::IsLegalVariable(Name name) {
    if (name_table.find(name) != name_table.end()) { // the variable exists
        if (global_definition_completed == false) {// before main function
            return time_index_table[name_table[name].back()];
        } else {
            if (name_table[name].back() > current_funcframe_time) 
                return time_index_table[name_table[name].back()];
            if (name_table[name][0] < global_time_limit)
                return time_index_table[name_table[name][0]];
            throw std::exception();
        }
    } else throw std::exception();
    return -1;
}

/**
 * Creates a new frame by recording the time and frame type.
 */
void SymbolTable::NewFrame(FrameType frametype) {
    TimeElapse();
    // record the time global variables are completely defined
    if (global_definition_completed == false) global_time_limit = time;
    global_definition_completed = true;

    current_frame_time = time;
    frame_time_list.push(time);
    // when the frame belongs to a function, update funcframe.
    if (frametype == 1) {
        current_funcframe_time = time;
        funcframe_time_list.push(time);
    }
    return;
}

/**
 * Delete the current frame. 
 * Recycle the variables defined after frametime and accordingly change in the 
 * name table and address table.
 */
void SymbolTable::DeleteFrame() {
    TimeElapse();
    if (frame_time_list.size() == 1) throw std::exception();
    while (!info_table.empty() && info_table.back().time > current_frame_time) {
        Item last_item = info_table.back();
        // modify name table
        name_table[last_item.name].pop_back();
        if (name_table[last_item.name].empty()) name_table.erase(last_item.name);
        // modify address table
        address_table.erase(last_item.address);
        // modify time_name table;
        time_index_table.erase(last_item.time);
        // modify info table
        info_table.pop_back();
    }

    //update the time lists and current newest frame time
    if (current_frame_time == current_funcframe_time)  funcframe_time_list.pop();
    frame_time_list.pop();
    current_frame_time = frame_time_list.top();
    current_funcframe_time = funcframe_time_list.top();
    return;
}

/**
 * Creates a new variable and update the lookup tables.
 * If a variable with the same name is already created in the frame, throw an exception.
 */
void SymbolTable::InsertItem(Name name, BasicTypePtr type, bool initialized, long long value = 0) {
    TimeElapse();
    if (name_table.find(name) != name_table.end()) { //the variable already exists
        if (name_table[name].back() > current_frame_time) throw std::exception();
        name_table[name].push_back(time);
    } else { //the variable doesn't exist
        // modify name table
        std::vector<Time> temp;
        temp.push_back(time);
        name_table[name] = temp;
    }
    // modify info table
    if (initialized == true) info_table.push_back({name, type, type -> GetAddress(), true, value, time});
    else info_table.push_back({name, type, type -> GetAddress(), false, value, time});
    // modify address table
    address_table[type -> GetAddress()] = info_table.size() - 1;
    // modify time_index table
    time_index_table[time] = info_table.size() - 1;
    return;
}

/**
 * Modify a variable by passing in its name and value.
 * 
 * Alert: only variables in the frame and global varialbles are allowed.
 */
void SymbolTable::ModifyByName(Name name, long long value) {
    TimeElapse();
    int index = IsLegalVariable(name);
    if (index >= 0) {
        info_table[index].initialized = true;
        info_table[index].value = value;
    }
    else throw std::exception();
    return;
}

/**
 * Modify a variable by passing in its address and value.
 * Throws an exception when the address doesn't correspond to a variable.
 * 
 * Allowed whenever there exists a variable at the address, regardless of frame.
 */
void SymbolTable::ModifyByAddress(Address address, long long value) {
    TimeElapse();
    if (address_table.find(address) != address_table.end()) {
        info_table[address_table[address]].value = value;
    } else throw std::exception();
    return;
}

/**
 * Return the value of given variable (if legal).
 * 
 * If the variable is uninitialized, throw an exception.
 */
long long SymbolTable::GetValue(Name name) {
    TimeElapse();
    int index = IsLegalVariable(name);
    if (index >= 0) {
        if (info_table[index].initialized == false) throw std::exception();
        else return info_table[index].value;
    }
    else throw std::exception();
    return -1;
}

/**
 * Return the address of given variable (if legal).
 */
Address SymbolTable::GetAddress(Name name) {
    TimeElapse();
    int index = IsLegalVariable(name);
    if (index >= 0) return info_table[index].address;
    else throw std::exception();
    return 0;
}

/**
 * Return the type of given variable (if legal).
 */
BasicTypePtr SymbolTable::GetType(Name name) {
    TimeElapse();
    int index = IsLegalVariable(name);
    if (index >= 0) return info_table[index].type;
    else throw std::exception();
    return nullptr;
}

#endif