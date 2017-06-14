#include "oddeven.decl.h"
#include <stdlib.h>
#include <time.h>

CProxy_Main mainProxy; /* readonly */

void shuffle(int* arr, size_t n) {
  if (n > 1) {
    size_t i;
    srand(time(NULL));
    for (i = 0; i < n-1; i++) {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = arr[j];
      arr[j] = arr[i];
      arr[i] = t;
    }
  }
}

class Main : public CBase_Main {
  double startTime;
  int numElems;
  int* elems;
  CProxy_Element elemProxy;

  public:
  Main(CkArgMsg* m) {
    // handle arguments
    if (m->argc == 2) {
      numElems = atoi(m->argv[1]);
    } else if (m->argc > 2) {
      CkAbort("Usage: ./oddeven [number of elements]?");
    }
    else {
      numElems = 10;
    }

    // allocate and fill in elements
    elems = (int*)malloc(sizeof(int) * numElems);
    for (int i = 0; i < numElems; i++) {
      elems[i] = i;
    }

    // shuffle the elements for randomization
    shuffle(elems, numElems);

    // print elements
    ckout << "Before: ";
    for (int i = 0; i < numElems; i++) {
      ckout << elems[i] << " ";
    }
    ckout << endl;

    // store proxies
    mainProxy = thisProxy;
    elemProxy = CProxy_Element::ckNew(numElems);

    // save start time
    startTime = CkWallTimer();

    // send elements
    for (int i = 0; i < numElems; i++) {
      elemProxy[i].receive(elems[i]);
    }
  }

  void done() {
    // sorting complete
    double endTime = CkWallTimer();
    CkPrintf("Odd-even sort of %d elements finished in %f seconds\n", 
        numElems, endTime - startTime);
    CkExit();
  }
};

class Element : public CBase_Element {
  unsigned int elem;

  public:
  Element() {}

  Element(CkMigrateMessage*) {}

  void receive(int elem) {
    this->elem = elem;
    ckout << "[" << thisIndex << "] " << this->elem << endl;
  }
};

#include "oddeven.def.h"
