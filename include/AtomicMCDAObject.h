#ifndef ATOMIC_H
#define ATOMIC_H


class AtomicMCDAObject {
public:
  AtomicMCDAObject();

  virtual ~AtomicMCDAObject();

  static int get_nb_instances();

private:
  static int nb_instances_;
};

#endif