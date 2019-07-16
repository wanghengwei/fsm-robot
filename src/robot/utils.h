#pragma once

template<typename E>
E* eventCast(void* ve) {
    if (ve == nullptr) {
        return nullptr;
    }

    IEvent* e = static_cast<IEvent*>(ve);
    if (e->GetCLSID() == E::_GetCLSID()) {
        return static_cast<E*>(e);
    }

    return nullptr;
}
