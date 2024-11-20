#include "flag.h"


Flag::Flag(int index) {
    agentIndex_ = index;
}

Flag::~Flag() {

}

void Flag::setFlag(bool value_) {
    this->value_ = value_;
}

bool Flag::isTrue() { return value_; }