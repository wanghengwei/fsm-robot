#pragma once
#include <string>

class CEvent {
public:
    virtual ~CEvent() {}
};

class CEventLogin : public CEvent {
public:
    std::string account;
};

struct CEventRes1 : public CEvent {};

struct CEventRes2 : public CEvent {};