#include "duckdb/parser/statement/select_statement.hpp"

#include "duckdb/common/serializer.hpp"
#include "duckdb/common/serializer/format_serializer.hpp"
#include "duckdb/common/serializer/format_deserializer.hpp"

namespace duckdb {

SelectStatement::SelectStatement(const SelectStatement &other) : SQLStatement(other), node(other.node->Copy()) {
}

unique_ptr<SQLStatement> SelectStatement::Copy() const {
	return unique_ptr<SelectStatement>(new SelectStatement(*this));
}

void SelectStatement::Serialize(Serializer &serializer) const {
	node->Serialize(serializer);
}

void SelectStatement::FormatSerialize(FormatSerializer &serializer) const {
	node->FormatSerialize(serializer);
}

unique_ptr<SelectStatement> SelectStatement::Deserialize(Deserializer &source) {
	auto result = make_unique<SelectStatement>();
	result->node = QueryNode::Deserialize(source);
	return result;
}

unique_ptr<SelectStatement> SelectStatement::FormatDeserialize(FormatDeserializer &deserializer) {
	auto result = make_unique<SelectStatement>();
	deserializer.ReadProperty("node", result->node);
	return result;
}

bool SelectStatement::Equals(const SQLStatement *other_p) const {
	if (type != other_p->type) {
		return false;
	}
	auto other = (SelectStatement *)other_p;
	return node->Equals(other->node.get());
}

string SelectStatement::ToString() const {
	return node->ToString();
}

} // namespace duckdb
