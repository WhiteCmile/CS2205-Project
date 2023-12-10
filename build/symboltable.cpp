/*
    @author hujinbo0931
*/

/**
 * Initialization
 */

#include "basictype.hpp"
#include "value.hpp"
#include "symboltable.hpp"
#include <unordered_map>
#include <vector>
#include <stack>
#include <string>
#include <exception>

SymbolTable :: SymbolTable() {
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
void SymbolTable::InsertItem(Name name, ValuePtr value_ptr, bool initialized) {
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
    if (initialized == true) info_table.push_back((Item) {name, value_ptr, value_ptr.GetAddress(), true, time});
    else info_table.push_back((Item) {name, value_ptr, value_ptr.GetAddress(), false, time});
    // modify address table
    address_table[value_ptr.GetAddress()] = info_table.size() - 1;
    // modify time_index table
    time_index_table[time] = info_table.size() - 1;
    return;
}

/**
 * Modify a variable by passing in its name and value.
 * 
 * Alert: only variables in the frame and global varialbles are allowed.
 */
void SymbolTable::ModifyByName(Name name, ValuePtr value_ptr) {
    TimeElapse();
    int index = IsLegalVariable(name);
    if (index >= 0) {
        info_table[index].initialized = true;
        info_table[index].value_ptr = value_ptr;
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
void SymbolTable::ModifyByAddress(Address address, ValuePtr value_ptr) {
    TimeElapse();
    if (address_table.find(address) != address_table.end()) {
        info_table[address_table[address]].value_ptr = value_ptr;
    } else throw std::exception();
    return;
}

/**
 * Return the value of given variable (if legal).
 * 
 * If the variable is uninitialized, throw an exception.
 */
ValuePtr SymbolTable::GetValue(Name name) {
    TimeElapse();
    int index = IsLegalVariable(name);
    if (index >= 0) {
        if (info_table[index].initialized == false) throw std::exception();
        else return info_table[index].value_ptr;
    }
    throw std::exception();
}

/**
 * Return the address of given variable (if legal).
 */
Address SymbolTable::GetAddress(Name name) {
    TimeElapse();
    int index = IsLegalVariable(name);
    if (index >= 0) return info_table[index].address;
    throw std::exception();
}

/**
 * Return the type of given variable (if legal).
 */
BasicTypePtr SymbolTable::GetType(Name name) {
    TimeElapse();
    int index = IsLegalVariable(name);
    if (index >= 0) return info_table[index].value_ptr -> GetType();
    throw std::exception();
}