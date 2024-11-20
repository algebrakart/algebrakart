// Defines flag for ffn input
#pragma once

class Flag {

public:
    Flag(int index);
    ~Flag();
    bool isTrue();
    void setFlag(bool value_);
private:
    bool value_;
    // Agent index
    int agentIndex_;

};

