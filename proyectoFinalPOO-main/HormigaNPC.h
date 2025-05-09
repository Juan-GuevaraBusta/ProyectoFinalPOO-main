#ifndef HORMIGA_NPC_H
#define HORMIGA_NPC_H

#include "HormigaNormal.h"

class HormigaNPC : public HormigaNormal {
public:
    HormigaNPC();
    virtual ~HormigaNPC();
    void caminarPredeterminado();
};

#endif //HORMIGANPC_H
