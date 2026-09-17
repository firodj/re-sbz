#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cstring>
#include <unordered_map>

enum class FieldType {
    INT8, UINT8, INT16, UINT16, INT32, UINT32, INT64, UINT64,
    FLOAT, DOUBLE, POINTER, STRUCT, ARRAY, CSTRING
};

class StructSchema;

struct TypeNode {
    FieldType type;
    std::shared_ptr<TypeNode> base_type;         // Used for POINTER and ARRAY
    std::shared_ptr<StructSchema> struct_schema; // Used for STRUCT
    size_t array_length = 0;                     // Used for ARRAY

    // Factories
    static std::shared_ptr<TypeNode> Scalar(FieldType t) {
        return std::make_shared<TypeNode>(TypeNode{t, nullptr, nullptr, 0});
    }
    static std::shared_ptr<TypeNode> CString() {
        return std::make_shared<TypeNode>(TypeNode{FieldType::CSTRING, nullptr, nullptr, 0});
    }
    static std::shared_ptr<TypeNode> Pointer(std::shared_ptr<TypeNode> target) {
        return std::make_shared<TypeNode>(TypeNode{FieldType::POINTER, target, nullptr, 0});
    }
    static std::shared_ptr<TypeNode> Struct(std::shared_ptr<StructSchema> schema) {
        return std::make_shared<TypeNode>(TypeNode{FieldType::STRUCT, nullptr, schema, 0});
    }
    static std::shared_ptr<TypeNode> Array(std::shared_ptr<TypeNode> base, size_t len) {
        return std::make_shared<TypeNode>(TypeNode{FieldType::ARRAY, base, nullptr, len});
    }

    // Needed to know how many bytes to jump forward when looping through an array
    size_t get_size() const; 
};

struct FieldDescriptor {
    std::string name;
    std::shared_ptr<TypeNode> type_info;
    size_t size;
    size_t alignment;
    size_t offset;
};

class StructSchema {
public:
    std::string name;
    std::vector<FieldDescriptor> fields;
    std::unordered_map<std::string, size_t> field_indices;  
    size_t total_size = 0;

    StructSchema(const std::string& name) : name(name) {}

    void add_field(const std::string& fn, std::shared_ptr<TypeNode> type_info);
    const FieldDescriptor& get_field(const std::string& field_name) const;
};

class DynamicInspector {
private:
    static void traverse_internal(void* raw_memory, std::shared_ptr<TypeNode> type_info, std::stringstream& out, int depth);

public:
    static std::string inspect(void* raw_memory, std::shared_ptr<TypeNode> type_info);
};

class DynamicStruct {
private:
    std::shared_ptr<StructSchema> schema;
    std::vector<uint8_t> memory;

public:
    DynamicStruct(std::shared_ptr<StructSchema> schema) : schema(schema) {
        // Allocate raw memory zeroed out
        memory.resize(schema->total_size, 0);
    }

    // Generic setter
    template <typename T>
    void set(const std::string& field_name, T value) {
        const FieldDescriptor& field = schema->get_field(field_name);
        if (sizeof(T) > field.size && field.type_info->type != FieldType::ARRAY) {
            throw std::runtime_error("Value size exceeds field size");
        }
        std::memcpy(memory.data() + field.offset, &value, sizeof(T));
    }

    // Generic getter
    template <typename T>
    T get(const std::string& field_name) const {
        const FieldDescriptor& field = schema->get_field(field_name);
        T value;
        std::memcpy(&value, memory.data() + field.offset, sizeof(T));
        return value;
    }

    // Get pointer to array or nested struct
    void* get_ptr(const std::string& field_name) {
        return memory.data() + schema->get_field(field_name).offset;
    }

    uint8_t* data() {
        return memory.data();
    }

    size_t get_total_size() const { return schema->total_size; }
};