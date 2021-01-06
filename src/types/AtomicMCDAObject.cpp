#include "../../include/types/AtomicMCDAObject.h"

AtomicMCDAObject::AtomicMCDAObject() { ++nb_instances_; };

AtomicMCDAObject::~AtomicMCDAObject() { --nb_instances_; };

int AtomicMCDAObject::get_nb_instances() { return nb_instances_; };
