#include "pch.h"
#include "dynstruct.hpp"
#include <sstream>

void StructSchema::add_field(const std::string& fn, std::shared_ptr<TypeNode> type_info) {
    size_t sz = type_info->get_size();

    // Simplified alignment logic for demonstration
    size_t align = (type_info->type == FieldType::STRUCT) ? 4 : 
        (sz > 4 ? sizeof(void*) : sz); 
    if (type_info->type == FieldType::ARRAY) align = type_info->base_type->get_size();

    size_t padding = (align - (total_size % align)) % align;
    size_t offset = total_size + padding;
    total_size = offset + sz;

    field_indices[fn] = fields.size();
    fields.push_back({fn, type_info, sz, align, offset});
}

const FieldDescriptor& StructSchema::get_field(const std::string& field_name) const {
    return fields[field_indices.at(field_name)];
}

// Implement get_size() now that StructSchema is defined
size_t TypeNode::get_size() const {
    switch(type) {
    case FieldType::INT8:  case FieldType::UINT8: return 1;
    case FieldType::INT16:  case FieldType::UINT16: return 2;
    case FieldType::INT32: case FieldType::FLOAT: return 4;
    case FieldType::INT64:  case FieldType::UINT64: case FieldType::DOUBLE:  return 8;
    case FieldType::POINTER: case FieldType::CSTRING: return sizeof(void*);
    case FieldType::STRUCT: return struct_schema->total_size;
    case FieldType::ARRAY: return base_type->get_size() * array_length;
    default: return 0;
    }
}

void DynamicInspector::traverse_internal(void* raw_memory, std::shared_ptr<TypeNode> type_info, std::stringstream& out, int depth) {
    std::string indent(depth * 2, ' ');
  
    if (raw_memory == nullptr) {
        out << "NULL\n";
        return;
    }

    switch (type_info->type) {
    case FieldType::INT8: {
        int8_t val; std::memcpy(&val, raw_memory, sizeof(int8_t));
        out << (unsigned int)val << "\n";
        break;
    }
    case FieldType::INT16: {
        int16_t val; std::memcpy(&val, raw_memory, sizeof(int16_t));
        out << val << "\n";
        break;
    }
    case FieldType::INT32: {
        int32_t val; std::memcpy(&val, raw_memory, sizeof(int32_t));
        out << val << "\n";
        break;
    }
    case FieldType::INT64: {
        int64_t val; std::memcpy(&val, raw_memory, sizeof(int64_t));
        out << val << "\n";
        break;
    }
    case FieldType::FLOAT: {
        float val; std::memcpy(&val, raw_memory, sizeof(float));
        out << val << "\n";
        break;
    }
    case FieldType::CSTRING: {
        // Read the pointer, then treat the target memory as a null-terminated char array
        char* str_ptr = nullptr;
        std::memcpy(&str_ptr, raw_memory, sizeof(char*));
        if (str_ptr) out << "\"" << str_ptr << "\"\n";
        else out << "NULL\n";
        break;
    }
    case FieldType::STRUCT: {
        out << "{\n";
        for (const auto& field : type_info->struct_schema->fields) {
            out << indent << "  " << field.name << ": ";
            void* field_memory = static_cast<uint8_t*>(raw_memory) + field.offset;
            traverse_internal(field_memory, field.type_info, out, depth + 1);
        }
        out << indent << "}\n";
        break;
    }
    case FieldType::ARRAY: {
        out << "[\n";
        size_t step_size = type_info->base_type->get_size();
        for (size_t i = 0; i < type_info->array_length; ++i) {
            out << indent << "  [" << i << "]: ";
            // Calculate memory address for the i-th element
            void* element_memory = static_cast<uint8_t*>(raw_memory) + (i * step_size);
            traverse_internal(element_memory, type_info->base_type, out, depth + 1);
        }
        out << indent << "]\n";
        break;
    }
    case FieldType::POINTER: {
        // 1. Read the address stored at this memory location
        void* next_ptr = nullptr;
        std::memcpy(&next_ptr, raw_memory, sizeof(void*));

        out << "-> [" << next_ptr << "] ";

        // 2. Recursively traverse the dereferenced pointer
        if (next_ptr != nullptr) {
            traverse_internal(next_ptr, type_info->base_type, out, depth + 1);
        }
        else {
            out << "NULL\n";
        }
        break;
    }
    default:
        out << "ERROR: unknown field enum: " << (int)type_info->type << std::endl;
    }
}

std::string DynamicInspector::inspect(void* raw_memory, std::shared_ptr<TypeNode> type_info) {
    std::stringstream ss;
    traverse_internal(raw_memory, type_info, ss, 0);
    return ss.str();
}
