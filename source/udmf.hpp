#ifndef UDMF_HPP
#define UDMF_HPP

#include <string>
#include <unordered_map>

namespace udmf {

class Value {
private:
	std::string stringValue;
	union {
		int intValue;
		double doubleValue;
		bool boolValue;
	};
	enum Tag { STRING, INT, DOUBLE, BOOL };
	Tag tag;
public:
	Value(const char* s) : stringValue(s), tag(Tag::STRING) { };
	Value(int i) : intValue(i), tag(Tag::INT) { };
	Value(double d) : doubleValue(d), tag(Tag::DOUBLE) { };
	Value(bool b) : boolValue(b), tag(Tag::BOOL) { };
	std::string asString();
	int asInt();
	double asDouble();
	bool asBool();
};

typedef std::string Identifier;
typedef std::pair<Identifier, Value> Assignment;
typedef std::unordered_map<Identifier, Value> List;

void parse(const std::string& input);

}

#endif
