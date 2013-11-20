#include <string>

#include "udmf.hpp"

namespace udmf {

std::string Value::asString() {
	switch (this->tag) {
	case STRING:
		return this->stringValue;
	case INT:
		return std::to_string(this->intValue);
	case DOUBLE:
		return std::to_string(this->doubleValue);
	case BOOL:
		return std::to_string(this->boolValue);
	default:
		return std::string();
	}
}

int Value::asInt() {
	switch (this->tag) {
	case STRING:
		return std::stoi(this->stringValue);
	case INT:
		return this->intValue;
	case DOUBLE:
		return static_cast<int>(this->doubleValue);
	case BOOL:
		return static_cast<int>(this->boolValue);
	default:
		return 0;
	}
}

double Value::asDouble() {
	switch (this->tag) {
	case STRING:
		return std::stod(this->stringValue);
	case INT:
		return static_cast<double>(this->intValue);
	case DOUBLE:
		return this->doubleValue;
	case BOOL:
		return static_cast<double>(this->boolValue);
	default:
		return 0.0;
	}
}

bool Value::asBool() {
	switch (this->tag) {
	case STRING: // [AM] Laziness
		return this->stringValue.compare("true") == 0;
	case INT:
		return this->intValue != 0;
	case DOUBLE:
		return this->doubleValue != 0.0;
	case BOOL:
		return this->boolValue;
	default:
		return false;
	}
}

}
