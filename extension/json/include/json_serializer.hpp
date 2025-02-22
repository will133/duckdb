#pragma once

#include "json_common.hpp"
#include "duckdb/common/serializer/format_serializer.hpp"

namespace duckdb {

struct JsonSerializer : FormatSerializer {
private:
	yyjson_mut_doc *doc;
	yyjson_mut_val *current_tag;
	vector<yyjson_mut_val *> stack;

	// Skip writing property if null
	bool skip_if_null = false;
	// Skip writing property if empty string, empty list or empty map.
	bool skip_if_empty = false;

	// Get the current json value
	inline yyjson_mut_val *Current() {
		return stack.back();
	};

	// Either adds a value to the current object with the current tag, or appends it to the current array
	void PushValue(yyjson_mut_val *val);

public:
	explicit JsonSerializer(yyjson_mut_doc *doc, bool skip_if_null, bool skip_if_empty)
	    : doc(doc), stack({yyjson_mut_obj(doc)}), skip_if_null(skip_if_null), skip_if_empty(skip_if_empty) {
		serialize_enum_as_string = true;
	}

	yyjson_mut_val *GetRootObject() {
		D_ASSERT(stack.size() == 1); // or we forgot to pop somewhere
		return stack.front();
	};

	void SetTag(const char *tag) final;

	//===--------------------------------------------------------------------===//
	// Nested Types Hooks
	//===--------------------------------------------------------------------===//
	void OnOptionalBegin(bool present) final;
	void OnListBegin(idx_t count) final;
	void OnListEnd(idx_t count) final;
	void OnMapBegin(idx_t count) final;
	void OnMapEntryBegin() final;
	void OnMapEntryEnd() final;
	void OnMapKeyBegin() final;
	void OnMapValueBegin() final;
	void OnMapEnd(idx_t count) final;
	void OnObjectBegin() final;
	void OnObjectEnd() final;

	//===--------------------------------------------------------------------===//
	// Primitive Types
	//===--------------------------------------------------------------------===//
	void WriteNull() final;
	void WriteValue(uint8_t value) final;
	void WriteValue(int8_t value) final;
	void WriteValue(uint16_t value) final;
	void WriteValue(int16_t value) final;
	void WriteValue(uint32_t value) final;
	void WriteValue(int32_t value) final;
	void WriteValue(uint64_t value) final;
	void WriteValue(int64_t value) final;
	void WriteValue(hugeint_t value) final;
	void WriteValue(float value) final;
	void WriteValue(double value) final;
	void WriteValue(interval_t value) final;
	void WriteValue(const string &value) final;
	void WriteValue(const string_t value) final;
	void WriteValue(const char *value) final;
	void WriteValue(bool value) final;
};

} // namespace duckdb
