#include "pch.h"
#include "CppUnitTest.h"
#include "dynstruct.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testing
{
	TEST_CLASS(StructSchemaTests)
	{
	public:

		std::shared_ptr<StructSchema> vec3_schema;
		std::shared_ptr<StructSchema> player_schema;
		std::shared_ptr<StructSchema> inventory_schema;

		TEST_METHOD_INITIALIZE(TestSetup)
		{
			vec3_schema = std::make_shared<StructSchema>("Vector3");
			vec3_schema->add_field("x", TypeNode::Scalar(FieldType::FLOAT));
			vec3_schema->add_field("y", TypeNode::Scalar(FieldType::FLOAT));
			vec3_schema->add_field("z", TypeNode::Scalar(FieldType::FLOAT));

			inventory_schema = std::make_shared<StructSchema>("Inventory");
			inventory_schema->add_field("coins", TypeNode::Scalar(FieldType::INT8));
			inventory_schema->add_field("bags", TypeNode::Scalar(FieldType::INT8));

			player_schema = std::make_shared<StructSchema>("Player");
			player_schema->add_field("id", TypeNode::Scalar(FieldType::INT32));
			player_schema->add_field("name", TypeNode::CString());
			player_schema->add_field("health", TypeNode::Scalar(FieldType::FLOAT));
			player_schema->add_field("position", TypeNode::Struct(vec3_schema));
			player_schema->add_field("inventory", TypeNode::Pointer(TypeNode::Struct(inventory_schema)));
			player_schema->add_field("stats", TypeNode::Array(TypeNode::Scalar(FieldType::INT32), 3));
		}
		
		TEST_METHOD(TotalSize)
		{
			/**
			struct __fixed CSceneState
{
	CStruct_9C stc9C;
	int field_9C[9];
	float field_C0;
	float field_C4;
	__int64 field_C8;
	int field_D0;
	int gapD4[10];
	int field_FC;
	int gap100[31];
	int field_17C;
	float frameTimeMultiplied_180;
	int field_184;
	int field_188;
	float flt_18C;
};
			*/
		

			Assert::AreEqual(40, (int)player_schema->total_size);

			std::vector<uint8_t> memory(player_schema->total_size, 0);
			DynamicStruct memory_inventory(inventory_schema);
			memory_inventory.set<int8_t>("coins", 80);

			auto field_inventory = player_schema->get_field("inventory");
			Assert::AreEqual(24, (int)field_inventory.offset);
			Assert::AreEqual(4, (int)field_inventory.size);


			auto ptr = (unsigned int)memory_inventory.data();

			memcpy(memory.data() + field_inventory.offset, &ptr, sizeof(field_inventory.size));

			const char* name = "Pumpin Jenkins";
			std::memcpy(memory.data() + player_schema->fields[1].offset, &name, sizeof(char*));
			
			

			Assert::AreEqual(0, (int)player_schema->fields[0].offset);
			Assert::AreEqual(4, (int)player_schema->fields[1].offset);

			auto data = DynamicInspector::inspect(memory.data(), TypeNode::Struct(player_schema));

		
			Logger::WriteMessage("\n");
			Logger::WriteMessage(data.c_str());
			Logger::WriteMessage("\n");
			Logger::WriteMessage("done");
		}

		TEST_METHOD(FieldOffset)
		{

		}
	};
}
